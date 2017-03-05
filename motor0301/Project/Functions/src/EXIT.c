/******************************************************************
功能：外部中断
说明：配置外部中断时 PX0对应外部中断0 PX1对应外部中断1 PA-PE0只能一个 
      NVIC_IRQChannel = EXTI9_5_IRQn  EXTI15_10_IRQn
			另：定义两个外部中断时注意分开不要 | 逻辑
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
void EXIT_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void EXIT_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure1;  //中断属性的结构体
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,GPIO_PinSource0);//将中断线与IO线相连,后一个参数与外部引脚有关
	EXTI_InitStructure1.EXTI_Line = EXTI_Line0;  //与配置的外部引脚有关pc0-pc6
	EXTI_InitStructure1.EXTI_Mode = EXTI_Mode_Interrupt; //设置EXIT为中断请求
  EXTI_InitStructure1.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure1.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure1);//初始化中断
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,GPIO_PinSource1);//将中断线与IO线相连,后一个参数与外部引脚有关
	EXTI_InitStructure1.EXTI_Line = EXTI_Line1;  //与配置的外部引脚有关pc0-pc6
	EXTI_InitStructure1.EXTI_Mode = EXTI_Mode_Interrupt; //设置EXIT为中断请求
  EXTI_InitStructure1.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure1.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure1);//初始化中断
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //通道中断使能
	NVIC_Init(&NVIC_InitStructure);	
}

void EXIT(void)
{
	EXIT_GPIO_Config();
	EXIT_Config();
	NVIC_Config();
}