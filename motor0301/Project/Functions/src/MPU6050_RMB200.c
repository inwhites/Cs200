/******************************************************************
功能：陀螺仪参数读取 串口版
连线方法：读取陀螺仪数据使用USART3来作为接收串口
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
#define b_uart_head  0x80   //收到A5头标志位
#define b_rx_over    0x40   //收到完整帧标志位 完成一次接收标志
#define RX_BUFFER_SIZE 100  //接收缓冲区字节数

float GPS_Altitude , //天线离海平面高度
			Latitude_GPS , //维度
			Longitude_GPS , //经度
			Speed_GPS , //地面速率
			Course_GPS ; //地面航向

unsigned char GPS_STA_Num ;

float 	Yaw=0.0,     //偏航角
		    Pitch=0.0,   //俯仰角
		    Roll=0.0,    //滚转角
		    Alt=0.0,     //高度
		    Tempr=0.0,   //温度		
		    Press=0.0;   //气压

int16_t Ax, Ay, Az;  //加速度计
int16_t Gx, Gy, Gz;  //陀螺仪
int16_t Hx, Hy, Hz;  //磁力计

volatile unsigned char Rx_buffer[RX_BUFFER_SIZE]; //接收数据缓冲区
volatile unsigned char Rx_wr_index;               //缓冲区写数据指针
volatile unsigned char RC_Flag;                   //接收状态标志

void Data_Receive(unsigned char Data)
{
  if(Data==0xa5) 
  { 
	  RC_Flag|=b_uart_head; //接收到A5 置位标志位为数据头
    Rx_buffer[Rx_wr_index++]=Data; //记录此数据
  }
  else if(Data==0x5a)
  { 
	  if(RC_Flag&b_uart_head) //如果上一字节为A5 此处认为准备开始接收数据
	  { 
			Rx_wr_index=0;  //清空指针
		  RC_Flag&=~b_rx_over; //改帧数据未接收完毕
    }
		else //如果上一字节不是A5
		{
			Rx_buffer[Rx_wr_index++]=Data;
		}   
		RC_Flag&=~b_uart_head; //清楚帧头标志位
  }
	else
	{ 
		Rx_buffer[Rx_wr_index++]=Data;
		RC_Flag&=~b_uart_head;
		if(Rx_wr_index==Rx_buffer[0]) //接收够了一帧的字节数
	  {  
			RC_Flag|=b_rx_over; //标志位置为接收完毕
			UART2_CommandRoute(); //提取数据
    }
	}
  if(Rx_wr_index==RX_BUFFER_SIZE) //如果超出了接收范围（防止缓冲区溢出）
  Rx_wr_index--;
}
	

void UART2_CommandRoute(void)
{
	if(RC_Flag&b_rx_over) //判断是否已经接收完一帧数据
	{
		RC_Flag&=~b_rx_over; //清标志位
		if(Sum_check())
		{ 
			if(Rx_buffer[1]==0xA1) //IMU 的数据
			{
			UART2_Get_IMU();	      //读数据
			}
			if(Rx_buffer[1]==0xA2) //Motion的数据
			{
			UART2_Get_Motion();	  //读数据
			}  
			if(Rx_buffer[1]==0xA3) //Position的数据
			{
				UART2_Get_GPS();	    //读数据
			} 
		}
	}
}


unsigned char Sum_check(void)
{ 
  unsigned char i;
  unsigned int checksum=0; 
  for(i=0;i<Rx_buffer[0]-2;i++)
   checksum+=Rx_buffer[i];
  if((checksum%256)==Rx_buffer[Rx_buffer[0]-2])
   return(0x01); //Checksum successful
  else
   return(0x00); //Checksum error
}


void UART2_Get_IMU(void)
{
	int16_t temp;
	
	temp = 0;
	temp = Rx_buffer[2];
	temp <<= 8;
	temp |= Rx_buffer[3];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Yaw=(float)temp / 10.0f; 
	
	temp = 0;
	temp = Rx_buffer[4];
	temp <<= 8;
	temp |= Rx_buffer[5];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Pitch=(float)temp / 10.0f;
	
	temp = 0;
	temp = Rx_buffer[6];
	temp <<= 8;
	temp |= Rx_buffer[7];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Roll=(float)temp / 10.0f;

	temp = 0;
	temp = Rx_buffer[8];
	temp <<= 8;
	temp |= Rx_buffer[9];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Alt=(float)temp / 10.0f;
	
	temp = 0;
	temp = Rx_buffer[10];
	temp <<= 8;
	temp |= Rx_buffer[11];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Tempr=(float)temp / 10.0f;
	
	temp = 0;
	temp = Rx_buffer[12];
	temp <<= 8;
	temp |= Rx_buffer[13];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Press=(float)temp * 10.0f;
	
	Display_Data();
}


void UART2_Get_Motion(void)
{
	int16_t temp;
	
	temp = 0;
	temp = Rx_buffer[2];
	temp <<= 8;
	temp |= Rx_buffer[3];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Ax=temp;
	
	temp = 0;
	temp = Rx_buffer[4];
	temp <<= 8;
	temp |= Rx_buffer[5];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Ay=temp;
	
	temp = 0;
	temp = Rx_buffer[6];
	temp <<= 8;
	temp |= Rx_buffer[7];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Az=temp;
	
	temp = 0;
	temp = Rx_buffer[8];
	temp <<= 8;
	temp |= Rx_buffer[9];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Gx=temp;
	
	temp = 0;
	temp = Rx_buffer[10];
	temp <<= 8;
	temp |= Rx_buffer[11];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Gy=temp;
	
	temp = 0;
	temp = Rx_buffer[12];
	temp <<= 8;
	temp |= Rx_buffer[13];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Gz=temp;
	
	temp = 0;
	temp = Rx_buffer[14];
	temp <<= 8;
	temp |= Rx_buffer[15];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Hx=temp;
	
	temp = 0;
	temp = Rx_buffer[16];
	temp <<= 8;
	temp |= Rx_buffer[17];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Hy=temp;
	
	temp = 0;
	temp = Rx_buffer[18];
	temp <<= 8;
	temp |= Rx_buffer[19];
	if(temp&0x8000){
	temp = 0-(temp&0x7fff);
	}else temp = (temp&0x7fff);
	Hz=temp;
}

void UART2_Get_GPS(void)
{
	long int temp;
	temp = 0;
	temp = ((long int)Rx_buffer[2]<<24);
	temp |= ((long int)Rx_buffer[3]<<16);
	temp |= ((long int)Rx_buffer[4]<<8);
	temp |= ((long int)Rx_buffer[5]); 
	Longitude_GPS = ((float)temp) / ((float)10000.0);  //经度
	temp = 0;
	temp = ((long int)Rx_buffer[6]<<24);
	temp |= ((long int)Rx_buffer[7]<<16);
	temp |= ((long int)Rx_buffer[8]<<8);
	temp |= ((long int)Rx_buffer[9]);
	Latitude_GPS = ((float)temp) / ((float)10000.0);  //纬度
	temp = 0;
	temp |= ((long int)Rx_buffer[10]<<8);
	temp |= ((long int)Rx_buffer[11]);
	GPS_Altitude = ((float)temp) / ((float)10.0);  //海拔高度
	GPS_STA_Num = Rx_buffer[12];     //锁定卫星数量	
	temp = 0;
	temp |= ((long int)Rx_buffer[13]<<8);
	temp |= ((long int)Rx_buffer[14]);
	Course_GPS = ((float)temp) / ((float)10.0);  //GPS航向
	temp = 0;
	temp |= ((long int)Rx_buffer[15]<<8);
	temp |= ((long int)Rx_buffer[16]);
	Speed_GPS = ((float)temp) / ((float)10.0);   //GPS速度
}

void Display_Data()
{
	LCD_Clear(LCD_COLOR_YELLOW);
	Display_Yaw();
	Display_Pitch();
	Display_Roll();
	Display_Alt();
	Display_Tempr();
	Display_Press();
	if(Yaw >= 0)
		Display_Num_Int(Yaw,LCD_LINE_2,160);
	else if(Yaw < 0)
	{
		Display_Yaw_F();
		Display_Num_Int((-Yaw),LCD_LINE_2,160);
	}
	if(Pitch >= 0)
		Display_Num_Int(Pitch,LCD_LINE_3,160);
	else if(Pitch < 0)
	{
		Display_Pitch_F();
		Display_Num_Int((-Pitch),LCD_LINE_3,160);
	}
	if(Roll >= 0)
		Display_Num_Int(Roll,LCD_LINE_4,160);
	else if(Roll < 0)
	{
		Display_Roll_F();
		Display_Num_Int((-Roll),LCD_LINE_4,160);
	}
	Display_Num_Int(Alt,LCD_LINE_5,160);
	Display_Num_Int(Tempr,LCD_LINE_6,160);
	Display_Num_Int(Press,LCD_LINE_7,160);
}