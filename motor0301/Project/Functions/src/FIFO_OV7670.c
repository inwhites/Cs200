#include "FIFO_OV7670.h"
#include "LCD.h"
#include "st7735r.h"
uint16_t C[]=
{
	LCD_COLOR_YELLOW,
	LCD_COLOR_GREEN,
	LCD_COLOR_RED,
	LCD_COLOR_WHITE,
	LCD_COLOR_BLACK,
	LCD_COLOR_CYAN,
	LCD_COLOR_MAGENTA,
	LCD_COLOR_BLUE2
};
uint8_t count=0;
void FIFO_PortConfig(void)
	
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	
  RCC_AHB1PeriphClockCmd(FIFO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin =  FIFO_WR_PIN ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_Init(FIFO_WR_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  FIFO_WRST_PIN;
	GPIO_Init(FIFO_WRST_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  FIFO_RRST_PIN;
	GPIO_Init(FIFO_RRST_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  FIFO_OE_PIN;
	GPIO_Init(FIFO_OE_GPIO_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin =  FIFO_RCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(FIFO_RCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=FIFO_Data_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_Init(FIFO_Data_GPIO_PORT, &GPIO_InitStructure);
	
	FIFO_OE_EN;
	FIFO_WR_EN;
}

void FIFO_Prepare(void)
{
	FIFO_RRST_EN;
	__nop();
	FIFO_RCK_0;
	__nop();
	__nop();
	FIFO_RCK_1;
	__nop();
	__nop();
	FIFO_RCK_0;
	__nop();
	__nop();
	FIFO_RCK_1;
	__nop();
	__nop();
	
	FIFO_RRST_DIS;
	__nop();
	FIFO_RCK_0;
	__nop();
	__nop();
	FIFO_RCK_1;	
	__nop();
}

void FIFO_Show(void)
{
	uint16_t i,j;
	uint16_t Color;
	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			Color=0;
			FIFO_RCK_0;
			Color=(uint16_t)((FIFO_Data_GPIO_PORT->IDR<<8)&0xff00);
			FIFO_RCK_1;
			__nop();
			FIFO_RCK_0;
			Color|=(uint16_t)(FIFO_Data_GPIO_PORT->IDR&0x00ff);
			FIFO_RCK_1;
			//LCD_SetPoint(i,j,Color);
			if(i<128 && j <128)
			{
				st7735r_piont(i,j,Color);
			}
		}
	}
		
}


void FIFO_Show_RGB_YUV(void)
{
	uint16_t i,j;
	uint16_t Y,Color,R,G,B;

	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			Color=0;
			FIFO_RCK_0;
			Color=(uint16_t)((FIFO_Data_GPIO_PORT->IDR<<8)&0xff00);
			FIFO_RCK_1;
			R=(Color&0xf800)>>8;
			FIFO_RCK_0;
			Color|=(uint16_t)(FIFO_Data_GPIO_PORT->IDR&0x00ff);
			G=(Color&0x07e0)>>3;
			B=(Color&0x001f)<<3;
			FIFO_RCK_1;
			
			Y=(31*R+59*G+11*B)/100;
			
			if(Y>90) Color=0xffff;
			else Color=0;
			if(i<128 && j <128)
			{
				st7735r_piont(i,j,Color);
			}
		}
	}
		
}










