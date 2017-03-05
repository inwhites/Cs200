/*************************************************************
功能：stm32f407 函数总结  just for 2015 电设
制作人： 朱麒文 
时间：2015/4/22
版本号：1.0
**************************************************************/

#include "Include.h"

int main()
{
	/************************   begin   ****************************/
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_ClocksTypeDef   Rcc_Clock_Freqs;
	uint32_t sysclk_frequency;      /*!<  SYSCLK clock frequency expressed in Hz */
  uint32_t hclk_frequency;        /*!<  HCLK clock frequency expressed in Hz */
  uint32_t pclk1_frequency;       /*!<  PCLK1 clock frequency expressed in Hz */
  uint32_t pclk2_frequency;       /*!<  PCLK2 clock frequency expressed in Hz */ 
  uint8_t sysclk_source ;         //whitesnark sysclk
  __IO uint32_t Systick_CountNum ; // systick 
  uint32_t Systick_Relode_Val = 0 ; // whitesnark systick 	
	NVIC_InitTypeDef NVIC_InitTypeDef_inst;
  RCC_GetClocksFreq( &Rcc_Clock_Freqs );
  sysclk_frequency =  Rcc_Clock_Freqs.SYSCLK_Frequency ;
  hclk_frequency = Rcc_Clock_Freqs.HCLK_Frequency ;
  pclk1_frequency = Rcc_Clock_Freqs.PCLK1_Frequency ;
  pclk2_frequency = Rcc_Clock_Freqs.PCLK2_Frequency ;
  sysclk_source = RCC_GetSYSCLKSource(); // sysclk
	//************************ STM32F407VGT6 ********************* //  
	NVIC_SetPriorityGrouping( NVIC_PriorityGroup_0); 
  /**************************   over    *************************/
	st7735r_init();
	//YYH_Display();
	//motor_init();
	//YYH_Function_Choose_Init();	
	USART1_Config(9600);
	YYH_motor_Init();
	st7735r_rectangle(0,0,128,128,WHITE);
	st7735r_string(0,0,"x_send:",BLACK,WHITE);
	st7735r_string(0,20,"y_send:",BLACK,WHITE);
	st7735r_string(0,40,"z_send:",BLACK,WHITE);
	st7735r_string(0,60,"x_sum:",BLACK,WHITE);
	st7735r_string(0,80,"y_sum:",BLACK,WHITE);
	st7735r_string(0,100,"z_sum:",BLACK,WHITE);
	while(1)
	{
    //Function_Choose();
	}
}
