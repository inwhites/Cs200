#ifndef __SPI1_H
#define __SPI1_H
#include "stm32f4xx.h"

//#define Set_SD_CS          {GPIO_SetBits(GPIOB,GPIO_Pin_7);}
//#define Clr_SD_CS          {GPIO_ResetBits(GPIOB,GPIO_Pin_7);}


//#define Set_SPI_PCM1770_CS {GPIO_SetBits(GPIOB,GPIO_Pin_11);}
//#define Clr_SPI_PCM1770_CS {GPIO_ResetBits(GPIOB,GPIO_Pin_11);}


//#define Set_SPI_7843_NSS   {GPIO_SetBits(GPIOA,GPIO_Pin_4);}
//#define Clr_SPI_7843_NSS   {GPIO_ResetBits(GPIOA,GPIO_Pin_4);}

#define RCC_SPI1_CLK                RCC_APB2Periph_SPI1
#define SPI1_GPIO_PORT     GPIOB
#define RCC_SPI1      RCC_AHB1Periph_GPIOB
#define SPI1_SCK      GPIO_Pin_3
#define SPI1_MISO      GPIO_Pin_4
#define SPI1_MOSI      GPIO_Pin_5
#define SPI1_SCK_SOURCE             GPIO_PinSource3
#define SPI1_MISO_SOURCE            GPIO_PinSource4
#define SPI1_MOSI_SOURCE            GPIO_PinSource5


																					  
void SPI1_Init(void);			 //初始化SPI口 
//void SPIx_SetSpeed(u8 SpeedSet); //设置SPI速度
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
		 
#endif

