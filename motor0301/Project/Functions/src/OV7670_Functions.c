#include "Include.h"
#include "FIFO_OV7670.h"
#include "OV7670.h"
__IO uint8_t OV7670_VSYNC=3;
extern int flag_choose;
void Display_OV7670(void)
{
	FIFO_PortConfig();	
	OV7670_Init();
	OV7670_VSYNC_Config();
	Delay_us(2000);
  OV7670_VSYNC = 0;
	while(1)
	{
		One_Motor_Run(1);
		if(OV7670_VSYNC==2)  
    {			
			Delay_us(10);	
		  FIFO_Prepare();
			if(flag_choose == 0)
				Circle_Center();
			else if(flag_choose == 1)
				Find_Centerline();		
			OV7670_VSYNC = 0;
     }
	 }
}