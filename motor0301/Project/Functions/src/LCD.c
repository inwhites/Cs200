/******************************************************************
功能：LCD
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
/***********************************************************
  * LCD默认采用横屏显示，其中坐标表示为(x,y)
  * x LCD的行数  
  * y LCD的列数
  (0,0)     表示LCD左上角的点 
  (239,319) 表示LCD右下角的点   
  (0,2)     表示LCD第1行第3列的点行
  (238,2)   表示LCD第239行第3列的点行
************************************************************/

/* TFT彩屏控制信号信号线BitBand定义*/
#define SetCs  Periph_BB((uint32_t)&LCD_PORT_CS->ODR, LCD_PinNum_CS) = 1;
#define ClrCs  Periph_BB((uint32_t)&LCD_PORT_CS->ODR, LCD_PinNum_CS) = 0;
 

#define SetWr  Periph_BB((uint32_t)&LCD_PORT_WR->ODR, LCD_PinNum_WR) = 1;
#define ClrWr  Periph_BB((uint32_t)&LCD_PORT_WR->ODR, LCD_PinNum_WR) = 0;

#define SetRs  Periph_BB((uint32_t)&LCD_PORT_RS->ODR, LCD_PinNum_RS) = 1;
#define ClrRs  Periph_BB((uint32_t)&LCD_PORT_RS->ODR, LCD_PinNum_RS) = 0;

#define SetRd  Periph_BB((uint32_t)&LCD_PORT_RD->ODR, LCD_PinNum_RD) = 1;
#define ClrRd  Periph_BB((uint32_t)&LCD_PORT_RD->ODR, LCD_PinNum_RD) = 0;

#define LCD_Write(LCD_DATA)  LCD_PORT_DAT->ODR = LCD_DATA
#define LCD_Read()           LCD_PORT_DAT->IDR

/***********************LCD模组驱动芯片ID ***************************/
static  __IO uint16_t DeviceIdCode;

/******************前景色TextColor和背景色BackColor******************/
static  __IO uint16_t TextColor = 0x0000, BackColor = 0xF800;

/***********************LCD当前的英文字库****************************/
sFONT *LCD_Currentfonts;
/************************选择背景是否透明****************************/
static __IO uint8_t HyalineBack = HyalineBackEn;
// HyalineBackEn           0x01  //  透明背景
// HyalineBackDis          0x00  //  不透明背景(默认)

/******************定义所需显示的汉字（字模软件）********************/
const uint8_t UserStr[2][72]=             //其中数组第一个表示显示的
	                                         //汉字的个数，第二个表示每
                                           //一个汉字的ASCII码的个数，扫屏方式必须定义为横向扫屏
{
	{0x00,0x00,0x00,0x03,0x00,0x00,0x03,0xCE,0x00,0x03,0x9E,0x00,0x07,0x1C,0x00,0x07,
0x1C,0x0C,0x0E,0x38,0x1E,0x0E,0x3F,0xFE,0x1F,0x73,0x18,0x1E,0x63,0xB0,0x3E,0xC3,
0x80,0x3F,0xF3,0x80,0x6E,0x3F,0xE0,0xCE,0x3B,0xB0,0x0E,0x33,0x98,0x0E,0x73,0x9C,
0x0E,0x63,0x8E,0x0E,0xE3,0x8E,0x0E,0xC3,0x8E,0x0F,0x83,0x86,0x0F,0x03,0x80,0x0E,
0x3F,0x80,0x0E,0x07,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x07,0x00,0x38,0x0E,0x3F,0xFC,0x0E,
0x00,0x30,0x0E,0xC0,0x60,0xFF,0xF0,0xE0,0x0C,0xE1,0xC0,0x1C,0xE1,0xC0,0x1C,0xC1,
0xC0,0x1D,0xC1,0xCC,0x19,0xFF,0xFE,0x19,0xC1,0xC3,0x39,0x81,0xC0,0x3B,0x81,0xC0,
0x1F,0x81,0xC0,0x07,0x81,0xC0,0x07,0xE1,0xC0,0x0E,0xF1,0xC0,0x1C,0x61,0xC0,0x38,
0x1F,0xC0,0xF0,0x03,0x80,0x00,0x00,0x00},
	
};

/*******************************************************************************
* 函数名         : LCD_DB_AS_InPut
* 函数功能       : 将16根数据信号线配置为输入模式,DB0 -- DB15
* 输入参数       : 无
* 输出参数       : 无
* 返回值         : 无.
*******************************************************************************/
void LCD_DB_AS_InPut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = LCD_PIN_DATA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(LCD_PORT_DAT, &GPIO_InitStructure);
}

/**					
	*名称：void LCD_WindowMax()
	*参数：
	*返回：无
	*功能：设置窗口
	*备注：
**/
void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end) 
{
	LCD_WriteReg(0x44,x|((x_end-1)<<8));
	LCD_WriteReg(0x45,y);
	LCD_WriteReg(0x46,y_end-1);
}


/*******************************************************************************
* 函数名         : LCD_DB_AS_OutPut
* 函数功能       : 将16根数据信号线配置为输出模式 DB0 -- DB15
* 输入参数       : 无
* 输出参数       : 无
* 返回值         : 无.
*******************************************************************************/
void LCD_DB_AS_OutPut(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = LCD_PIN_DATA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(LCD_PORT_DAT, &GPIO_InitStructure);
}

/****************************************************************************
* 名    称：LCD_Init()
* 功    能：初始化 LCD 
* 入口参数：无
* 出口参数：无
* 返 回 值：无
****************************************************************************/
void LCD_Init(void)
{ 
  /* 初始化配置TFT彩屏的引脚*/
  LCD_Pins_Config();
  LCD_WriteReg(0x0000,0x0001);          //启动内部震荡器
  //Delay_ms(50); /* Delay 50 ms */
  DeviceIdCode = LCD_ReadReg(0x0000);   //读取芯片ID
	if(DeviceIdCode==0x9320||DeviceIdCode==0x9300)
    {
        LCD_WriteReg(0x00,0x0001);
        LCD_WriteReg(0x01,0x0100);      //Driver Output Contral. 0x0100
        LCD_WriteReg(0x02,0x0700);      //LCD Driver Waveform Contral.
        LCD_WriteReg(0x03,0x1018);      //Entry Mode Set. 0x1018 0x1030
    
        LCD_WriteReg(0x04,0x0000);      //Scalling Contral.
        LCD_WriteReg(0x08,0x0202);      //Display Contral 2.(0x0207)
        LCD_WriteReg(0x09,0x0000);      //Display Contral 3.(0x0000)
        LCD_WriteReg(0x0a,0x0000);      //Frame Cycle Contal.(0x0000)
        LCD_WriteReg(0x0c,(1<<0));      //Extern Display Interface Contral 1.(0x0000)
        LCD_WriteReg(0x0d,0x0000);      //Frame Maker Position.
        LCD_WriteReg(0x0f,0x0000);      //Extern Display Interface Contral 2.
    
        //Delay_ms(50);
        LCD_WriteReg(0x07,0x0101);      //Display Contral.
        //Delay_ms(50);
    
        LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));     //Power Control 1.(0x16b0)
        LCD_WriteReg(0x11,0x0007);                                  //Power Control 2.(0x0001)
        LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));                    //Power Control 3.(0x0138)
        LCD_WriteReg(0x13,0x0b00);                                  //Power Control 4.
        LCD_WriteReg(0x29,0x0000);                                  //Power Control 7.
        LCD_WriteReg(0x2b,(1<<14)|(1<<4));
        
        LCD_WriteReg(0x50,0);           //Set X Start.
        LCD_WriteReg(0x51,239);         //Set X End.
        LCD_WriteReg(0x52,0);           //Set Y Start.
        LCD_WriteReg(0x53,319);         //Set Y End.
        
        LCD_WriteReg(0x60,0x2700);      //Driver Output Control.
        LCD_WriteReg(0x61,0x0001);      //Driver Output Control.
        LCD_WriteReg(0x6a,0x0000);      //Vertical Srcoll Control.
    
        LCD_WriteReg(0x80,0x0000);      //Display Position? Partial Display 1.
        LCD_WriteReg(0x81,0x0000);      //RAM Address Start? Partial Display 1.
        LCD_WriteReg(0x82,0x0000);      //RAM Address End-Partial Display 1.
        LCD_WriteReg(0x83,0x0000);      //Displsy Position? Partial Display 2.
        LCD_WriteReg(0x84,0x0000);      //RAM Address Start? Partial Display 2.
        LCD_WriteReg(0x85,0x0000);      //RAM Address End? Partial Display 2.
    
        LCD_WriteReg(0x90,(0<<7)|(16<<0));    //Frame Cycle Contral.(0x0013)
        LCD_WriteReg(0x92,0x0000);      //Panel Interface Contral 2.(0x0000)
        LCD_WriteReg(0x93,0x0001);      //Panel Interface Contral 3.
        LCD_WriteReg(0x95,0x0110);      //Frame Cycle Contral.(0x0110)
        LCD_WriteReg(0x97,(0<<8));      //
        LCD_WriteReg(0x98,0x0000);      //Frame Cycle Contral.
        Delay_ms(5);
        LCD_WriteReg(0x07,0x0173);      //(0x0173)
        Delay_ms(5);
    }
	else if(DeviceIdCode==0x8989)
    {
        LCD_WriteReg(0x0000,0x0001);    //打开晶振
        LCD_WriteReg(0x0003,0xA8A4);    //0xA8A4
//		LCD_WriteReg(0x0003,0x6664);    //0xA8A4
        LCD_WriteReg(0x000C,0x0000);    
        LCD_WriteReg(0x000D,0x080C);    
        LCD_WriteReg(0x000E,0x2B00);    
        LCD_WriteReg(0x001E,0x00B0);    
        LCD_WriteReg(0x0001,0x293F);    //驱动输出控制320*240  0x693F  0x2B3F 0x293F
        LCD_WriteReg(0x0002,0x0600);     //LCD Driving Waveform control
        LCD_WriteReg(0x0010,0x0000);    
        LCD_WriteReg(0x0011,0x6078);    //0x4030    //定义数据格式  16位色 0x6070    横屏 0x6058
        LCD_WriteReg(0x0005,0x0000);    
        LCD_WriteReg(0x0006,0x0000);    
        LCD_WriteReg(0x0016,0xEF1C);    
        LCD_WriteReg(0x0017,0x0003);    
        LCD_WriteReg(0x0007,0x0233);    //0x0233       
        LCD_WriteReg(0x000B,0x0000);    
        LCD_WriteReg(0x000F,0x0000);    //扫描开始地址
        LCD_WriteReg(0x0041,0x0000);    
        LCD_WriteReg(0x0042,0x0000);    
        LCD_WriteReg(0x0048,0x0000);    
        LCD_WriteReg(0x0049,0x013F);    
        LCD_WriteReg(0x004A,0x0000);    
        LCD_WriteReg(0x004B,0x0000);    
        LCD_WriteReg(0x0044,0xEF00);    //Specify the start/end positions of the window address in the horizontal direction by an address unit
        LCD_WriteReg(0x0045,0x0000);    //Specify the start positions of the window address in the vertical direction by an address unit 
        LCD_WriteReg(0x0046,0x013F);    //Specify the end positions of the window address in the vertical direction by an address unit 
        LCD_WriteReg(0x0030,0x0707);    
        LCD_WriteReg(0x0031,0x0204);    
        LCD_WriteReg(0x0032,0x0204);    
        LCD_WriteReg(0x0033,0x0502);    
        LCD_WriteReg(0x0034,0x0507);    
        LCD_WriteReg(0x0035,0x0204);    
        LCD_WriteReg(0x0036,0x0204);    
        LCD_WriteReg(0x0037,0x0502);    
        LCD_WriteReg(0x003A,0x0302);    
        LCD_WriteReg(0x003B,0x0302);    
        LCD_WriteReg(0x0023,0x0000);    
        LCD_WriteReg(0x0024,0x0000);    
        LCD_WriteReg(0x0025,0xC000);    //Frame Frequency Control 
        LCD_WriteReg(0x004e,0);         //列(X)首址0
        LCD_WriteReg(0x004f,0);         //行(Y)首址0
    }
	else if(DeviceIdCode==0x9325||DeviceIdCode==0x9328)
    {
        LCD_WriteReg(0x00e3,0x3008);
        LCD_WriteReg(0x00e7,0x0012);
        LCD_WriteReg(0x00ef,0x1231);        //Set the internal vcore voltage
//        LCD_WriteReg(0x00e7,0x0010);      
        LCD_WriteReg(0x0000,0x0001);        //start internal osc
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700);        //Driving Wave Control                   
        LCD_WriteReg(0x0003,(1<<12)|(0<<5)|(1<<4)|(1<<3) );     //65K 
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0207);               
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000);        //display setting         
        LCD_WriteReg(0x000c,0x0001);        //display setting          
        LCD_WriteReg(0x000d,0x0000);        //0f3c          
        LCD_WriteReg(0x000f,0x0000);
        //Power On sequence //
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
        Delay_ms(5);
        Delay_ms(5);
        LCD_WriteReg(0x0010,0x1590);   
        LCD_WriteReg(0x0011,0x0227);
        Delay_ms(5);
        Delay_ms(5);
        LCD_WriteReg(0x0012,0x009c);                  
        Delay_ms(5);
        Delay_ms(5);
        LCD_WriteReg(0x0013,0x1900);   
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        Delay_ms(5);
        Delay_ms(5);
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
        Delay_ms(5);
        Delay_ms(5);
        LCD_WriteReg(0x0030,0x0007); 
        LCD_WriteReg(0x0031,0x0707);   
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04); 
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);        
        LCD_WriteReg(0x0039,0x0706);     
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        Delay_ms(5);
        Delay_ms(5);
        LCD_WriteReg(0x0050,0x0000);        
        LCD_WriteReg(0x0051,0x00ef);   
        LCD_WriteReg(0x0052,0x0000);     
        LCD_WriteReg(0x0053,0x013f);
        LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0600);  
        if(DeviceIdCode==0x9328)
        {   
            LCD_WriteReg(0x0093,0x0003);
            LCD_WriteReg(0x0095,0x0110);
            LCD_WriteReg(0x0097,0x0000);        
            LCD_WriteReg(0x0098,0x0000);  
         }
         //display on sequence     
        LCD_WriteReg(0x0007,0x0133);
    
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);
    }
    else if(DeviceIdCode==0x9331)
    {
        LCD_WriteReg(0x00E7, 0x1014);
        LCD_WriteReg(0x0001, 0x0100);   // set SS and SM bit   0x0100
        LCD_WriteReg(0x0002, 0x0200);   // set 1 line inversion
        LCD_WriteReg(0x0003, 0x1030);   // set GRAM write direction and BGR=1.     0x1030
        LCD_WriteReg(0x0008, 0x0202);   // set the back porch and front porch
        LCD_WriteReg(0x0009, 0x0000);   // set non-display area refresh cycle ISC[3:0]
        LCD_WriteReg(0x000A, 0x0000);   // FMARK function
        LCD_WriteReg(0x000C, 0x0000);   // RGB interface setting
        LCD_WriteReg(0x000D, 0x0000);   // Frame marker Position
        LCD_WriteReg(0x000F, 0x0000);   // RGB interface polarity
        //*************Power On sequence ****************//
        LCD_WriteReg(0x0010, 0x0000);   // SAP, BT[3:0], AP, DSTB, SLP, STB
        LCD_WriteReg(0x0011, 0x0007);   // DC1[2:0], DC0[2:0], VC[2:0]
        LCD_WriteReg(0x0012, 0x0000);   // VREG1OUT voltage
        LCD_WriteReg(0x0013, 0x0000);   // VDV[4:0] for VCOM amplitude
        Delay_ms(5); // Dis-charge capacitor power voltage
        LCD_WriteReg(0x0010, 0x1690);   // SAP, BT[3:0], AP, DSTB, SLP, STB
        LCD_WriteReg(0x0011, 0x0227);   // DC1[2:0], DC0[2:0], VC[2:0]
        Delay_ms(2); // Delay 50ms
        LCD_WriteReg(0x0012, 0x000C);   // Internal reference voltage= Vci;
        Delay_ms(2); // Delay 50ms
        LCD_WriteReg(0x0013, 0x0800);   // Set VDV[4:0] for VCOM amplitude
        LCD_WriteReg(0x0029, 0x0011);   // Set VCM[5:0] for VCOMH
        LCD_WriteReg(0x002B, 0x000B);   // Set Frame Rate
        Delay_ms(2); // Delay 50ms
        LCD_WriteReg(0x0020, 0x0000);   // GRAM horizontal Address
        LCD_WriteReg(0x0021, 0x0000);   // GRAM Vertical Address
        // ----------- Adjust the Gamma Curve ----------//
        LCD_WriteReg(0x0030, 0x0000);
        LCD_WriteReg(0x0031, 0x0106);
        LCD_WriteReg(0x0032, 0x0000);
        LCD_WriteReg(0x0035, 0x0204);
        LCD_WriteReg(0x0036, 0x160A);
        LCD_WriteReg(0x0037, 0x0707);
        LCD_WriteReg(0x0038, 0x0106);
        LCD_WriteReg(0x0039, 0x0707);
        LCD_WriteReg(0x003C, 0x0402);
        LCD_WriteReg(0x003D, 0x0C0F);
        //------------------ Set GRAM area ---------------//
        LCD_WriteReg(0x0050, 0x0000);   // Horizontal GRAM Start Address
        LCD_WriteReg(0x0051, 0x00EF);   // Horizontal GRAM End Address
        LCD_WriteReg(0x0052, 0x0000);   // Vertical GRAM Start Address
        LCD_WriteReg(0x0053, 0x013F);   // Vertical GRAM Start Address
        LCD_WriteReg(0x0060, 0x2700);   // Gate Scan Line
        LCD_WriteReg(0x0061, 0x0001);   // NDL,VLE, REV
        LCD_WriteReg(0x006A, 0x0000);   // set scrolling line
        //-------------- Partial Display Control ---------//
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        //-------------- Panel Control -------------------//
        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0600);
        LCD_WriteReg(0x0007,0x0021);        
        Delay_ms(2);
        LCD_WriteReg(0x0007,0x0061);
        Delay_ms(2);
        LCD_WriteReg(0x0007,0x0133);    // 262K color and display ON
        Delay_ms(2);
    }
    else if(DeviceIdCode==0x9919)
    {
        //*********POWER ON &RESET DISPLAY OFF
         LCD_WriteReg(0x28,0x0006);
         LCD_WriteReg(0x00,0x0001);
         LCD_WriteReg(0x10,0x0000);
         LCD_WriteReg(0x01,0x72ef);
         LCD_WriteReg(0x02,0x0600);
         LCD_WriteReg(0x03,0x6a38);
         LCD_WriteReg(0x11,0x6874);
         
         //  RAM WRITE DATA MASK
         LCD_WriteReg(0x0f,0x0000); 
         //  RAM WRITE DATA MASK
         LCD_WriteReg(0x0b,0x5308); 
         LCD_WriteReg(0x0c,0x0003);
         LCD_WriteReg(0x0d,0x000a);
         LCD_WriteReg(0x0e,0x2e00);
         LCD_WriteReg(0x1e,0x00be);
         LCD_WriteReg(0x25,0x8000);
         LCD_WriteReg(0x26,0x7800);
         LCD_WriteReg(0x27,0x0078);
         LCD_WriteReg(0x4e,0x0000);
         LCD_WriteReg(0x4f,0x0000);
         LCD_WriteReg(0x12,0x08d9);
         // -----------------Adjust the Gamma Curve----//
         LCD_WriteReg(0x30,0x0000);     //0007
         LCD_WriteReg(0x31,0x0104);     //0203
         LCD_WriteReg(0x32,0x0100);     //0001
         LCD_WriteReg(0x33,0x0305);     //0007
         LCD_WriteReg(0x34,0x0505);     //0007
         LCD_WriteReg(0x35,0x0305);     //0407
         LCD_WriteReg(0x36,0x0707);     //0407
         LCD_WriteReg(0x37,0x0300);     //0607
         LCD_WriteReg(0x3a,0x1200);     //0106
         LCD_WriteReg(0x3b,0x0800);         
         LCD_WriteReg(0x07,0x0033);
    }
    else if(DeviceIdCode==0x1505)
    {
        // second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LCD_WriteReg(0x0007,0x0000);
        Delay_ms(2);
        LCD_WriteReg(0x0012,0x011C);    //0x011A   why need to set several times?
        LCD_WriteReg(0x00A4,0x0001);    //NVM
        LCD_WriteReg(0x0008,0x000F);
        LCD_WriteReg(0x000A,0x0008);
        LCD_WriteReg(0x000D,0x0008);
       
        //GAMMA CONTROL/
        LCD_WriteReg(0x0030,0x0707);
        LCD_WriteReg(0x0031,0x0007);    //0x0707
        LCD_WriteReg(0x0032,0x0603); 
        LCD_WriteReg(0x0033,0x0700); 
        LCD_WriteReg(0x0034,0x0202); 
        LCD_WriteReg(0x0035,0x0002);    //?0x0606
        LCD_WriteReg(0x0036,0x1F0F);
        LCD_WriteReg(0x0037,0x0707);    //0x0f0f  0x0105
        LCD_WriteReg(0x0038,0x0000); 
        LCD_WriteReg(0x0039,0x0000); 
        LCD_WriteReg(0x003A,0x0707); 
        LCD_WriteReg(0x003B,0x0000);    //0x0303
        LCD_WriteReg(0x003C,0x0007);    //?0x0707
        LCD_WriteReg(0x003D,0x0000);    //0x1313//0x1f08
        Delay_ms(2);
        LCD_WriteReg(0x0007,0x0001);
        LCD_WriteReg(0x0017,0x0001);    //Power supply startup enable
        Delay_ms(2);
  
        //power control//
        LCD_WriteReg(0x0010,0x17A0); 
        LCD_WriteReg(0x0011,0x0217);    //reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LCD_WriteReg(0x0012,0x011E);    //0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LCD_WriteReg(0x0013,0x0F00);    //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LCD_WriteReg(0x002A,0x0000);  
        LCD_WriteReg(0x0029,0x000A);    //0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LCD_WriteReg(0x0012,0x013E);    // 0x013C  power supply on
           //Coordinates Control//
        LCD_WriteReg(0x0050,0x0000);    //0x0e00
        LCD_WriteReg(0x0051,0x00EF); 
        LCD_WriteReg(0x0052,0x0000); 
        LCD_WriteReg(0x0053,0x013F); 
        //Pannel Image Control//
        LCD_WriteReg(0x0060,0x2700); 
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006A,0x0000); 
        LCD_WriteReg(0x0080,0x0000); 
        //Partial Image Control//
        LCD_WriteReg(0x0081,0x0000); 
        LCD_WriteReg(0x0082,0x0000); 
        LCD_WriteReg(0x0083,0x0000); 
        LCD_WriteReg(0x0084,0x0000); 
        LCD_WriteReg(0x0085,0x0000); 
        //Panel Interface Control//
        LCD_WriteReg(0x0090,0x0013);    //0x0010 frenqucy
        LCD_WriteReg(0x0092,0x0300); 
        LCD_WriteReg(0x0093,0x0005); 
        LCD_WriteReg(0x0095,0x0000); 
        LCD_WriteReg(0x0097,0x0000); 
        LCD_WriteReg(0x0098,0x0000); 
  
        LCD_WriteReg(0x0001,0x0100); 
        LCD_WriteReg(0x0002,0x0700); 
        LCD_WriteReg(0x0003,0x1030); 
        LCD_WriteReg(0x0004,0x0000); 
        LCD_WriteReg(0x000C,0x0000); 
        LCD_WriteReg(0x000F,0x0000); 
        LCD_WriteReg(0x0020,0x0000); 
        LCD_WriteReg(0x0021,0x0000); 
        LCD_WriteReg(0x0007,0x0021); 
        Delay_ms(2);
        LCD_WriteReg(0x0007,0x0061); 
        Delay_ms(2);
        LCD_WriteReg(0x0007,0x0173); 
        Delay_ms(2);
    }                             
    else
    {

    }
}

//设置背景颜色
void LCD_SetBackColor(__IO uint16_t Color)
{
    BackColor = Color;
}

/****************************************************************************
* 名    称：LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint8_t Dir,uint8_t *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标 
*           pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式  取模软件img2LCD
* 调用方法：lcd_DrawPicture(0,0,0,(u16*)demo);
****************************************************************************/
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint8_t *pic)
{
    uint32_t  i=8, len;
    uint16_t temp,x,y;
  	  
  
		x=((uint16_t)(pic[2]<<8)+pic[3])-1;	  	//从图像数组里取出图像的长度
		y=((uint16_t)(pic[4]<<8)+pic[5])-1;	  	//从图像数组里取出图像的高度

		LCD_WindowMax(StartX,StartY,x,y);

		len=2*((uint16_t)(pic[2]<<8)+pic[3])*((uint16_t)(pic[4]<<8)+pic[5]);   //计算出图像所占的字节数  
		while(i<(len+8)) {							           //从图像数组的第9位开始递增
  	temp=(uint16_t)( pic[i]<<8)+pic[i+1];		 //16位总线， 需要一次发送2个字节的数据
  	*(__IO uint16_t *) (Bank1_LCD_D)=temp;	//将取出的16位像素数据送入显示区
		i=i+2;										                //取模位置加2，以为获取下一个像素数据
  }
}

/*******************************************************************************
* 函数名         : LCD_Clear
* 函数功能       : 将屏幕刷成设定的颜色
* 输入参数       : 16位的颜色数据
* 输出参数       : 无
* 返回值         : 无.
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
    uint32_t index = 0;
    LCD_SetCursor(0,0); 
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		ClrCs
		SetRs
    for(index=0; index<(LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); index++)
    {
      ClrWr
      LCD_Write(Color);
      SetWr
    }
    SetCs
}

/*******************************************************************************
* 函数名         : LCD_SetCursor
* 函数功能       : 设定光标的位置
* 参数Xpos       : 指定X位置
* 参数Ypos       : 指定Y位置
* 返回值         : 无.
*******************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    if(DeviceIdCode==0x8989)
    {
        LCD_WriteReg(0x004e,Xpos);      //行
        LCD_WriteReg(0x004f,Ypos);      //列
    }
    else if((DeviceIdCode == 0x9320))
    {
        LCD_WriteReg(0x0020,Xpos);      // 行
        LCD_WriteReg(0x0021,0x13f-Ypos);// 列
    }
    else if((DeviceIdCode == 0x9919))
    {
        LCD_WriteReg(0x004e,Xpos);      // 行
        LCD_WriteReg(0x004f,Ypos);      // 列    
    }
    else
    {
        LCD_WriteReg(0x0020,Xpos);      // 列
        LCD_WriteReg(0x0021,0x13f-Ypos);// 行
    }
}

/*******************将某一个点的颜色赋值************************/
void LCD_SetPoint(uint16_t Xpos, uint16_t Ypos, uint16_t Color)
{ 
    LCD_SetCursor(Xpos, Ypos);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(Color);
    SetCs
}

/********************显示一个像素***********************/
void PutPixel(int16_t Xpos, int16_t Ypos)
{ 
    if(Xpos < 0 || Xpos > LCD_PIXEL_HEIGHT || Ypos < 0 || Ypos > LCD_PIXEL_WIDTH)
    {
        return;  
    }
    LCD_SetPoint(Xpos, Ypos, TextColor);
}

/************************显示一个点***********************/
uint16_t LCD_GetPoint(uint16_t Xpos, uint16_t Ypos)
{ 
    uint16_t Color;
    LCD_SetCursor(Xpos, Ypos);

    /* Write 16-bit Index (then Read Reg) */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_REG_34);
    SetWr
    SetRs


    LCD_DB_AS_InPut();
    /* Read 16-bit Reg */
    ClrRd  //Dummy read
    SetRd
    ClrRd
    SetRd
    Color = LCD_Read();
    SetCs

    LCD_DB_AS_OutPut();

    return (Color);
}
/********************************************************************/
void LCD_RAM_WrRdTest(void)
{
    uint16_t ReadData = 0;  
    
    LCD_SetPoint(1,1,0x0);
    ReadData = LCD_GetPoint(1,1);
    if(0x0 != ReadData)
    {
//        LCD_DEBUG_PRINTF("\r\n Err: LCD_SetPoint(1,1,0x0) LCD_GetPoint(1,1)=0x%04X", ReadData);
    }
    LCD_SetPoint(1,1,0xFFFF);
    ReadData = LCD_GetPoint(1,1);
    if(0xFFFF != ReadData)
    {
//        LCD_DEBUG_PRINTF("\r\n Err: LCD_SetPoint(1,1,0x0) LCD_GetPoint(1,1)=0x%04X", ReadData);
    }

    LCD_SetPoint(1,1,0x5a5a);
    ReadData = LCD_GetPoint(1,1);
    if(0x5a5a != ReadData)
    {
//        LCD_DEBUG_PRINTF("\r\n Note: LCD Write order is R-G-B, but Read order is B-G-R");
    }
}


/**************************************************************************/
void LCD_DrawBigPoint(uint16_t xScreen, uint16_t yScreen, uint16_t Color)
{
    LCD_SetPoint(xScreen,   yScreen,   Color);
    LCD_SetPoint(xScreen-1, yScreen,   Color);
    LCD_SetPoint(xScreen+1, yScreen,   Color);
    LCD_SetPoint(xScreen,   yScreen-1, Color);
    LCD_SetPoint(xScreen,   yScreen+1, Color);
    LCD_SetPoint(xScreen-1, yScreen-1, Color);
    LCD_SetPoint(xScreen-1, yScreen+1, Color);
    LCD_SetPoint(xScreen+1, yScreen-1, Color);
    LCD_SetPoint(xScreen+1, yScreen+1, Color);
}

/**********************************画一条线段*********************************
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: line length.
  * @param  Direction: line direction.
  **************************************************************************/
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
    uint32_t i = 0;

    LCD_SetCursor(Xpos, Ypos);

    if(Direction == LCD_DIR_HORIZONTAL)
    { 
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
        LCD_WriteRAM(TextColor);
        for(i = 1; i < Length; i++)
        {
            ClrWr
            SetWr
        }
    }
    else
    {
        for(i = 0; i < Length; i++)
        {
            LCD_WriteRAMWord(TextColor);
            Xpos++;
            LCD_SetCursor(Xpos, Ypos);
        }
    }
    SetCs
}

/****************************画一条直线*******************************
  * @brief  Displays a Cross.
  * @param  Xpos: specifies the left X position.
  * @param  Ypos: specifies the botton Y position.
 *********************************************************************/
void LCD_DrawCross(uint16_t Xpos, uint16_t Ypos)
{
    LCD_DrawLine(Xpos, Ypos - 15, 30, LCD_DIR_HORIZONTAL); /* Horizontal Line */
    LCD_DrawLine(Xpos - 15, Ypos, 30, LCD_DIR_VERTICAL);   /* Vertical Line   */
}


/**********************************画一个矩形***********************************
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
********************************************************************************/
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
    LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
    LCD_DrawLine((Xpos + Height), Ypos, Width, LCD_DIR_HORIZONTAL);

    LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
    LCD_DrawLine(Xpos, (Ypos + Width - 1), Height, LCD_DIR_VERTICAL);
}


/******************************画一个圆**************************************
  * @brief  Displays a circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  ***************************************************************************/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t   D;    /* Decision Variable */
    uint32_t  CurX; /* Current X Value */
    uint32_t  CurY; /* Current Y Value */ 

    D = 3 - (Radius << 1);
    CurX = 0;
    CurY = Radius;

    while (CurX <= CurY)
    {
        LCD_SetCursor(Xpos + CurX, Ypos + CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos + CurX, Ypos - CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurX, Ypos + CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurX, Ypos - CurY);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos + CurY, Ypos + CurX);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos + CurY, Ypos - CurX);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurY, Ypos + CurX);
        LCD_WriteRAMWord(TextColor);
        LCD_SetCursor(Xpos - CurY, Ypos - CurX);
        LCD_WriteRAMWord(TextColor);
        if (D < 0)
        { 
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }
}


/********************************************************
  * @brief  Writes index to select the LCD register.
  * @param  LCD_Reg: address of the selected register.
  *******************************************************/
void LCD_WriteRegIndex(uint8_t LCD_Reg)
{
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_Reg);
    SetWr
    SetCs
}

/*******************************************************************************
* 函数名         : LCD_ReadReg
* 函数功能       : 读取所选的TFT彩屏寄存器的数据
* 参数LCD_Reg    : 所要读取的寄存器
* 返回值         : 寄存器中的数值.
*******************************************************************************/
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
    uint16_t data;

    /* Write 16-bit Index (then Read Reg) */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_Reg);
    SetWr

    LCD_DB_AS_InPut();

    /* Read 16-bit Reg */
    SetRs
    ClrRd
    SetRd
    data = LCD_Read(); 
    SetCs
    LCD_DB_AS_OutPut();

    return    data;
}

/*******************************************************************************
* 函数名         : LCD_WriteRAM_Prepare
* 函数功能       : 准备向LCD RAM写入数据
* 参数LCD_Reg    : 所要读取的寄存器
* 返回值         : 寄存器中的数值.
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
    /* Write 16-bit Index, then Write Reg */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_REG_34);
    SetWr
    SetRs
}


/***************************************************************
  * @brief  Writes 1 word to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  **************************************************************/
void LCD_WriteRAMWord(uint16_t RGB_Code)
{
    LCD_WriteRAM_Prepare();
    LCD_WriteRAM(RGB_Code);
    SetCs
}

/*******************************************************************************
* 函数名         : LCD_WriteReg
* 函数功能       : 向指定的LCD寄存器写入数据
* 参数LCD_Reg    : 指定要写的寄存器
* 参数LCD_Reg    : 指定写入的数据值
* 返回值         : 无.
*******************************************************************************/
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{

    /* Write 16-bit Index, then Write Reg */
    ClrCs
    ClrRs
    ClrWr
    LCD_Write(LCD_Reg);
    SetWr
    /* Write 16-bit Reg */
    SetRs
    ClrWr
    LCD_Write(LCD_RegValue);
    SetWr
    SetCs
}


/************************************************************
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  ***********************************************************/
void LCD_WriteRAM(uint16_t RGB_Code)
{
    ClrWr
    LCD_Write(RGB_Code);
    SetWr
}

/*******************************************************************************
* 函数名         : LCD_Pins_Config
* 函数功能       : 初始化配置LCD管脚
* 参数           : 无
* 返回值         : 无.
*******************************************************************************/
void LCD_Pins_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd( LCD_CLK_RS  | LCD_CLK_DATA |
                            LCD_CLK_WR | LCD_CLK_RD   |
                            LCD_CLK_CS, ENABLE);

    // DB15--0
    GPIO_InitStructure.GPIO_Pin = LCD_PIN_DATA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(LCD_PORT_DAT, &GPIO_InitStructure);

    //LCD_Pin_WR
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_WR;
    GPIO_Init(LCD_PORT_WR, &GPIO_InitStructure);

    //LCD_Pin_CS
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_CS;
    GPIO_Init(LCD_PORT_CS, &GPIO_InitStructure);

    //LCD_Pin_RS
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_RS;
    GPIO_Init(LCD_PORT_RS, &GPIO_InitStructure);

    //LCD_Pin_RD
    GPIO_InitStructure.GPIO_Pin = LCD_Pin_RD;
    GPIO_Init(LCD_PORT_RD, &GPIO_InitStructure);

    SetCs
    SetWr
    SetRd
    SetRs
}


/**-------------------------------------------------------
  * @函数名 LCD_DrawChinaChar
  * @功能   在指定座标显示中文字符
  * @参数   StartX     行起始座标
  *         StartY     列起始座标
  *         cpFontArray 字库数组的地址指针
  * @返回值 无
***------------------------------------------------------*/
void LCD_DrawChinaChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *cpFontArray)
{
    uint32_t index = 0, i = 0, j = 0;
    uint8_t Xaddress = 0;

    Xaddress = Xpos;

    LCD_SetCursor(Xaddress, Ypos);

    for(index = 0; index < 24; index++)
    {
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
        for(j = 0; j < 3; j++)
        {
            for(i = 0; i < 8; i++)
            {
                if((cpFontArray[3*index + j] & (0x80 >> i)) == 0x00)
                {
                    if(HyalineBack == HyalineBackDis)
                    {
                        LCD_WriteRAM(BackColor);
                    }
                    else
                    {
                        LCD_SetCursor(Xaddress, Ypos+(j*8)+i+1);
                        LCD_WriteRAM_Prepare();
                    }
                }
                else
                {
                    LCD_WriteRAM(TextColor);
                }
            }   
        }
        SetCs
        Xaddress++;
        LCD_SetCursor(Xaddress, Ypos);
    }
}

/**-------------------------------------------------------
  * @函数名 LCD_DisplayUserStr
  * @功能   在指定座标显示用户所设定字符字符
  * @参数   Line     行起始座标
  * @返回值 无
***------------------------------------------------------*/
void LCD_DisplayUserStr(uint16_t Line)
{
    uint16_t num = 0;

    /* Send the string character by character on LCD */
    for(num=0; num<2; num++)
    {
        /* Display one China character on LCD */
        LCD_DrawChinaChar(Line, num*24+20, (uint8_t *)UserStr[num]);
    }
}

/****************************************************************
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  ***************************************************************/
void LCD_SetFont(sFONT *fonts)
{
    LCD_Currentfonts = fonts;
}

/*****************************************************************
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  ****************************************************************/
sFONT *LCD_GetFont(void)
{
    return LCD_Currentfonts;
}

/*****************************************************************
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  ****************************************************************/
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *cpFontArray)
{
    uint32_t index = 0, i = 0, j = 0, k = 0, y;
    uint8_t Xaddress = 0;

    Xaddress = Xpos;

    LCD_SetCursor(Xaddress, Ypos);

    for(y = 0; y < LCD_Currentfonts->Height; y++)
    {
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
        for(i = LCD_Currentfonts->Width; i > 0;)
        {
            if(i>=8)
            {
                k = 8;
            }
            else
            {
                k = i;
            }

            for(j = 0; j < k; j++)
            {
                if((cpFontArray[index] & (0x80 >> j)) == 0x00)
                {
                    if(HyalineBack == HyalineBackDis)
                    {
                        LCD_WriteRAM(BackColor); //putchar(' ');
                    }
                    else
                    {
                        LCD_SetCursor(Xaddress, Ypos+(LCD_Currentfonts->Width - i)+1);
                        LCD_WriteRAM_Prepare();
                    }
                }
                else
                {
                    LCD_WriteRAM(TextColor); //putchar('*');
                }
                i--;
                //Delay(2);//此处加延迟可以清楚的看到组成字符的一个个的点绘制过程
            }
            index++;
        }   
        SetCs
        Xaddress++;
        LCD_SetCursor(Xaddress, Ypos);
    }
}

/*****************************************************************
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  ****************************************************************/
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
    Ascii -= 32;
    LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->BytesPreChar]);
}


/*****************************************************************
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  ****************************************************************/
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
  uint16_t refcolumn = 0;

    while ((*ptr != 0))
    {
        /* 显示字符到彩屏上 */
        LCD_DisplayChar(Line, refcolumn, *ptr);

	    /*判断显示是否超出了范围*/
        refcolumn += LCD_Currentfonts->Width;
        if(refcolumn + LCD_Currentfonts->Width > LCD_PIXEL_WIDTH)
        {
            break;
        }
        ptr++;
    }
}


void LCD_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr)
{
    uint16_t refcolumn = Ypos;

    /* Send the string character by character on lCD */
    //while ((*ptr != 0) & (((refcolumn + 1) & 0xFFFF) <= LCD_Currentfonts->Width))
    while ((*ptr != 0))
    {
        /* Display one character on LCD */
        LCD_DisplayChar(Xpos, refcolumn, *ptr);
        /* Decrement the column position by 16 */
        refcolumn += LCD_Currentfonts->Width;
        if(refcolumn + LCD_Currentfonts->Width > LCD_PIXEL_WIDTH)
        {
            break;
        }
        /* Point on the next character */
        ptr++;
    }
}

/**-------------------------------------------------------
  * @函数名 LCD_DisplayCurrentFont
  * @功能   将系统当前使用的ASCII字库的所有字符展示一次
  * @参数   无
  * @返回值 无
  * 作者    www.armjishu.com
***------------------------------------------------------*/
void LCD_DisplayCurrentFont(void)
{
    uint16_t cChar = 0, line = 0, refcolumn = 0;

    uint8_t sStr[20] = {0};

    sprintf((char *)sStr, "SIZE:%dX%d", LCD_Currentfonts->Width, LCD_Currentfonts->Height);

    LCD_Clear(LCD_COLOR_WHITE);
    LCD_DisplayStringLine(0, sStr);
    line = LCD_Currentfonts->Height + 1;
    for(cChar = 0x20; cChar<=0x7F; cChar++)
    {
        LCD_DisplayChar(line, refcolumn, cChar);

        refcolumn += LCD_Currentfonts->Width;
        refcolumn++;

        if(refcolumn + LCD_Currentfonts->Width > LCD_PIXEL_WIDTH)
        {
            line += LCD_Currentfonts->Height;
            line++;
            refcolumn = 0;
            if((line+LCD_Currentfonts->Height) > LCD_PIXEL_HEIGHT)
            {
                line = LCD_Currentfonts->Height + 1;
                if(cChar<=0x7F)
                {
                    Delay_ms(10);
                    LCD_Clear(LCD_COLOR_WHITE);
                    LCD_DisplayStringLine(0, sStr);
                }
            }
        }
    }
}

