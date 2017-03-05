/******************************************************************
���ܣ������ǲ�����ȡ ���ڰ�
���߷�������ȡ����������ʹ��USART3����Ϊ���մ���
�����ˣ�������
ʱ�䣺2015.4.23
�汾�ţ�1.0
*******************************************************************/
#include "Include.h"
#define b_uart_head  0x80   //�յ�A5ͷ��־λ
#define b_rx_over    0x40   //�յ�����֡��־λ ���һ�ν��ձ�־
#define RX_BUFFER_SIZE 100  //���ջ������ֽ���

float GPS_Altitude , //�����뺣ƽ��߶�
			Latitude_GPS , //ά��
			Longitude_GPS , //����
			Speed_GPS , //��������
			Course_GPS ; //���溽��

unsigned char GPS_STA_Num ;

float 	Yaw=0.0,     //ƫ����
		    Pitch=0.0,   //������
		    Roll=0.0,    //��ת��
		    Alt=0.0,     //�߶�
		    Tempr=0.0,   //�¶�		
		    Press=0.0;   //��ѹ

int16_t Ax, Ay, Az;  //���ٶȼ�
int16_t Gx, Gy, Gz;  //������
int16_t Hx, Hy, Hz;  //������

volatile unsigned char Rx_buffer[RX_BUFFER_SIZE]; //�������ݻ�����
volatile unsigned char Rx_wr_index;               //������д����ָ��
volatile unsigned char RC_Flag;                   //����״̬��־

void Data_Receive(unsigned char Data)
{
  if(Data==0xa5) 
  { 
	  RC_Flag|=b_uart_head; //���յ�A5 ��λ��־λΪ����ͷ
    Rx_buffer[Rx_wr_index++]=Data; //��¼������
  }
  else if(Data==0x5a)
  { 
	  if(RC_Flag&b_uart_head) //�����һ�ֽ�ΪA5 �˴���Ϊ׼����ʼ��������
	  { 
			Rx_wr_index=0;  //���ָ��
		  RC_Flag&=~b_rx_over; //��֡����δ�������
    }
		else //�����һ�ֽڲ���A5
		{
			Rx_buffer[Rx_wr_index++]=Data;
		}   
		RC_Flag&=~b_uart_head; //���֡ͷ��־λ
  }
	else
	{ 
		Rx_buffer[Rx_wr_index++]=Data;
		RC_Flag&=~b_uart_head;
		if(Rx_wr_index==Rx_buffer[0]) //���չ���һ֡���ֽ���
	  {  
			RC_Flag|=b_rx_over; //��־λ��Ϊ�������
			UART2_CommandRoute(); //��ȡ����
    }
	}
  if(Rx_wr_index==RX_BUFFER_SIZE) //��������˽��շ�Χ����ֹ�����������
  Rx_wr_index--;
}
	

void UART2_CommandRoute(void)
{
	if(RC_Flag&b_rx_over) //�ж��Ƿ��Ѿ�������һ֡����
	{
		RC_Flag&=~b_rx_over; //���־λ
		if(Sum_check())
		{ 
			if(Rx_buffer[1]==0xA1) //IMU ������
			{
			UART2_Get_IMU();	      //������
			}
			if(Rx_buffer[1]==0xA2) //Motion������
			{
			UART2_Get_Motion();	  //������
			}  
			if(Rx_buffer[1]==0xA3) //Position������
			{
				UART2_Get_GPS();	    //������
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
	Longitude_GPS = ((float)temp) / ((float)10000.0);  //����
	temp = 0;
	temp = ((long int)Rx_buffer[6]<<24);
	temp |= ((long int)Rx_buffer[7]<<16);
	temp |= ((long int)Rx_buffer[8]<<8);
	temp |= ((long int)Rx_buffer[9]);
	Latitude_GPS = ((float)temp) / ((float)10000.0);  //γ��
	temp = 0;
	temp |= ((long int)Rx_buffer[10]<<8);
	temp |= ((long int)Rx_buffer[11]);
	GPS_Altitude = ((float)temp) / ((float)10.0);  //���θ߶�
	GPS_STA_Num = Rx_buffer[12];     //������������	
	temp = 0;
	temp |= ((long int)Rx_buffer[13]<<8);
	temp |= ((long int)Rx_buffer[14]);
	Course_GPS = ((float)temp) / ((float)10.0);  //GPS����
	temp = 0;
	temp |= ((long int)Rx_buffer[15]<<8);
	temp |= ((long int)Rx_buffer[16]);
	Speed_GPS = ((float)temp) / ((float)10.0);   //GPS�ٶ�
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