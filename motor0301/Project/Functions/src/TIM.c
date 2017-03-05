/******************************************************************
功能：定时器中断
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
void TIM3_Int_Init(u16 Arr,u16 Psc)  //1000-1 8400-1
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
  TIM_TimeBaseInitStructure.TIM_Period = Arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=Psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=4; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}