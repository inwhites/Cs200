#include "Include.h"
unsigned char TX_DATA[4];  	 //显示据缓存区
unsigned char BUF[10];       //接收数据缓存区
short T_X,T_Y,T_Z,T_T;		 //X,Y,Z轴，温度

void Init_MPU6050(void)
{
  Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//解除休眠状态
	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
	Single_Write(MPU6050_Addr,CONFIG, 0x06);
	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);
}

void READ_MPU6050(void)
{
	 unsigned char BUF[10];       //接收数据缓存区
   BUF[0]=Single_Read(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,GYRO_XOUT_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=16.4; 						   //读取计算X轴数据

   BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=16.4; 						   //读取计算Y轴数据
	
   BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=16.4; 					       //读取计算Z轴数据

  // BUF[6]=Single_Read(MPU6050_Addr,TEMP_OUT_L); 
  // BUF[7]=Single_Read(MPU6050_Addr,TEMP_OUT_H); 
  // T_T=(BUF[7]<<8)|BUF[6];
  // T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
}

void DATA_printf(unsigned char *s,short temp_data)
{
	if(temp_data<0)
	{
		temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30; 	
}

 void Send_Data(unsigned char axis)
 {unsigned char i;
  USART_SendData(USART3,axis);
	Delay_ms(10);
  USART_SendData(USART3,':');
	Delay_ms(10);
  for(i=0;i<4;i++)
	{
		USART_SendData(USART3,TX_DATA[i]);
		Delay_ms(20);
	}
  USART_SendData(USART3,' ');
  USART_SendData(USART3,' ');
 }

void readdmp() 
{
	unsigned char receice[42];
	unsigned char dmpnum;
	IIC_Start();
	I2C_SendByte(MPU6050_Addr);   
	I2C_SendByte(0x74);     
	IIC_Start();
	I2C_SendByte(MPU6050_Addr + 1);  
	for (dmpnum = 0; dmpnum < 41; dmpnum++) 
	{
		receice[dmpnum] = I2C_RadeByte();
		I2C_Ack();
	};
	receice[41] = I2C_RadeByte();
	I2C_Ack();                
	IIC_Stop();                   
}
