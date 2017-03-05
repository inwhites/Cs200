//���ۿ�����
#include "spi1.h"
#include "stm32f4xx.h"

//��������ӿ�SPI�ĳ�ʼ����SPI���ó���ģʽ							  
//������ѡ��SPI1��NRF24L01���ж�д�������ȶ�SPI1���г�ʼ��
void SPI1_Init(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable SPI2 and GPIO clocks */
  RCC_AHB1PeriphClockCmd(RCC_SPI1, ENABLE);

  /*!< SPI Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_SPI1_CLK, ENABLE);

  GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_SCK_SOURCE, GPIO_AF_SPI1);
  GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_MISO_SOURCE, GPIO_AF_SPI1);
  GPIO_PinAFConfig(SPI1_GPIO_PORT, SPI1_MOSI_SOURCE, GPIO_AF_SPI1);
      
  /*!< Configure  pins: SCK */
  GPIO_InitStructure.GPIO_Pin = SPI1_SCK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure  pins: MISO */
  GPIO_InitStructure.GPIO_Pin = SPI1_MISO;
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure  pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = SPI1_MOSI;
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure);

  /* SPI1 configuration *///��ʼ��SPI1�ṹ��
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1����Ϊ����ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����SPI1Ϊ��ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //SPI���ͽ���8λ֡�ṹ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //��һ��ʱ���ؿ�ʼ��������
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�ź��������ʹ��SSIλ������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; //SPI������Ԥ��ƵֵΪ8
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ݴ����MSBλ��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7; //CRCֵ����Ķ���ʽ

	SPI_Init(SPI1, &SPI_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI2�Ĵ���
	
	/* Enable SPI2  */
	SPI_Cmd(SPI1, ENABLE);                                              //ʹ��SPI1����
	
	SPI1_ReadWriteByte(0xff);                                           //��������		 
}  

u8 SPI1_ReadWriteByte(u8 TxData)                                        //SPI��д���ݺ���
{		
	u8 retry=0;				 	
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)      //���ͻ����־λΪ��
		{
		retry++;
		if(retry>200)return 0;
		}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, TxData);                                    //ͨ������SPI1����һ������
	retry=0;
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);   //���ջ����־λ��Ϊ��
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);                                 //ͨ��SPI1���ؽ�������				    
}






















