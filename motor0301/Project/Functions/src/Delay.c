/******************************************************************
功能：延时 此部分包含systick延时 for延时
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
__IO uint32_t TimingDelay;

void Delay_ms(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	SysTick_Config(168000); // systick enable
  while(TimingDelay);
	SysTick->CTRL=0x00;
}

void Delay_us(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	SysTick_Config(168); // systick enable 
  while(TimingDelay);
	SysTick->CTRL=0x00;
}

void Delay_Ms(__IO uint32_t nTime)
{
	uint32_t i;
	for(i = nTime*20000;i>0;i--)
	{
	}
}

void Delay_Us(__IO uint32_t nTime)
{
	uint32_t i;
	for(i = nTime*20;i>0;i--)
	{
	}
}