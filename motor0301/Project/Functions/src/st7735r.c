#include "st7735r.h"
#include "font.h"
#include "COLOR.h"

void gpio_init(GPIO_TypeDef* GPIOx,uint32_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed,GPIOOType_TypeDef GPIO_OType,GPIOPuPd_TypeDef GPIO_PuPd)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd; 
	GPIO_Init(GPIOx, &GPIO_InitStructure); 
}

//oled显示屏驱动
u8   st7735r_dir=ST7735R_DIR;
u16  st7735r_h=ST7735R_H;
u16  st7735r_w=ST7735R_W;

/***********************************************************
* author:肖丰
* data:2015-5-7
* brief:初始化st7735r
* param:None
*retval:None
*************************************************************/
void ST7735R_init(void)
{
//显示屏管脚初始化
	  gpio_init(ST7735R_CS_PORT,ST7735R_CS_Pin,GPIO_Mode_OUT,GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_UP);
	  gpio_init(ST7735R_RST_PORT,ST7735R_RST_Pin,GPIO_Mode_OUT,GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_UP);
	  gpio_init(ST7735R_RS_PORT,ST7735R_RS_Pin,GPIO_Mode_OUT,GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_UP);
	  gpio_init(ST7735R_WR_PORT,ST7735R_WR_Pin,GPIO_Mode_OUT,GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_UP);
	  gpio_init(ST7735R_RD_PORT,ST7735R_RD_Pin,GPIO_Mode_OUT,GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_UP);
	  gpio_init(ST7735R_D_PORT,ST7735R_D0_Pin |ST7735R_D1_Pin |ST7735R_D2_Pin |ST7735R_D3_Pin |
	                           ST7735R_D4_Pin |ST7735R_D5_Pin |ST7735R_D6_Pin |ST7735R_D7_Pin ,
	            GPIO_Mode_OUT,GPIO_Speed_100MHz,GPIO_OType_PP,GPIO_PuPd_UP);
// 初始化总线
	 ST7735R_RST=0;
	 Delay_ms(100);
	 ST7735R_RST=1;
	 Delay_ms(500);

	 ST7735R_WR_CMD(0x11);   //Sleep exit 退出睡眠模式
	 Delay_ms(120);
	
//-------------------------ST7735R Frame Rate--------------------//
/* Set the frame frequency of the full colors normal mode. */
	  ST7735R_WR_CMD(0xB1);     //In Normal Mode (Full Colors)    全屏模式
    ST7735R_WR_DATA(0x05);    //设置 RTNA Set 1-line  Period  一行周期
    ST7735R_WR_DATA(0x3A);    //设置 FPA: Front Porch
    ST7735R_WR_DATA(0x3A);    //设置 BPA: Back Porch
    //Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
    //其中 fosc = 850kHz

    /* Set the frame frequency of the Idle mode.  */
    ST7735R_WR_CMD(0xB2);     //In Idle Mode (8-colors)
    ST7735R_WR_DATA(0x05);
    ST7735R_WR_DATA(0x3A);
    ST7735R_WR_DATA(0x3A);

    /* Set the frame frequency of the Partial mode/ full colors. */
    ST7735R_WR_CMD(0xB3);
    ST7735R_WR_DATA(0x05);
    ST7735R_WR_DATA(0x3A);
    ST7735R_WR_DATA(0x3A);
    ST7735R_WR_DATA(0x05);
    ST7735R_WR_DATA(0x3A);
    ST7735R_WR_DATA(0x3A);

    //------------------------------------End ST7735R Frame Rate-----------------------------------------//
    ST7735R_WR_CMD(0xB4);   //Display Inversion Control  显示反转控制

    ST7735R_WR_DATA(0x07);  //ST7735R_WR_DATA(0x03); 原来为3,改为7
    // 低三位从高到低,分别为 full colors normal mode、Idle mode、
    // full Colors partial mode  的点反转或列反转,1为列反转

    ST7735R_WR_CMD(0xC0);   //Power Control Setting  电源控制设置
    ST7735R_WR_DATA(0x28);
    ST7735R_WR_DATA(0x08);
    ST7735R_WR_DATA(0x84);
		
    ST7735R_WR_CMD(0xC1);
    ST7735R_WR_DATA(0XC0);
		
    ST7735R_WR_CMD(0xC2);
    ST7735R_WR_DATA(0x0C);
    ST7735R_WR_DATA(0x00);
		
    ST7735R_WR_CMD(0xC3);
    ST7735R_WR_DATA(0x8C);
    ST7735R_WR_DATA(0x2A);
		
    ST7735R_WR_CMD(0xC4);
    ST7735R_WR_DATA(0x8A);
    ST7735R_WR_DATA(0xEE);
    //---------------------------------End ST7735R Power Sequence-------------------------------------//
    ST7735R_WR_CMD(0xC5);   //  VCOM 电压配置
    ST7735R_WR_DATA(0x0C); //  -0.725
    //------------------------------------ST7735R Gamma Sequence-----------------------------------------//
    ST7735R_WR_CMD(0xE0);
    ST7735R_WR_DATA(0x05);
    ST7735R_WR_DATA(0x1A);
    ST7735R_WR_DATA(0x0C);
    ST7735R_WR_DATA(0x0E);
    ST7735R_WR_DATA(0x3A);
    ST7735R_WR_DATA(0x34);
    ST7735R_WR_DATA(0x2D);
    ST7735R_WR_DATA(0x2F);
    ST7735R_WR_DATA(0x2D);
    ST7735R_WR_DATA(0x2A);
    ST7735R_WR_DATA(0x2F);
    ST7735R_WR_DATA(0x3C);
    ST7735R_WR_DATA(0x00);
    ST7735R_WR_DATA(0x01);
    ST7735R_WR_DATA(0x02);
    ST7735R_WR_DATA(0x10);
		
    ST7735R_WR_CMD(0xE1);
    ST7735R_WR_DATA(0x04);
    ST7735R_WR_DATA(0x1B);
    ST7735R_WR_DATA(0x0D);
    ST7735R_WR_DATA(0x0E);
    ST7735R_WR_DATA(0x2D);
    ST7735R_WR_DATA(0x29);
    ST7735R_WR_DATA(0x24);
    ST7735R_WR_DATA(0x29);
    ST7735R_WR_DATA(0x28);
    ST7735R_WR_DATA(0x26);
    ST7735R_WR_DATA(0x31);
    ST7735R_WR_DATA(0x3B);
    ST7735R_WR_DATA(0x00);
    ST7735R_WR_DATA(0x00);
    ST7735R_WR_DATA(0x03);
    ST7735R_WR_DATA(0x12);
    //------------------------------------End ST7735R Gamma Sequence-----------------------------------------//

    ST7735R_WR_CMD(0x3A); //65k mode
    ST7735R_WR_DATA(0x05);
    ST7735R_WR_CMD(0x29); //开显示 Display on
    ST7735R_WR_CMD(0x2c); //
	
	  ST7735R_dir(st7735r_dir);
}

/***********************************************************
* author:肖丰
* data:2015-5-7
* brief:设置st7735_r gram 指针扫描方向
* param:u8 option:(0...3,0--0  1--90 2--180 3--270
*retval:None
*************************************************************/
void ST7735R_dir(u8 option)
{
    st7735r_dir = option;
    switch(option)
    {
    case 0:
        ST7735R_WR_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ,MV 行列交换 ,ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_DATA(0xc8);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_CMD(0x2a);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(128);

        ST7735R_WR_CMD(0x2B);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(50);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(128+50);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 1:
        ST7735R_WR_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ,MV 行列交换 ,ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_DATA(0xA8);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_CMD(0x2a);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x03);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x82);

        ST7735R_WR_CMD(0x2B);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x02);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x81);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    case 2:
        ST7735R_WR_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ,MV 行列交换 ,ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_DATA(0x08);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_CMD(0x2a);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x02);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x81);

        ST7735R_WR_CMD(0x2B);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x03);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x82);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 3:
        ST7735R_WR_CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ,MV 行列交换 ,ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        ST7735R_WR_DATA(0x68);//0xc8  0xA8 0x08 0x68

        ST7735R_WR_CMD(0x2a);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x03);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x82);

        ST7735R_WR_CMD(0x2B);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(0x00);
        ST7735R_WR_DATA(128);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    default:
        break;
    }

}

/***********************************************************
* author:肖丰
* data:2015-5-7
* brief:开窗
* param:u16 x:左上角坐标x
*       u16 y:左上角坐标y
*       u16 w:宽度
*       u16 h:高度
*retval:None
*************************************************************/
void ST7735R_setregion(u16 x,u16 y,u16 w,u16 h)
{
    if(st7735r_dir&0x01)
    {
        x+= 32;     //液晶需要偏移一下,避免四周看不到的行
    }
    else
    {
        y+= 32;     //液晶需要偏移一下,避免四周看不到的行
    }
    ST7735R_WR_CMD(0x2A);   //Partial Mode On 局部模式
    ST7735R_WR_Data(x);  
    ST7735R_WR_Data((x + w - 1));

    ST7735R_WR_CMD(0x2B);//Row Address Set  行地址设置
    ST7735R_WR_Data(y);
    ST7735R_WR_Data((y + h - 1));
		ST7735R_WR_CMD(0x2C);
}

//以下为API接口函数
/***********************************************************
* author:肖丰
* data:2015-5-7
* brief:初始化显示屏
* param:None
*retval:None
*************************************************************/
void st7735r_init(void)
{
    ST7735R_init();
	  st7735r_rectangle(0,0,ST7735R_W,ST7735R_H,BCOLOR);
}
	
/***********************************************************
* author:肖丰
* data:2015-5-7
* brief:画长方形
* param:u16 x:左上角坐标x
*       u16 y:左上角坐标y
*       u16 w:宽度
*       u16 h:高度
*       u16 rgb565:颜色
*retval:None
*************************************************************/
void st7735r_rectangle(u16 x,u16 y,u16 w,u16 h,u16 rgb565)
{
    u32 n, temp;
    temp =w*h;
    ST7735R_setregion(x,y,w,h);                                     
    for(n = 0; n < temp; n++)
    {
        ST7735R_WR_Data(rgb565);          
    }
}

/***********************************************************
* author:肖丰
* data:2015-5-7
* brief:清屏
* param: u16 rgb565:颜色
*retval:None
*************************************************************/
void st7735r_clear(u16 rgb565)
{
	 st7735r_rectangle(0,0,ST7735R_W,ST7735R_H,rgb565);
}

/***********************************************************
* author:肖丰
* data:2015-5-7
* brief:画点
* param: u16 rgb565:颜色
*retval:None
*************************************************************/
void st7735r_piont(u16 x,u16 y,u16 rgb565)
{
   st7735r_rectangle(x,y,1,1,rgb565);
}

/***********************************************************
* author:肖丰
* data:2015-5-9
* brief:显示字符
* param:u16 x:左上角坐标x
*       u16 y:左上角坐标y
*       u8 ascii:显示的字符
*       u16 color:字体颜色
*       u16 bcolor:字体背景颜色
*retval:None
*************************************************************/
void st7735r_char(u16 x,u16 y,u8 ascii,u16 color,u16 bcolor)
{
	  u8 temp, t, pos;
    if(x > (ST7735R_W-ASCII_W) || y > (ST7735R_H-ASCII_H))
    {
        return;
    }
		ST7735R_setregion(x,y,ASCII_W,ASCII_H);
    for (pos = 0; pos < ASCII_H; pos++)
    {
        temp = ascii8x16[((ascii-0x20)*16)+pos];
        for(t = 0; t < ASCII_W; t++)
        {
            if(temp & 0x80)
            {
                ST7735R_WR_Data(color);
            }
            else
            {
                ST7735R_WR_Data(bcolor);
            }
            temp <<= 1;
        }
    }
    return;
}

/***********************************************************
* author:肖丰
* data:2015-5-9
* brief:显示字符串
* param:u16 x:左上角坐标x
*       u16 y:左上角坐标y
*       u8 ascii:显示的字符串
*       u16 color:字体颜色
*       u16 bcolor:字体背景颜色
*retval:None
*************************************************************/
void st7735r_string(u16 x,u16 y,u8 *string,u16 color,u16 bcolor)
{
	  while(*string != '\0')
    {
        if(x > (ST7735R_W-ASCII_W) )//换行
        {
            x = 0;
            y += ASCII_H;
        }
        if(y > (ST7735R_H-ASCII_H))//换屏
        {
            y = 0;
            x = 0;
        }
				st7735r_char(x,y,*string,color,bcolor);
        x += ASCII_W;
        string ++ ;
    }
}

/***********************************************************
* author:肖丰
* data:2015-5-9
* brief:显示数字
* param:u16 x:左上角坐标x
*       u16 y:左上角坐标y
*       u8 num:显示的数字
*       u16 color:字体颜色
*       u16 bcolor:字体背景颜色
*retval:None
*************************************************************/
void st7735r_num(u16 x,u16 y,u32 num,u16 color,u16 bcolor)
{
	  u32 res = num;
    u8 len = 0;
    u16 tempx,tempy;
    tempy = y;
    if( num == 0 )
    {
			  st7735r_char(x,y,'0',color,bcolor);
        return;
    }
    while(res) 
    {
        res /= 10;
        len++;
    }
    while(num)
    {
        tempx = x + (ASCII_W * (len--) - ASCII_W);
        st7735r_char(tempx,tempy, (num % 10) + '0', color, bcolor);
        num /= 10 ;
    }
}

/***********************************************************
* author:肖丰
* data:2015-5-9
* brief:显示数字
* param:u16 x:左上角坐标x
*       u16 y:左上角坐标y
*       u8 num:显示的数字
*       u8 max_len:最大数字长度，超过部分不显示
*       u16 color:字体颜色
*       u16 bcolor:字体背景颜色
*retval:None
*************************************************************/
void st7735r_numlen(u16 x,u16 y,u32 num,u8 max_len,u16 color,u16 bcolor)
{
	  u32 res = num;
    u8 len = 0;
    u16 tempx,tempy;
	  u16 tempw,temph;
    tempy = y;
    if( num == 0 )
    {
			  st7735r_char(x,y,'0',color,bcolor);
        x += ASCII_W;
        temph  = ASCII_H;
        tempw  = ASCII_W * (max_len - 1);
			  st7735r_rectangle(x,y,tempw,temph,bcolor);
        return;
    }
    while( res )        
    {
        res /= 10;
        len++;
    }
    if(len >= max_len )   
    {
        len = max_len;
    }
    res = len;
    while( len != 0 )
    {
			  tempx = x + (ASCII_W * (len--) - ASCII_W);
			  st7735r_char(tempx,tempy, (num % 10) + '0', color, bcolor);
        num /= 10 ;
    }
    if(res != max_len )
    {
        tempw = ASCII_W * (max_len - res);
        x += (ASCII_W * res);
        temph  = ASCII_H;
        st7735r_rectangle(x,y,tempw,temph,bcolor);
    }
}
/***********************************************************
* author:肖丰
* data:2015-5-9
* brief:显示数字
* param:u16 x:左上角坐标x
*       u16 y:左上角坐标y
*       u8 num:显示的数字
*       u8 max_len:最大数字长度，超过部分不显示
*       u16 color:字体颜色
*       u16 bcolor:字体背景颜色
*retval:None
*************************************************************/
void st7735r_nnum(u16 x,u16 y,int num,u8 max_len,u16 color,u16 bcolor)
{
	 if(num>=0)
		 st7735r_numlen(x,y,num,max_len,color,bcolor);
	 else
	 {
		 st7735r_char(x,y,'-',color,bcolor);
		 st7735r_numlen(x+8,y,-num,max_len,color,bcolor);
	 }
}

