#include "SCCB_OV7670.h"
#include "LCD.h"

#define DEV_ADR  ADDR_OV7670 			 /*�豸��ַ����*/

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
	return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	SDA_L;
	SCCB_delay();
	if(SDA_read) 
	return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
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
	SCL_H;		   /* SCL��Ϊ�ߵ�ƽʱ��SDA��״̬�任��ʾֹͣ�ź� */
	SCCB_delay();
	SDA_H;
	SCCB_delay();
}
static void SCCB_NoAck(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_H;	   //�ߵ�ƽΪ��Ӧ���ź�
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
	SCL_L;		//SDA�͵�ƽӦ��
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
    SCCB_SendByte( DEV_ADR );                    /* ������ַ����һ�׶�*/
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte((u8)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ���ڶ��׶� */      
    SCCB_WaitAck();	
    SCCB_SendByte(SendByte);					  /* д���ݣ������׶� */
    SCCB_WaitAck();   
    SCCB_Stop(); 
    return ENABLE;
}

int SCCB_ReadByte(u8* pBuffer,  u8 ReadAddress)
{	
	
    if(!SCCB_Start())
	{
	    return DISABLE;
	}								  /*�ڶ�����ǰҪ��д�������ṩ��ʼ��ַ*/
    SCCB_SendByte( DEV_ADR );         /* ������ַ��д������һ�׶� */
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte( ReadAddress );    /* ���õ���ʼ��ַ��д�����ڶ��׶� */      
    SCCB_WaitAck();	
    SCCB_Stop(); 
	
    if(!SCCB_Start())
	{
		return DISABLE;
	}
    SCCB_SendByte( DEV_ADR + 1 );               /* ������ַ����������һ�׶� */ 
    if(!SCCB_WaitAck())
	{
		SCCB_Stop(); 
		return DISABLE;
	}

    *pBuffer = SCCB_ReceiveByte();			    /* ��ȡ���ݣ��������ڶ��׶� */ 
	SCCB_NoAck();

    SCCB_Stop();
    return ENABLE;
}











