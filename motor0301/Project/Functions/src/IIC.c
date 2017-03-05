#include "Include.h"
#define SCL_Read      GPIOB->IDR  & GPIO_Pin_0
#define SDA_Read      GPIOB->IDR  & GPIO_Pin_1
char  test=0; 				 //IIC�õ�
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
	if(!SDA_Read)return 0;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);  //SDA = 0
	Delay_ms(5);
	if(SDA_Read) return 0;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
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
int I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
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
void I2C_SendByte(u8 SendByte) //���ݴӸ�λ����λ//
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
unsigned char I2C_RadeByte(void)  //���ݴӸ�λ����λ//
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
//���ֽ�д��*******************************************

int Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
  	if(!IIC_Start())return 0;
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�//
	  //I2C_SendByte(((REG_Address & 0x0700) >>7) | (SlaveAddress & 0xFFFE));//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){IIC_Stop(); return 0;}
    I2C_SendByte(REG_Address );   //���õ���ʼ��ַ      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    IIC_Stop(); 
    Delay_ms(5);
    return 1;
}

//���ֽڶ�ȡ*****************************************
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   unsigned char REG_data;     	
	if(!IIC_Start())return 0;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//���ø���ʼ��ַ+������ַ 
    if(!I2C_WaitAck()){IIC_Stop();test=1; return 0;}
    I2C_SendByte((u8) REG_Address);   //���õ���ʼ��ַ      
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
