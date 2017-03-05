/******************************************************************
功能：步进电机
使用方法：分为同时单电机 同时双电机等 用0-3口 0x0008 4-7口 0x0080 8-11口 0x0800，12-15口 0x8000
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
unsigned char Two_Motor_Clockwise[8]={0x0008,0x000c,0x0004,0x0006,0x0002,0x0003,0x0001,0x0009};  //正向旋转 双电机同时
unsigned char Two_Motor_Anti_Clockwise[8]={0x0009,0x0001,0x0003,0x0002,0x0006,0x0004,0x000c,0x0008};//反向旋转  双电机同时
unsigned char One_Motor_Clockwise[8] = {0x0080,0x00c0,0x0040,0x0060,0x0020,0x0030,0x0010,0x0090};//正向旋转 单电机
unsigned char One_Motor_Anti_Clockwise[8] = {0x0090,0x0010,0x0030,0x0020,0x0060,0x0040,0x00c0,0x0080};//反向旋转 单电机

unsigned char Motor_2_4_Clockwise[4] = {0x0070,0x00B0,0x00D0,0x00E0};//两相 正转
unsigned char Motor_2_4_Anti_Clockwise[4] ={0x00E0,0x00D0,0x00B0,0x0070}; //两相 反转
int Turn_one = 0;
extern int Turn_Dir;

void  Two_Motor_Run(unsigned int n)
{   
	unsigned int Step;
  unsigned int Num;
	GPIO_InitTypeDef GPIO_InitStructure;  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | 
	                              GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 | GPIO_Pin_7;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
  for (Step=0; Step<8*64*n; Step++)
  {
    for (Num=0; Num<8; Num++)
    {
      GPIO_Write(GPIOA,Two_Motor_Clockwise[Num]);
      Delay_ms(10);
    }
  }
}

void  One_Motor_Run(unsigned int n)
{   
	unsigned int Step;
  unsigned int Num;
	GPIO_InitTypeDef GPIO_InitStructure;  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	if(Turn_Dir == 1 && Turn_one == 0)	
	{
		Turn_one = 1;
		for (Step=0; Step<140; Step++)
		{
			for (Num=0; Num<8; Num++)
			{
				GPIO_Write(GPIOB,(Two_Motor_Anti_Clockwise[Num]));
				Delay_ms(2);
			}
		}
	}
	if(Turn_Dir == 2 && Turn_one == 1)
	{
		Turn_one = 0;
		for (Step=0; Step<140; Step++)
		{
			for (Num=0; Num<8; Num++)
			{
				GPIO_Write(GPIOB,(Two_Motor_Clockwise[Num]));
				Delay_ms(2);
			}
		}
  }
}