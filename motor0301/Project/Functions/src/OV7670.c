#include "OV7670.h"

uint8_t Sensor_IDCode = 0;
#define OV7670_REG_NUM 114
uint8_t EnglishStr1[] = "  write error  ";
uint8_t EnglishStr2[] = "  write success  ";
uint8_t EnglishStr3[] = "  config success  ";


void delay(__IO uint16_t T)
{
	__IO uint16_t i,j;
	
	for(i=5*T;i>0;i--) for(j=42000;j>0;j--);
		
}

void VSYNC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_AHB1PeriphClockCmd(VSYNC_CLK, ENABLE);	
	
	  GPIO_InitStructure.GPIO_Pin=VSYNC_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN; 
	  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(VSYNC_GPIO_PORT, &GPIO_InitStructure);
}

void VSYNC_EXTI_Config(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
	
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(VSYNC_EXTI_PORT_SOURCE, VSYNC_EXTI_PIN_SOURCE);

  /* Configure EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = VSYNC_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = VSYNC_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void OV7670_VSYNC_Config(void)
{
	VSYNC_GPIO_Config();
  VSYNC_EXTI_Config();
	
}

ErrorStatus OV7670_Init(void)
{   
		uint16_t i=0;
	  LCD_SetFont(&Font12x24);
	  SCCB_GPIO_Configuration();
	  delay(10);

		  if( 0 == SCCB_WriteByte ( OV7670_COM7, SCCB_REG_RESET ) ) /*¸´Î»sensor */
    {
        delay(10);
			  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)EnglishStr1);
        return ERROR ;
    }
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)EnglishStr2);
		delay(10);
   
        for( i = 0 ; i < OV7670_REG_NUM ; i++ )
        {
            if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
            {            
							return ERROR;
            }
        }
			LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)EnglishStr3);
    return SUCCESS;
}
	
	
	
	
	


