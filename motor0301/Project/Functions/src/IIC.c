#include "Include.h"
#define SCL_Read      GPIOB->IDR  & GPIO_Pin_0
#define SDA_Read      GPIOB->IDR  & GPIO_Pin_1
char  test=0; 				 //IIC用到
void IIC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

int IIC_Start(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_1);  //SDA = 1
	GPIO_SetBits(GPIOB,GPIO_Pin_0);  //SCL = 1
	Delay_ms(5);
	if(!SDA_Read)return 0;	//SDA线为低电平则总线忙,退出
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //SDA = 0
	Delay_ms(5);
	if(SDA_Read) return 0;	//SDA线为高电平则总线出错,退出
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //SDA = 0
	Delay_ms(5);
	return 1;
}

void IIC_Stop(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	Delay_ms(5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //SDA = 0
	Delay_ms(5);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);  //SCL = 1
	Delay_ms(5);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);  //SDA = 1
	Delay_ms(5);
}

/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	Delay_ms(2);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //SDA = 0
	Delay_ms(2);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);  //SCL = 1
	Delay_ms(2);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	Delay_ms(2);
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	Delay_ms(2);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);  //SDA = 1
	Delay_ms(2);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);  //SCL = 1
	Delay_ms(2);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	Delay_ms(2);
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
int I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	Delay_ms(2);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);  //SDA = 1			
	Delay_ms(2);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);  //SCL = 1
	Delay_ms(2);
	if(SDA_Read)
	{
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	  Delay_ms(2);
      return 0;
	}
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
	Delay_ms(2);
	return 1;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
        Delay_ms(2);
      if(SendByte&0x80)
        GPIO_SetBits(GPIOB,GPIO_Pin_1);  //SDA = 1  
      else 
        GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //SDA = 0   
        SendByte<<=1;
        Delay_ms(2);
				GPIO_SetBits(GPIOB,GPIO_Pin_0);  //SCL = 1
        Delay_ms(2);
    }
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
unsigned char I2C_RadeByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    GPIO_SetBits(GPIOB,GPIO_Pin_1);  //SDA = 1				
    while(i--)
    {
      ReceiveByte<<=1;      
      GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
      Delay_ms(2);
	    GPIO_SetBits(GPIOB,GPIO_Pin_0);  //SCL = 1
      Delay_ms(2);	
      if(SDA_Read)
      {
        ReceiveByte|=0x01;
      }
    }
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);  //SCL = 0
    return ReceiveByte;
} 
//ZRX          
//单字节写入*******************************************

int Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!IIC_Start())return 0;
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号//
	  //I2C_SendByte(((REG_Address & 0x0700) >>7) | (SlaveAddress & 0xFFFE));//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){IIC_Stop(); return 0;}
    I2C_SendByte(REG_Address );   //设置低起始地址      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    IIC_Stop(); 
    Delay_ms(5);
    return 1;
}

//单字节读取*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   unsigned char REG_data;     	
	if(!IIC_Start())return 0;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){IIC_Stop();test=1; return 0;}
    I2C_SendByte((u8) REG_Address);   //设置低起始地址      
    I2C_WaitAck();
    IIC_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	REG_data= I2C_RadeByte();
    I2C_NoAck();
    IIC_Stop();
    //return TRUE;
	return REG_data;

}						      
