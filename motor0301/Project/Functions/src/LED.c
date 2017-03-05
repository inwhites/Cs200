/*********************************************************************
函数名:  LED
制作人： 朱麒文
时间： 2015.4.22
版本号： 1.0
相关参数值：
GPIO_Speed : GPIO_Speed_2MHz;GPIO_Speed_25MHz;GPIO_Speed_50MHz;GPIO_Speed_100MHz
GPIO_Mode :GPIO_Mode_IN;GPIO_Mode_OUT;GPIO_Mode_AF;GPIO_Mode_AN
GPIO_OType :GPIO_OType_PP;GPIO_OType_OD
GPIO_PuPd :GPIO_PuPd_NOPULL;GPIO_PuPd_UP;GPIO_PuPd_DOWN
**********************************************************************/
#include "Include.h"

void Led_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
}
//参数：寄存器法 CRH 高8位 CRL 低8位 ODR 输出16位 IDR 输入  
void Led_Num(u16 Num)  //Num格式0xC000
{
	Led_GPIO_Config();
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	GPIO_ResetBits(GPIOD,GPIO_Pin_13);
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	Num = Num | GPIOD->ODR;
	GPIO_Write(GPIOD,Num);
}