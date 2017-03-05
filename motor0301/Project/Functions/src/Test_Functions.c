#include "Include.h"
int YYH_mode = 0;
int mode_control = 0;
int tp_up = 0;
int tp_down = 0;
int first_tp = 0;
int key_Choose_y = 0;
int flag_keydown = 0;
int yes_count = 0;
int M1_lr = 0;
int M2_lr = 0;
int M3_lr = 0;
u32 M1_num = 0;
u32 M2_num = 0;
u32 M3_num = 0;
int i_1 = 0;

extern u32 sum_x;
extern u32 sum_y;
extern u32 sum_z;
extern u32 x;
extern u32 y;
extern u32 z;
extern int x_tp;
extern int y_tp;
extern int z_tp;
int i = 0;

void YYH_Display(void)
{
	int i;
	st7735r_rectangle(0,0,128,128,WHITE);
	for(i= 0;i<128;i++)
	{
		st7735r_piont(i,64,BLACK);
	}
	st7735r_string(10,26,"Manual Control",BLACK,WHITE);
	st7735r_string(17,78,"Auto Control",BLACK,WHITE);
}

void YYH_Function_Choose_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

void YYH_motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

void YYH_motor(void)
{
		if(x_tp == 1)//发送‘+’
		{
			for(int n = 0;n<=x;n++)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_8);
				GPIO_SetBits(GPIOD,GPIO_Pin_9);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_8);
				GPIO_ResetBits(GPIOD,GPIO_Pin_9);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_10);
				GPIO_ResetBits(GPIOD,GPIO_Pin_11);
			}
		}
		if(x_tp == 2)//发送‘-’
		{
			for(int n = 0;n<=x;n++)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_8);
				GPIO_SetBits(GPIOD,GPIO_Pin_9);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_8);
				GPIO_ResetBits(GPIOD,GPIO_Pin_9);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_11);
				GPIO_ResetBits(GPIOD,GPIO_Pin_10);
			}
		}
		if(y_tp == 1)//发送‘+’
		{
			for(int n = 0;n<=y;n++)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_12);
				GPIO_SetBits(GPIOD,GPIO_Pin_13);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_12);
				GPIO_ResetBits(GPIOD,GPIO_Pin_13);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_14);
				GPIO_ResetBits(GPIOD,GPIO_Pin_15);
			}
		}
		if(y_tp == 2)//发送‘-’
		{
			for(int n = 0;n<=y;n++)
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_12);
				GPIO_SetBits(GPIOD,GPIO_Pin_13);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_12);
				GPIO_ResetBits(GPIOD,GPIO_Pin_13);
				Delay_Us(104);
				GPIO_SetBits(GPIOD,GPIO_Pin_15);
				GPIO_ResetBits(GPIOD,GPIO_Pin_14);
			}
		}
		if(z_tp == 1)//发送‘+’
		{
			for(int n = 0;n<=z;n++)
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_6);
				GPIO_SetBits(GPIOC,GPIO_Pin_7);
				Delay_Us(104);
				GPIO_SetBits(GPIOC,GPIO_Pin_6);
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				Delay_Us(104);
				GPIO_SetBits(GPIOC,GPIO_Pin_8);
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
			}
		}
		if(z_tp == 2)//发送‘-’
		{
			for(int n = 0;n<=z;n++)
			{
				GPIO_ResetBits(GPIOC,GPIO_Pin_6);
				GPIO_SetBits(GPIOC,GPIO_Pin_7);
				Delay_Us(104);
				GPIO_SetBits(GPIOC,GPIO_Pin_6);
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				Delay_Us(104);
				GPIO_SetBits(GPIOC,GPIO_Pin_9);
				GPIO_ResetBits(GPIOC,GPIO_Pin_8);
			}
		}
}

void Function_Choose(void) //右 左 上 下 中 PE0-4
{
	int key1_value = 0;

	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
			key1_value = 1;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == 0)
			key1_value = 2;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
			key1_value = 3;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
			key1_value = 4;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
			key1_value = 5;
			flag_keydown = 1;
		}
	}
	
	if(key1_value == 3)
	{
		key_Choose_y-=64;
		if(key_Choose_y < 0)
			key_Choose_y = 0;
	}
	
	if(key1_value == 4)
	{
		key_Choose_y+=64;
		if(key_Choose_y >64)
			key_Choose_y = 64;
	}	
	if(key_Choose_y == 0 && flag_keydown == 1)
	{
		YYH_Display();
		st7735r_rectangle(0,0,128,64,YELLOW);
		st7735r_string(10,26,"Manual Control",BLACK,YELLOW);
		mode_control = 1;
	}
	else if(key_Choose_y == 64 && flag_keydown == 1)
	{
		YYH_Display();
		st7735r_rectangle(0,65,128,128,YELLOW);
		st7735r_string(17,78,"Auto Control",BLACK,YELLOW);
		mode_control = 2;
	}
	
	if(key1_value == 5 && mode_control == 1)
	{
		int Key_value = 0;
		first_tp = 0;
		st7735r_rectangle(0,0,128,128,WHITE);
		st7735r_string(10,26,"Manual Control",BLACK,WHITE);
		while(1)
		{
			if(first_tp == 0)
			{		
				Key_value = Key_Display();
			}
			while(Key_value == 12 || tp_up == 1)
			{
				first_tp = 1;
				tp_up = 1;
				Key_value = Key_Display();
				if(Key_value == 13)
					tp_up = 2;
				GPIO_ResetBits(GPIOD,GPIO_Pin_8);
				GPIO_SetBits(GPIOD,GPIO_Pin_9);
				Delay_us(6);
				GPIO_SetBits(GPIOD,GPIO_Pin_8);
				GPIO_ResetBits(GPIOD,GPIO_Pin_9);
				Delay_us(1);
				GPIO_SetBits(GPIOD,GPIO_Pin_0);
				GPIO_ResetBits(GPIOD,GPIO_Pin_1);
			}
			while(Key_value == 13 || tp_down == 1)
			{
				first_tp = 1;
				tp_down = 1;
				Key_value = Key_Display();
				if(Key_value == 12)
					tp_down = 2;
				GPIO_ResetBits(GPIOD,GPIO_Pin_8);
				GPIO_SetBits(GPIOD,GPIO_Pin_9);
				Delay_us(6);
				GPIO_SetBits(GPIOD,GPIO_Pin_8);
				GPIO_ResetBits(GPIOD,GPIO_Pin_9);
				Delay_us(1);
				GPIO_SetBits(GPIOD,GPIO_Pin_1);
				GPIO_ResetBits(GPIOD,GPIO_Pin_0);
			}
		}
	}
	if(key1_value == 5 && mode_control == 2)
	{
		int Key2_value = 0;
		int j = 0;
		st7735r_rectangle(0,0,128,128,WHITE);
		st7735r_string(10,5,"M1_lr:",BLACK,WHITE);
		st7735r_string(10,25,"M1_pu:",BLACK,WHITE);
		st7735r_string(10,45,"M2_lr:",BLACK,WHITE);
		st7735r_string(10,65,"M2_pu:",BLACK,WHITE);
		st7735r_string(10,85,"M3_lr:",BLACK,WHITE);
		st7735r_string(10,105,"M3_pu:",BLACK,WHITE);
		while(1)
		{		
			if(yes_count == 0)
			{
				while(Key2_value != 12)
				{
					Key2_value = Key_Display();
					if(Key2_value >= 0 && Key2_value <= 9)
					{
						if(j == 0)
						{
							M1_lr = Key2_value;
							st7735r_num(60,5,Key2_value,BLACK,WHITE);
						}
						j += 1;
					}
					if(Key2_value == 13)
					{
						j = 0;
						M1_lr = 0;
						st7735r_string(60,5,"         ",BLACK,WHITE);
					}
				}
				if(Key2_value == 12)
				{
					yes_count = 1;
					j = 0;
					Key2_value = 0;
					//st7735r_num(60,5,M1_lr,BLACK,WHITE);
				}
			}
			if(yes_count == 1)
			{
				while(Key2_value != 12)
				{
					Key2_value = Key_Display();
					if(Key2_value >= 0 && Key2_value <= 9)
					{
						M1_num = M1_num * 10 + Key2_value;
						st7735r_num(60+7*j,25,Key2_value,BLACK,WHITE);	
						j += 1;
					}
					if(Key2_value == 13)
					{
						j = 0;
						M1_num = 0;
						st7735r_string(60,25,"         ",BLACK,WHITE);
					}
				}
				if(Key2_value == 12)
				{
					yes_count = 2;
					j = 0;
					Key2_value = 0;
					//st7735r_num(60,25,M1_num,BLACK,WHITE);	
				}
			}
			if(yes_count == 2)
			{
				while(Key2_value != 12)
				{
					Key2_value = Key_Display();
					if(Key2_value >= 0 && Key2_value <= 9)
					{
						if(j == 0)
						{
							M2_lr = Key2_value;
							st7735r_num(60,45,Key2_value,BLACK,WHITE);
						}
						j += 1;
					}
					if(Key2_value == 13)
					{
						j = 0;
						M2_lr = 0;
						st7735r_string(60,45,"         ",BLACK,WHITE);
					}
				}
				if(Key2_value == 12)
				{
					yes_count = 3;
					j = 0;
					Key2_value = 0;
					//st7735r_num(60,45,M2_lr,BLACK,WHITE);
				}
			}
			if(yes_count == 3)
			{
				while(Key2_value != 12)
				{
					Key2_value = Key_Display();
					if(Key2_value >= 0 && Key2_value <= 9)
					{
						M2_num = M2_num * 10 + Key2_value;
						st7735r_num(60+7*j,65,Key2_value,BLACK,WHITE);	
						j += 1;
					}
					if(Key2_value == 13)
					{
						j = 0;
						M2_num = 0;
						st7735r_string(60,65,"         ",BLACK,WHITE);
					}
				}
				if(Key2_value == 12)
				{
					yes_count = 4;
					j = 0;
					Key2_value = 0;
					//st7735r_num(60,65,M2_num,BLACK,WHITE);	
				}
			}
			if(yes_count == 4)
			{
				while(Key2_value != 12)
				{
					Key2_value = Key_Display();
					if(Key2_value >= 0 && Key2_value <= 9)
					{
						if(j == 0)
						{
							M3_lr = Key2_value;
							st7735r_num(60,85,Key2_value,BLACK,WHITE);
						}
						j += 1;
					}
					if(Key2_value == 13)
					{
						j = 0;
						M3_lr = 0;
						st7735r_string(60,85,"         ",BLACK,WHITE);
					}
				}
				if(Key2_value == 12)
				{
					yes_count = 5;
					j = 0;
					Key2_value = 0;
					//st7735r_num(60,85,M3_lr,BLACK,WHITE);
				}
			}
			if(yes_count == 5)
			{
				while(Key2_value != 12)
				{
					Key2_value = Key_Display();
					if(Key2_value >= 0 && Key2_value <= 9)
					{
						M3_num = M3_num * 10 + Key2_value;
						st7735r_num(60+7*j,105,Key2_value,BLACK,WHITE);	
						j += 1;
					}
					if(Key2_value == 13)
					{
						j = 0;
						M3_num = 0;
						st7735r_string(60,105,"         ",BLACK,WHITE);
					}
				}
				if(Key2_value == 12)
				{
					yes_count = 6;
					j = 0;
					Key2_value = 0;
					//st7735r_num(60,105,M3_num,BLACK,WHITE);	
				}
			}
			if(yes_count == 6)
			{
				yes_count = 7;
				if(M1_lr == 1)
				{
					for(i_1 = 0;i_1<= M1_num;i_1++)
					{
						GPIO_ResetBits(GPIOD,GPIO_Pin_8);
						GPIO_SetBits(GPIOD,GPIO_Pin_9);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_8);
						GPIO_ResetBits(GPIOD,GPIO_Pin_9);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_0);
						GPIO_ResetBits(GPIOD,GPIO_Pin_1);
					}
				}
				if(M1_lr == 2)
				{
					for(i_1 = 0;i_1<= M1_num;i_1++)
					{
						GPIO_ResetBits(GPIOD,GPIO_Pin_8);
						GPIO_SetBits(GPIOD,GPIO_Pin_9);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_8);
						GPIO_ResetBits(GPIOD,GPIO_Pin_9);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_1);
						GPIO_ResetBits(GPIOD,GPIO_Pin_0);
					}
				}
				if(M2_lr == 1)
				{			
					for(i_1 = 0;i_1<= M2_num;i_1++)
					{
						GPIO_ResetBits(GPIOD,GPIO_Pin_10);
						GPIO_SetBits(GPIOD,GPIO_Pin_11);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_10);
						GPIO_ResetBits(GPIOD,GPIO_Pin_11);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_2);
						GPIO_ResetBits(GPIOD,GPIO_Pin_3);
					}
				}
				if(M2_lr == 2)
				{
					for(i_1 = 0;i_1<= M2_num;i_1++)
					{
						GPIO_ResetBits(GPIOD,GPIO_Pin_10);
						GPIO_SetBits(GPIOD,GPIO_Pin_11);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_10);
						GPIO_ResetBits(GPIOD,GPIO_Pin_11);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_3);
						GPIO_ResetBits(GPIOD,GPIO_Pin_2);
					}
				}
				if(M3_lr == 1)
				{
					for(i_1 = 0;i_1<= M3_num;i_1++)
					{
						GPIO_ResetBits(GPIOD,GPIO_Pin_12);
						GPIO_SetBits(GPIOD,GPIO_Pin_13);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_12);
						GPIO_ResetBits(GPIOD,GPIO_Pin_13);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_4);
						GPIO_ResetBits(GPIOD,GPIO_Pin_5);
					}
				}
				if(M3_lr == 2)
				{
					for(i_1 = 0;i_1<= M3_num;i_1++)
					{
						GPIO_ResetBits(GPIOD,GPIO_Pin_12);
						GPIO_SetBits(GPIOD,GPIO_Pin_13);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_12);
						GPIO_ResetBits(GPIOD,GPIO_Pin_13);
						Delay_us(2);
						GPIO_SetBits(GPIOD,GPIO_Pin_5);
						GPIO_ResetBits(GPIOD,GPIO_Pin_4);
					}
				}
			}
		}
	}
	flag_keydown = 0;
}

void motor_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
}

void pulse_temp_clockwise1(void)
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	Delay_ms(2);
//	GPIO_SetBits(GPIOD,GPIO_Pin_8);
//	Delay_us(1);
//	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
//	Delay_us(1);
//	GPIO_SetBits(GPIOD,GPIO_Pin_8);
//	Delay_us(1);
//	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
//	Delay_us(1);
//	GPIO_SetBits(GPIOD,GPIO_Pin_8);
//	Delay_us(1);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	Delay_ms(2);
}

void pulse_temp1(void)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
	Delay_ms(2);
//	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
//	Delay_us(1);
//	GPIO_SetBits(GPIOD,GPIO_Pin_9);
//	Delay_us(1);
//	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
//	Delay_us(1);
//	GPIO_SetBits(GPIOD,GPIO_Pin_9);
//	Delay_us(1);
//	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
//	Delay_us(1);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
	Delay_ms(2);
}