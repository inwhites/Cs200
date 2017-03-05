#ifndef __SPI_H
#define __SPI_H
#include "stm32f4xx.h"

#define Set_SD_CS          {GPIO_SetBits(GPIOB,GPIO_Pin_7);}
#define Clr_SD_CS          {GPIO_ResetBits(GPIOB,GPIO_Pin_7);}


#define Set_SPI_PCM1770_CS {GPIO_SetBits(GPIOB,GPIO_Pin_11);}
#define Clr_SPI_PCM1770_CS {GPIO_ResetBits(GPIOB,GPIO_Pin_11);}


#define Set_SPI_7843_NSS   {GPIO_SetBits(GPIOA,GPIO_Pin_4);}
#define Clr_SPI_7843_NSS   {GPIO_ResetBits(GPIOA,GPIO_Pin_4);}

#define RCC_SPI2_CLK                RCC_APB1Periph_SPI2
#define SPI2_GPIO_PORT              GPIOB
#define RCC_SPI2                    RCC_AHB1Periph_GPIOB
#define SPI2_SCK                    GPIO_Pin_13
#define SPI2_MISO                   GPIO_Pin_14
#define SPI2_MOSI                   GPIO_Pin_15
#define SPI2_SCK_SOURCE             GPIO_PinSource13
#define SPI2_MISO_SOURCE            GPIO_PinSource14
#define SPI2_MOSI_SOURCE            GPIO_PinSource15

#define opt_check  0x00
#define opt_check1  0x01
#define opt_mot  0x02
#define opt_dx  0x03
#define opt_dy  0x04
#define opt_sum 0x06
#define opt_pixel_max 0x07
#define opt_cfg 0x0a
#define opt_extcfg 0x0b
#define opt_dol 0x0c
#define opt_dou  0x0d

#define opt_frame_period_low 0x10
#define opt_frame_period_up 0x11
#define opt_motclr 0x12
#define opt_cap 0x13
#define opt_srom  0x14

#define opt_frame_max_low 0x19
#define opt_frame_max_up 0x1a

#define opt_shut_max_low 0x1d
#define opt_shut_max_up 0x1e
#define opt_ver  0x1f

#define opt_load 0x60

																					  
void SPI2_Init(void);			 //初始化SPI口 
//void SPIx_SetSpeed(u8 SpeedSet); //设置SPI速度
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void optical_write(u8 add,u8 data);
u8 optical_read(u8 add);
void optical_init();
int optical_pixel();


#endif

