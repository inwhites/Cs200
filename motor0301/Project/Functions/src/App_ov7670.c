#include "FIFO_OV7670.h"
#include "st7735r.h"
#include "COLOR.h"
#include "Include.h"
#include "math.h"
#include "usart.h"
#include "Delay.h"
int flag_choose = 0;
extern int RC_Data_YAW;
extern int US_100_HIGH;
/********************************************
            寻找地标中心点坐标
********************************************/				
uint16_t i,j;
uint16_t Y,Color,R,G,B;
int x_center = 0;
uint32_t temp_count = 0;
int y_center=0;
int y_count[320];
int y[320];
int temp_js;
int temp_ct=0;

int key_Choose_x = 0;
int key_Choose_y = 0;
int flag_keydown = 0;

int Color_flag = 0;
/*******************************************/

/********************************************
                   循迹
********************************************/	
uint32_t Center_line[320];  //用来记录每一行黑色中心点坐标
uint32_t Temp_Sum[320];
int Temp_min_x = 240;
int Temp_max_x = 0;
int y_count[320];
int x_max = 0;
int x_min = 0;
int y_max = 0;
int y_min = 0;
int Temp_min = 320;
int Temp_max = 0;
int flag1 = 0;
int flag2 = 0;
int Temp_begin = 0;
int Temp_end = 0;
float a = 0;
int avg_center_last = 0;
int flag_lr = 0;
int allwhite_line = 0;
int flag_yuan = 0;
int OK_count = 0;
int Hight_Input = 0;
int Time_Input = 0;
int Input[6];
int i_input = 0;
extern int Key_value;
extern int Num_Temp_mine;
int send_count = 0;
int no_white_begin = 0;
int no_white_end = 0;
int no_white_flag_begin = 0;
int no_white_flag_end = 0;
int avg_x_last = 0;
int yuan_count_x = 0;
int yuan_count_y = 0;
int yuanma = 0;
int c = 0;
int b = 0;
int flag_Rec = 0;
/*******************************************/


/********************************************
                题目
********************************************/	
int flag_Timu = 0;
/********************************************
                均值及方差
********************************************/	
uint32_t avg_center = 0;
uint32_t avg_center_x[240];
int x_count[240];
uint32_t avg_x = 0;
uint32_t x2 = 0;
int all_white_count = 0;
void Citie(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

/********************************************************
函数名：中值滤波
参数：中值
制作人：朱麒文
时间：2015.7.18
版本号：1.0
********************************************************/
int find_middle_value(int no_1,int no_2,int no_3,int no_4,int no_5)
{
  int temp;
  int num[5];
  int i,j;
  num[0] = no_1;
  num[1] = no_2;
  num[2] = no_3;
  num[3] = no_4;
  num[4] = no_5;
  for(i = 0;i<5;i++)
  {
    for(j = 0;j<4-i;j++)
    {
      if(num[j]>num[j+1])
      {
        temp = num[j];
        num[j] = num[j+1];
        num[j+1] = temp;
      }
    }
  }
  return num[2];
}

/********************************************************
函数名：计算均值及方差
参数：X2
制作人：朱麒文
时间：2015.7.18
版本号：1.0
********************************************************/
void X2_calculate(void)
{
	avg_center = 0;
	for(i = no_white_begin;i<no_white_end;i++)
	{
		avg_center += Center_line[i];
	}
	avg_center = avg_center/(no_white_end - no_white_begin);
	if(no_white_begin >= no_white_end)
		avg_center = 0;
//	for(i = 30;i<290;i++)
//	{
//		x2 += (avg_center - Center_line[i])*(avg_center - Center_line[i]);
//	}
//	x2 = x2/260;
}

/********************************************************
函数名：循迹
参数：Center_line
制作人：朱麒文
时间：2015.7.18
版本号：1.0
********************************************************/
void Find_Centerline(void)
{ 
	allwhite_line = 0;
	flag_yuan = 0;
	all_white_count = 0;
	flag1 = 0;
	flag2 = 0;
	Temp_min = 320;
	Temp_max = 0;
	Temp_min_x = 240;
	Temp_max_x = 0;
	Temp_begin = 0;
	Temp_end = 0;
	yuan_count_x = 0;
	yuan_count_y = 0;
	avg_x_last = avg_x;
	avg_x = 0;
	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			Color=0;
			FIFO_RCK_0;
			Color=(uint16_t)((FIFO_Data_GPIO_PORT->IDR<<8)&0xff00);
			FIFO_RCK_1;
			R=(Color&0xf800)>>8;
			FIFO_RCK_0;
			Color|=(uint16_t)(FIFO_Data_GPIO_PORT->IDR&0x00ff);
			G=(Color&0x07e0)>>3;
			B=(Color&0x001f)<<3;
			FIFO_RCK_1;
			
			Y=(31*R+59*G+11*B)/100;
			
			if(Y>100) 
			{
				Color=0xffff;
			}
			else 
			{
				Color=0;
				if(j != 319&&i!=239&&j!= 0&&i!=0)
				//if(i>=64 && i<176 && j>=96&& j<224)  //根据具体需要确定
				{
					Temp_Sum[j]+=i;
					y_count[j]++;
					avg_center_x[i]+=j;
					x_count[i]++;
				}
			}
			if(i>=64 && i<176 && j>=96&& j<224)      //95-223   63-175
				st7735r_piont((j-96),128-16-(i-64),Color);
		}
	}
	for(j=0;j<320;j++)
	{
		if(y_count[j]!=0)
		{
			Center_line[j]=Temp_Sum[j]/y_count[j];
		}
		else if(y_count[j] == 0)
		{
			allwhite_line++;
			Center_line[j] = 0;
		}
	}
	//滤波两次
	for(j = 0;j<320;j++)
	{
		Center_line[j] = find_middle_value(Center_line[j],Center_line[j+1],Center_line[j+2],Center_line[j+3],Center_line[j+4]);
	}
	for(j = 0;j<320;j++)
	{
		Center_line[j] = find_middle_value(Center_line[j],Center_line[j+1],Center_line[j+2],Center_line[j+3],Center_line[j+4]);
	}
	//寻找非白行
	no_white_begin = 30;
	no_white_end = 290;
	no_white_flag_begin = 0;
	no_white_flag_end = 0;
	for(j = 30;j<290;j++)
	{
		if(y_count[j] != 0 && no_white_flag_begin == 0)
		{
			no_white_begin = j;
			no_white_flag_begin = 1;
		}
	}
	for(j = 289;j>=30;j--)
	{
		if(y_count[j] != 0 && no_white_flag_end == 0)
		{
			no_white_end = j;
			no_white_flag_end = 1;
		}
	}
	/*************************************************/
	for(j = no_white_begin;j<no_white_end;j++)
	{	
		if(y_count[j] < Temp_min && y_count[j] > 5)
		{
			Temp_min = y_count[j];
			y_min = j;
		}
	}
	for(j = no_white_begin;j<no_white_end;j++)
	{	
		if(y_count[j] > Temp_max)
		{
			Temp_max = y_count[j];
			y_max = j;
		}
	}
	for(i = 2;i<238;i++)
	{
		if(x_count[i] < Temp_min_x && x_count[i] > 5)
		{
			Temp_min_x = x_count[i];
			x_min = i;
		}
	}
	for(i = 2;i<238;i++)
	{
		if(x_count[i] > Temp_max_x)
		{
			Temp_max_x = x_count[i];
			x_max = i;
		}
	}
	/*************************************************/
	if(x_max < 225)
	{
		for(i = x_max+5;i<230;i++)
		{
			if(x_count[i] > x_count[i+1])
				yuan_count_x++;
			if(x_count[i] < x_count[i+1])
				i = 230;
		}
	}
	if(x_max>15)
	{
		for(i = x_max-5;i>10;i--)
		{
			if(x_count[i] > x_count[i-1])
				yuan_count_x++;
			if(x_count[i] < x_count[i-1])
				i = 10;
		}
	}	
	if(y_max < 285)
	{
		for(i = y_max+5;i<290;i++)
		{
			if(y_count[i] > y_count[i+1])
				yuan_count_y++;
			if(y_count[i] < y_count[i+1])
				i = 290;
		}
	}
	if(y_max > 35)
	{
		for(i = y_max-5;i>30;i--)
		{
			if(y_count[i] > y_count[i-1])
				yuan_count_y++;
			if(y_count[i] < y_count[i-1])
				i = 30;
		}
	}
	for(i = 0;i<240;i++)
	{
		if(x_count[i] == 0)
		{
			avg_center_x[i] = 0;
			all_white_count++;
		}
		else if(x_count[i] > Temp_min+7)
		{
			avg_center_x[i] = avg_center_x[i]/x_count[i];
		}
		else
		{
			avg_center_x[i] = 0;
			all_white_count++;
		}
	}
	for(i = 1; i<239;i++)
	{
		avg_x += avg_center_x[i];
	}
	if(all_white_count>=238)
		avg_x = 0;
	else 
		avg_x = avg_x/(238-all_white_count);
	if((Temp_max - Temp_min) < 30)
		avg_x = avg_x_last;
	/*************************************************/
	for(j = no_white_begin;j<no_white_end;j++)
	{
		if(y_count[j] > (Temp_min+17) && flag1 == 0)
		{
			Temp_begin = j;
			flag1 = 1;
		}
	}
	for(j = no_white_end;j>=no_white_begin;j--)
	{
		if(y_count[j] > (Temp_min+17) && flag2 == 0)
		{
			Temp_end = j;
			flag2 = 1;
		}
	}
	if(Temp_end > Temp_begin)
		a = (float)((float)Center_line[Temp_end+9] - (float)Center_line[Temp_begin-9])/((float)Temp_end - (float)Temp_begin + 18);
	if(Temp_end > Temp_begin && (!(flag_yuan > 60 && (Temp_max - Temp_min) < 120 && (Temp_max - Temp_min) >= 60)))
	{
		for(j = (Temp_begin-8);j<(Temp_end+9);j++)
		{
			Center_line[j] = Center_line[Temp_begin-9]+a*(j-Temp_begin+9);///(Temp_end - Temp_begin+18);
		}
	}
	//再进行一次中值滤波
	for(j = no_white_begin;j<no_white_end;j++)
	{
		Center_line[j] = find_middle_value(Center_line[j],Center_line[j+1],Center_line[j+2],Center_line[j+3],Center_line[j+4]);
	}
	for(j = no_white_begin;j<no_white_end;j++)
	{
		if(y_count[j] > (Temp_max-Temp_min))
		{
			flag_yuan++;
		}
	}
	for(j = 0;j<320;j++)
	{
		Temp_Sum[j] = 0;
		y_count[j] = 0;
	}
	for(i = 0;i<240;i++)
	{
		avg_center_x[i] = 0;
		x_count[i] = 0;
	}
	for(j = 96;j<224;j++)
	{
		if(Center_line[j] >=64 && Center_line[j]<176)
		{
			st7735r_piont((j-96),128-16-(Center_line[j]-64),RED);
		}
	}
	X2_calculate();
	if(allwhite_line > 120)
	{
		if(avg_center_last < 119)
		{
			avg_center = 20;
		}
		if(avg_center_last > 119)
		{
			avg_center = 230;
		}
	}
		if(Temp_max_x - Temp_min_x > 0)
		c= Temp_max_x - Temp_min_x;
	else if(Temp_max_x - Temp_min_x<= 0)
		c = -(Temp_max_x - Temp_min_x);
	if(Temp_max - Temp_min > 0)
		b= Temp_max - Temp_min;
	else if(Temp_max - Temp_min<= 0)
		b = -(Temp_max - Temp_min);
	avg_center_last = avg_center;
	USART_SendData(USART1,'K');
	Delay_Us(300);
	USART1_Num(avg_center); //将中线发到飞控
	USART1_Num(avg_x);
	if(yuan_count_x >=52 && yuan_count_y>= 52) 
	{
		USART1_Num(2);  //is circle
		yuanma = 1;
	}
	else
	{
		USART1_Num(1);  //not circle
		yuanma = 0;
	}
	if(c > 70 && b > 70 && !yuanma && all_white_count >= 40 && allwhite_line >= 40) 
	{
		USART1_Num(2);
	}
	else 
	{
		USART1_Num(1);
	}
	if(flag_Timu == 1 && send_count == 0)
	{
		USART1_Num(1);
		send_count = 1;
	}
	else if(flag_Timu == 2 && send_count == 0)
	{
		USART1_Num(2);
		send_count = 1;
	}
	else if(flag_Timu == 3 && send_count == 0)
	{
		USART1_Num(3);
		send_count = 1;
	}
	else if(flag_Timu == 4 && send_count == 0)
	{
		USART1_Num(4);
		send_count = 1;
	}
	else if(flag_Timu == 5 && send_count == 0)
	{
		USART1_Num(5);
		send_count = 1;
	}
	else if(send_count == 1)
	{
		USART1_Num(9);
	}
	USART_SendData(USART1,'E');
	st7735r_rectangle(0,112,128,16,WHITE);
	st7735r_string(0,112,"avg:",BLUE,YELLOW);	
	st7735r_string(65,112,"x2:",BLUE,YELLOW);
	st7735r_num(33,112,c,BLUE,YELLOW);//flag_yuan
	st7735r_num(93,112,b,BLUE,YELLOW);//(Temp_max - Temp_min)
}

/********************************************************
函数名：寻找地标中心点坐标
参数：x_center y_center
制作人：朱麒文
时间：2015.7.12
版本号：1.0
********************************************************/
void Circle_Center(void)     
{
	x_center = 0;
	temp_count = 0;
	y_center=0;
	for(i=0;i<240;i++)
	{
		for(j=0;j<320;j++)
		{
			Color=0;
			FIFO_RCK_0;
			Color=(uint16_t)((FIFO_Data_GPIO_PORT->IDR<<8)&0xff00);
			FIFO_RCK_1;
			R=(Color&0xf800)>>8;
			FIFO_RCK_0;
			Color|=(uint16_t)(FIFO_Data_GPIO_PORT->IDR&0x00ff);
			G=(Color&0x07e0)>>3;
			B=(Color&0x001f)<<3;
			FIFO_RCK_1;
			
			Y=(31*R+59*G+11*B)/100;
			
			if(Y>130) 
			{
				if(Color_flag == 1)
				Color=0xffff;
			}
			else 
			{
				if(Color_flag == 1)
				Color=0;
				if(j != 319&&i!=239&&j!= 0&&i!=0)
				//if(i>=63 && i<175 && j>=95&& j<223)
				{
					temp_count+=j;
					temp_js++;
					y[j]+=i;
					y_count[j]++;
				}
			}
			if(i>=63 && i<175 && j>=95&& j<223)      //95-223   63-175
				st7735r_piont((j-95),128-16-(i-63),Color);
		}
		if(temp_js != 0)
		{
			x_center += temp_count/temp_js;
			temp_ct++;
		}
			temp_count = 0;
			temp_js = 0;
	}
	if(temp_ct==0)
	{
		x_center=0;
	}
	else
	{
		x_center = x_center/temp_ct;
		temp_ct=0;
	}
	for(j=0;j<320;j++)
	{
		if(y_count[j]!=0)
		{
			y_center+=y[j]/y_count[j];
			temp_ct++;
		}
		y[j]=0;
		y_count[j]=0;
	}
	if(temp_ct==0)
	{
		y_center=0;
	}
	else
	{
		y_center=y_center/temp_ct;
		temp_ct=0;
	}
	st7735r_rectangle(0,112,128,16,WHITE);
	st7735r_string(13,112,"X:",BLUE,YELLOW);	
	st7735r_string(73,112,"Y:",BLUE,YELLOW);
	st7735r_num(33,112,x_center,BLUE,YELLOW);
	st7735r_num(93,112,y_center,BLUE,YELLOW);
}

/********************************************************
函数名：五轴按键测试
制作人：朱麒文
时间：2015.7.12
版本号：1.0
********************************************************/
void Draw_Function(void)
{
	int i;
	st7735r_init();
	for(i= 0;i<128;i++)
	{
		st7735r_piont(64,i,BLACK);
	}
	for(i= 0;i<128;i++)
	{
		st7735r_piont(i,32,BLACK);
	}
	for(i= 0;i<128;i++)
	{
		st7735r_piont(i,64,BLACK);
	}
	for(i= 0;i<128;i++)
	{
		st7735r_piont(i,96,BLACK);
	}
	st7735r_string(0,8," W_B_C",BLUE,WHITE);
	st7735r_string(0,40," COLOR",BLUE,WHITE);
	st7735r_string(0,72," W_B_L",BLUE,WHITE);
	st7735r_string(0,104," TiMu1",BLUE,WHITE);
	st7735r_string(65,8," TiMu2",BLUE,WHITE);
	st7735r_string(65,40," TiMu3",BLUE,WHITE);
	st7735r_string(65,72," TiMu4",BLUE,WHITE);
	st7735r_string(65,104,"  Key",BLUE,WHITE);
	
}
void Function_Choose_Init(void)
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

void Function_Choose(void) //右 左 上 下 中 PE0-4
{
	int key_value = 0;
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) == 0)
			key_value = 1;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1) == 0)
			key_value = 2;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0)
			key_value = 3;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0)
			key_value = 4;
			flag_keydown = 1;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
	{
		Delay_ms(5);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
		{
			while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
			key_value = 5;
		}
	}
	if(key_value == 1)
	{
		key_Choose_x+=65;
		if(key_Choose_x > 100)
			key_Choose_x = 65;
	}
	if(key_value == 2)
	{
		key_Choose_x-=65;
		if(key_Choose_x < 0)
			key_Choose_x = 0;
	}
	if(key_value == 3)
	{
		key_Choose_y-=32;
		if(key_Choose_y < 0)
			key_Choose_y = 0;
	}
	if(key_value == 4)
	{
		key_Choose_y+=32;
		if(key_Choose_y >100)
			key_Choose_y = 96;
	}	
	if(key_Choose_x == 0 && key_Choose_y == 0 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(0,0,64,32,YELLOW);
		st7735r_string(0,8," B_W_C",BLUE,YELLOW);
	}
	else if(key_Choose_x == 65 && key_Choose_y == 0 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(65,0,64,31,YELLOW);
		st7735r_string(65,8," TiMu2",BLUE,YELLOW);
	}
	else if(key_Choose_x == 0 && key_Choose_y == 32 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(0,33,64,31,YELLOW);
		st7735r_string(0,40," COLOR",BLUE,YELLOW);	
	}	
	else if(key_Choose_x == 65 && key_Choose_y == 32 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(65,33,64,31,YELLOW);
		st7735r_string(65,40," TiMu3",BLUE,YELLOW);	
	}	
	else if(key_Choose_x == 0 && key_Choose_y == 64 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(0,65,64,31,YELLOW);
		st7735r_string(0,72," B_W_L",BLUE,YELLOW);	
	}	
	else if(key_Choose_x == 65 && key_Choose_y == 64 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(65,65,64,31,YELLOW);
		st7735r_string(65,72," TiMu4",BLUE,YELLOW);	
	}	
	else if(key_Choose_x == 0 && key_Choose_y == 96 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(0,97,64,31,YELLOW);
		st7735r_string(0,104," TiMu1",BLUE,YELLOW);	
	}	
	else if(key_Choose_x == 65 && key_Choose_y == 96 && flag_keydown == 1)
	{
		Draw_Function();
		st7735r_rectangle(65,97,64,31,YELLOW);
		st7735r_string(65,104,"  Key",BLUE,YELLOW);	
	}	
	if(key_value == 5 && key_Choose_x == 0 && key_Choose_y == 0)
	{
		flag_choose = 0;
		Color_flag = 1;
		OV7670_LCD();
	}
	if(key_value == 5 && key_Choose_x == 0 && key_Choose_y == 32)
	{
		flag_choose = 0;
		Color_flag = 0;
		OV7670_LCD();
	}
	if(key_value == 5 && key_Choose_x == 0 && key_Choose_y == 64)
	{
		flag_Timu = 0;
		flag_choose = 1;
		OV7670_LCD();
	}
	if(key_value == 5 && key_Choose_x == 0 && key_Choose_y == 96)//第一题
	{
		flag_Timu = 1;
		flag_choose = 1;
		OV7670_LCD();
	}
	if(key_value == 5 && key_Choose_x == 65 && key_Choose_y == 0)//第二题
	{
		flag_Timu = 2;
		flag_choose = 1;
		OV7670_LCD();
	}
	if(key_value == 5 && key_Choose_x == 65 && key_Choose_y == 32)//第三题
	{
		flag_Timu = 3;
		flag_choose = 1;
		OV7670_LCD();
	}
	if(key_value == 5 && key_Choose_x == 65 && key_Choose_y == 64)//第四题
	{
		flag_Timu = 4;
		flag_choose = 1;
		OV7670_LCD();
	}
	if(key_value == 5 && key_Choose_x == 65 && key_Choose_y == 96)//键盘输入
	{
		flag_Timu = 5;
		flag_choose = 1;
		OV7670_LCD();
	}
	else if(key_value == 5)
	{
		st7735r_clear(RED);
	}
	flag_keydown = 0;
}
