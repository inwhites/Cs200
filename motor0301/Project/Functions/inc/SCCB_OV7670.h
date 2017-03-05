#ifndef __SCCB_H
#define __SCCB_H

#include "stm32f4xx.h"

#define ADDR_OV7670  0x42

#define SCL_GPIO_PORT  GPIOC
#define SCL_GPIO_PIN   GPIO_Pin_9

#define SDA_GPIO_PORT  GPIOD
#define SDA_GPIO_PIN   GPIO_Pin_11

#define SCL_H         GPIO_SetBits(SCL_GPIO_PORT, SCL_GPIO_PIN ) 
#define SCL_L         GPIO_ResetBits(SCL_GPIO_PORT , SCL_GPIO_PIN ) 
   
#define SDA_H         GPIO_SetBits(SDA_GPIO_PORT , SDA_GPIO_PIN) 
#define SDA_L         GPIO_ResetBits(SDA_GPIO_PORT , SDA_GPIO_PIN) 

#define SCL_read      GPIO_ReadInputDataBit(SCL_GPIO_PORT ,  SCL_GPIO_PIN) 
#define SDA_read      GPIO_ReadInputDataBit(SDA_GPIO_PORT , SDA_GPIO_PIN) 

void SCCB_GPIO_Configuration(void);
int SCCB_WriteByte( u16 WriteAddress , u8 SendByte );
int SCCB_ReadByte(u8* pBuffer,  u8 ReadAddress);


#endif
