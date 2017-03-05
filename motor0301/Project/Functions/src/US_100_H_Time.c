#include "Include.h"
#define US_100_Trig         	GPIO_Pin_6  
#define US_100_Trig_PORT      GPIOA

void US_100_H_Time_Calculate(void)
{
	GPIO_SetBits(US_100_Trig_PORT,US_100_Trig);
	Delay_us(15);
	GPIO_ResetBits(US_100_Trig_PORT,US_100_Trig);
	Delay_ms(50);
}