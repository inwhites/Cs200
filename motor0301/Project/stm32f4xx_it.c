/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Template/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "Include.h"
#include "FIFO_OV7670.h"
extern uint32_t TimingDelay;
unsigned char Data;
u32 x_data = 0;
u32 y_data = 0;
u32 z_data = 0;
int begin_temp = 0;
int x_temp = 0;
int y_temp = 0;
int z_temp = 0;
int finish_temp = 0;
int x_first = 0;
int y_first = 0;
int z_first = 0;
int x_tp = 0;
int y_tp = 0;
int z_tp = 0;
u32 x = 0;
u32 y = 0;
u32 z = 0;
u32 sum_x = 0;
u32 sum_y = 0;
u32 sum_z = 0;

int flag = 0;
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{

}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{

  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{

  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{

  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{

  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay--;
}

void USART1_IRQHandler()
{
 if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
  {
		
		Data=USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_SendData(USART1,Data);
		
		if(Data == 'B'){
			flag = 0;
			x = 0;
			y= 0;
			z = 0;
			x_data = 0;
			y_data = 0;
			z_data = 0;
			x_tp = 0; 
			y_tp = 0;
			z_tp = 0;
			return;
		
		}
		
		if(Data == 'X')
			flag = 1;
		else if(Data == 'Y')
			flag = 2;
		else if(Data == 'Z')
			flag = 3;
		
		
		
		if(flag == 1){
			if(Data == '+'){
				x_tp = 1;
			}else if(Data == '-')
			  x_tp = 2;
			else if(Data >= '0' && Data <= '9'){
				x_data = x_data*10 +Data-48;
			}
		}
			
		
		if(flag == 2){
			if(Data == '+'){
				y_tp = 1;
			}else if(Data == '-')
			  y_tp = 2;
				else if(Data >= '0' && Data <= '9') {
					y_data = y_data*10 +Data-48;
				}
		}
		
		if(flag == 3){
			if(Data == '+'){
				z_tp = 1;
			}else if(Data == '-')
			  z_tp = 2;
			else if(Data >= '0' && Data <= '9') {
					z_data = z_data*10 +Data-48;
			}
		}
		
		
				
		
		
		
		
		

		if(Data == 'F')
		{
			x = x_data;
			y = y_data;
			z = z_data;
			YYH_motor();

		
			if(x_tp == 1)
			{
				sum_x += x;
				st7735r_string(60,0,"+",BLACK,WHITE);
				st7735r_num(70,0,x,BLACK,WHITE);
			}
			if(x_tp == 2)
			{
				sum_x -= x;
				st7735r_string(60,0,"-",BLACK,WHITE);
				st7735r_num(70,0,x,BLACK,WHITE);
			}
			if(y_tp == 1)
			{
				sum_y += y;
				st7735r_string(60,20,"+",BLACK,WHITE);
				st7735r_num(70,20,y,BLACK,WHITE);
			}
			if(y_tp == 2)
			{
				sum_y -= y;
				st7735r_string(60,20,"-",BLACK,WHITE);
				st7735r_num(70,20,y,BLACK,WHITE);
			}
			if(z_tp == 1)
			{
				sum_z += z;
				st7735r_string(60,40,"+",BLACK,WHITE);
				st7735r_num(70,40,z,BLACK,WHITE);
			}
			if(z_tp == 2)
			{
				sum_z -= z;
				st7735r_string(60,40,"-",BLACK,WHITE);
				st7735r_num(70,40,z,BLACK,WHITE);
			}
			st7735r_num(70,60,sum_x,BLACK,WHITE);
			st7735r_num(70,80,sum_y,BLACK,WHITE);
			st7735r_num(70,100,sum_z,BLACK,WHITE);
			
				
			x_data = 0;
			y_data = 0;
			z_data = 0;
			x=0;
			y=0;
			z=0;
			
			x_tp = 0;
			y_tp = 0;
			z_tp = 0;
			return;
		
		}
		
	}   



}






void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))
		{
		}
	}
}  //外部中断

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //进入中断
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
	}
}

void EXTI15_10_IRQHandler(void)
{
	 if(EXTI_GetITStatus(EXTI_Line12) != RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line12);
  }	
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
