/******************************************************************
���ܣ��ⲿ�ж�
˵���������ⲿ�ж�ʱ PX0��Ӧ�ⲿ�ж�0 PX1��Ӧ�ⲿ�ж�1 PA-PE0ֻ��һ�� 
      NVIC_IRQChannel = EXTI9_5_IRQn  EXTI15_10_IRQn
			�����������ⲿ�ж�ʱע��ֿ���Ҫ | �߼�
�����ˣ�������
ʱ�䣺2015.4.23
�汾�ţ�1.0
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
	EXTI_InitTypeDef   EXTI_InitStructure1;  //�ж����ԵĽṹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,GPIO_PinSource0);//���ж�����IO������,��һ���������ⲿ�����й�
	EXTI_InitStructure1.EXTI_Line = EXTI_Line0;  //�����õ��ⲿ�����й�pc0-pc6
	EXTI_InitStructure1.EXTI_Mode = EXTI_Mode_Interrupt; //����EXITΪ�ж�����
  EXTI_InitStructure1.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStructure1.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure1);//��ʼ���ж�
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,GPIO_PinSource1);//���ж�����IO������,��һ���������ⲿ�����й�
	EXTI_InitStructure1.EXTI_Line = EXTI_Line1;  //�����õ��ⲿ�����й�pc0-pc6
	EXTI_InitStructure1.EXTI_Mode = EXTI_Mode_Interrupt; //����EXITΪ�ж�����
  EXTI_InitStructure1.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStructure1.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure1);//��ʼ���ж�
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ͨ���ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ͨ���ж�ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}

void EXIT(void)
{
	EXIT_GPIO_Config();
	EXIT_Config();
	NVIC_Config();
}