/******************************************************************
���ܣ���ʱ �˲��ְ���systick��ʱ for��ʱ
�����ˣ�������
ʱ�䣺2015.4.23
�汾�ţ�1.0
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