/******************************************************************
���ܣ����������
���߷�����Trig PA6 Echo PA7
�����ˣ�������
ʱ�䣺2015.4.23
�汾�ţ�1.0
*******************************************************************/
#include "Include.h"

#define US_100_Trig         	GPIO_Pin_6  
#define US_100_Trig_PORT      GPIOA

#define US_100_Echo         	GPIO_Pin_0  

#define US_100_Echo_PORT      GPIOA

#define Obstacle_Y            1
#define Obstacle_N            0

void US_100_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);	
	GPIO_InitStructure.GPIO_Pin = US_100_Trig;          //���͵�ƽ����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_Init(US_100_Trig_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(US_100_Trig_PORT,US_100_Trig);
	GPIO_InitStructure.GPIO_Pin =US_100_Echo;		        //���ص�ƽ����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        //��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
 	GPIO_Init(US_100_Echo_PORT, &GPIO_InitStructure);		
}

void US_100_GPIO_Config_H_Time(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);	
	GPIO_InitStructure.GPIO_Pin = US_100_Trig;          //���͵�ƽ����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_Init(US_100_Trig_PORT, &GPIO_InitStructure);	
}

int US_100 (int Set_Distance)
{
	u32 Count;
	u32 Us_Temp;
	LCD_Clear(LCD_COLOR_YELLOW);
	GPIO_SetBits(US_100_Trig_PORT,US_100_Trig);
	Delay_us(15);
	GPIO_ResetBits(US_100_Trig_PORT,US_100_Trig);
	while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	{
		Delay_us(10);
		Count++;
	}
	Us_Temp = 340/2*Count*10/1000;  //ģ�����ɲ��3m 
	Display_Num_Int(Us_Temp/1000,LCD_LINE_7,160);
	Count=0;
	Delay_ms(60);
	if( Us_Temp/1000 > Set_Distance)
	{
  	return Obstacle_N;   //ǰ�����ϰ�
	}
	else 
	{
		return Obstacle_Y;   //ǰ�����ϰ�
	}	
}