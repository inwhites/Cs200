/******************************************************************
功能：4x4矩阵键盘
连线方法：L4 -- L1  PB3 -- PB0
          H4 -- H1  PB7 -- PB4
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
int Num_Temp_mine = 0;
int Key_Value = 0;
int Key_value = 0;
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

int Key_Display(void)
{
	Key_Value = 0;
	Key_Init();
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	if((GPIOE->IDR & 0xf000)==0x0000)  
		return -1;
	else
	{	
		Delay_ms(5);   
		if((GPIOE->IDR & 0xf000)==0x0000) 
			return -1; 
	}
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
	switch(GPIOE->IDR & 0xf000)		                
	{
		case 0x1000:while((GPIOE->IDR & 0xf000) == 0x1000){ Key_Value=10; };	break;
		case 0x2000:while((GPIOE->IDR & 0xf000) == 0x2000){ Key_Value=0; };	break;
		case 0x4000:while((GPIOE->IDR & 0xf000) == 0x4000){ Key_Value=11; };	  break;
		case 0x8000:while((GPIOE->IDR & 0xf000) == 0x8000){ Key_Value=15; };	  break;
	}
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11);
	switch(GPIOE->IDR & 0xf000)		                
	{
		case 0x1000:while((GPIOE->IDR & 0xf000) == 0x1000){ Key_Value=7; };	break;
		case 0x2000:while((GPIOE->IDR & 0xf000) == 0x2000){ Key_Value=8; };	break;
		case 0x4000:while((GPIOE->IDR & 0xf000) == 0x4000){ Key_Value=9; };	  break;
		case 0x8000:while((GPIOE->IDR & 0xf000) == 0x8000){ Key_Value=14; };	  break;
	}

	GPIO_SetBits(GPIOE,GPIO_Pin_9);
	GPIO_ResetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11);
	switch(GPIOE->IDR & 0xf000)		              
	{
		case 0x1000:while((GPIOE->IDR & 0xf000) == 0x1000){ Key_Value=4; };	break;
		case 0x2000:while((GPIOE->IDR & 0xf000) == 0x2000){ Key_Value=5; };	break;
		case 0x4000:while((GPIOE->IDR & 0xf000) == 0x4000){ Key_Value=6; };	  break;
		case 0x8000:while((GPIOE->IDR & 0xf000) == 0x8000){ Key_Value=13; };	  break;
	}

	GPIO_SetBits(GPIOE,GPIO_Pin_8);
	GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);  
	switch(GPIOE->IDR & 0xf000)		               
	{
		case 0x1000:while((GPIOE->IDR & 0xf000) == 0x1000){ Key_Value=1; };	break;
		case 0x2000:while((GPIOE->IDR & 0xf000) == 0x2000){ Key_Value=2; };	break;
		case 0x4000:while((GPIOE->IDR & 0xf000) == 0x4000){ Key_Value=3; };	  break;
		case 0x8000:while((GPIOE->IDR & 0xf000) == 0x8000){ Key_Value=12; };	  break;
	}
	return Key_Value;		
}

void Key_Action(void)
{
	Key_value = 0;
	Key_value = Key_Display();
	if(Key_value == 15)
	{
		Led_Num(0x0000);
		Num_Temp_mine = 1;
	}
	else if(Key_value == 14)
	{
		Led_Num(0x1000);
		Num_Temp_mine = 2;
	}
	else if(Key_value == 13)
	{
		Led_Num(0x2000);
		Num_Temp_mine = 3;
	}
	else if(Key_value == 12)
	{
		Led_Num(0x3000);
	}
	else if(Key_value == 11)
	{
		Led_Num(0x4000);
		Num_Temp_mine = 4;
	}
	else if(Key_value == 10)
	{
		Led_Num(0x5000);
		Num_Temp_mine = 5;
	}
	else if(Key_value == 9)
	{
		Led_Num(0x6000);
		Num_Temp_mine = 6;
	}
	else if(Key_value == 8)
	{
		Led_Num(0x7000);
	}
	else if(Key_value == 7)
	{
		Led_Num(0x8000);
		Num_Temp_mine = 7;
	}
	else if(Key_value == 6)
	{
		Led_Num(0x9000);
		Num_Temp_mine = 8;
	}
	else if(Key_value == 5)
	{
		Led_Num(0xA000);
		Num_Temp_mine = 9;
	}
	else if(Key_value == 4)
	{
		Led_Num(0xB000);
	}
	else if(Key_value == 3)
	{
		Led_Num(0xC000);
	}
	else if(Key_value == 2)
	{
		Led_Num(0xD000);
		Num_Temp_mine = 0;
	}
	else if(Key_value == 1)
	{
		Led_Num(0xE000);
	}
	else if(Key_value == 0)
	{
		Led_Num(0xF000);
	}
}
	