#include "SCCB_OV7670.h"
#include "LCD.h"

#define DEV_ADR  ADDR_OV7670 			 /*设备地址定义*/

void SCCB_GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  SCL_GPIO_PIN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN ;
	GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStructure);
	
	SCL_H;
	SDA_H;
}

static void SCCB_delay(void)
{	
   __IO uint16_t i = 1400; 
   while(i) 
   { 
     i--; 
   } 
}

static int SCCB_Start(void)
{
	SDA_H;
	SCL_H;
	SCCB_delay();
	if(!SDA_read)
	return DISABLE;	/* SDA线为低电平则总线忙,退出 */
	SDA_L;
	SCCB_delay();
	if(SDA_read) 
	return DISABLE;	/* SDA线为高电平则总线出错,退出 */
	SDA_L;
	SCCB_delay();
	return ENABLE;
}

static void SCCB_Stop(void)
{
	SCL_L;
	SCCB_delay();
	SDA_L;
	SCCB_delay();
	SCL_H;		   /* SCL线为高电平时，SDA线状态变换表示停止信号 */
	SCCB_delay();
	SDA_H;
	SCCB_delay();
}
static void SCCB_NoAck(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_H;	   //高电平为非应答信号
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SCL_L;
	SCCB_delay();
}

static int SCCB_WaitAck(void) 	
{
	SCL_L;
	SCCB_delay();
	SDA_H;			
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;		//SDA低电平应答
	return ENABLE;
}

static void SCCB_SendByte(u8 SendByte) 
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        SCCB_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        SCCB_delay();
		SCL_H;
        SCCB_delay();
    }
    SCL_L;
}
static int SCCB_ReceiveByte(void)  
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      SCCB_delay();
	  SCL_H;
      SCCB_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

int SCCB_WriteByte( u16 WriteAddress , u8 SendByte )
{		
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
    SCCB_SendByte( DEV_ADR );                    /* 器件地址，第一阶段*/
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte((u8)(WriteAddress & 0x00FF));   /* 设置低起始地址，第二阶段 */      
    SCCB_WaitAck();	
    SCCB_SendByte(SendByte);					  /* 写内容，第三阶段 */
    SCCB_WaitAck();   
    SCCB_Stop(); 
    return ENABLE;
}

int SCCB_ReadByte(u8* pBuffer,  u8 ReadAddress)
{	
	
    if(!SCCB_Start())
	{
	    return DISABLE;
	}								  /*在读操作前要有写操作以提供起始地址*/
    SCCB_SendByte( DEV_ADR );         /* 器件地址，写操作第一阶段 */
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte( ReadAddress );    /* 设置低起始地址，写操作第二阶段 */      
    SCCB_WaitAck();	
    SCCB_Stop(); 
	
    if(!SCCB_Start())
	{
		return DISABLE;
	}
    SCCB_SendByte( DEV_ADR + 1 );               /* 器件地址，读操作第一阶段 */ 
    if(!SCCB_WaitAck())
	{
		SCCB_Stop(); 
		return DISABLE;
	}

    *pBuffer = SCCB_ReceiveByte();			    /* 读取内容，读操作第二阶段 */ 
	SCCB_NoAck();

    SCCB_Stop();
    return ENABLE;
}











