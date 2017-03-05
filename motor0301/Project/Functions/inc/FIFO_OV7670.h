#ifndef __FIFO_H
#define __FIFO_H
#include "stm32f4xx.h"

#define FIFO_CLK  RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD

#define FIFO_Data_PIN        GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
#define FIFO_Data_GPIO_PORT  GPIOD    //��λ����λ


#define FIFO_WR_PIN         	GPIO_Pin_5//GPIO_Pin_15  //��HRF ����߼��� ����WE
#define FIFO_WR_GPIO_PORT   	GPIOB

#define FIFO_WRST_PIN  				GPIO_Pin_4//GPIO_Pin_13
#define FIFO_WRST_GPIO_PORT 	GPIOB

#define FIFO_RCK_PIN 			 		GPIO_Pin_11
#define FIFO_RCK_GPIO_PORT 		GPIOB

#define FIFO_RRST_PIN  				GPIO_Pin_7
#define FIFO_RRST_GPIO_PORT 	GPIOB

#define FIFO_OE_PIN  					GPIO_Pin_6
#define FIFO_OE_GPIO_PORT 		GPIOB

#define FIFO_WR_EN  		GPIO_SetBits(FIFO_WR_GPIO_PORT,FIFO_WR_PIN)    //дʹ��
#define FIFO_WR_DIS  		GPIO_ResetBits(FIFO_WR_GPIO_PORT,FIFO_WR_PIN)   //дʧʹ��

#define FIFO_WRST_EN  	GPIO_ResetBits(FIFO_WRST_GPIO_PORT,FIFO_WRST_PIN)   //д��λʹ��
#define FIFO_WRST_DIS  	GPIO_SetBits(FIFO_WRST_GPIO_PORT,FIFO_WRST_PIN)   //д��λʧʹ��

#define FIFO_RCK_0  	  FIFO_RCK_GPIO_PORT->BSRRH = FIFO_RCK_PIN    //��ʱ����0
#define FIFO_RCK_1  	  FIFO_RCK_GPIO_PORT->BSRRL = FIFO_RCK_PIN     //��ʱ����1

#define FIFO_RRST_EN  	GPIO_ResetBits(FIFO_RRST_GPIO_PORT,FIFO_RRST_PIN)   //����λʹ��
#define FIFO_RRST_DIS  	GPIO_SetBits(FIFO_RRST_GPIO_PORT,FIFO_RRST_PIN)   //����λʧʹ��

#define FIFO_OE_EN  		GPIO_ResetBits(FIFO_OE_GPIO_PORT,FIFO_OE_PIN)   //���ʹ��
#define FIFO_OE_DIS  		GPIO_SetBits(FIFO_OE_GPIO_PORT,FIFO_OE_PIN)   //���ʧʹ��

void FIFO_PortConfig(void);    //����ͷ���żĴ�������
void FIFO_Prepare(void);    //����ͷ׼������
void FIFO_Show(void);   //����ͷͼ����ʾ����
void FIFO_Show_RGB_YUV(void);
void FIFO_GetRedDot(void);
void FIFO_Calculation(void);
void PreCalculation(void);
void SendAxisData(void);
void SendDataCal(void);
float Quick_Sqrt( float number );
#endif


