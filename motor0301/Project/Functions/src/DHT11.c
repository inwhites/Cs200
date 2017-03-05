/******************************************************************
功能：温湿度检测
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
void DHT11_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	  //浮空输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void DHT11_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	//开漏输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void DHT11WriteStart()    //向DHT11写入一个读取数据的引导码
{
	DHT11_PortOUT();
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	Delay_ms(25);//拉低电平至少18ms
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	Delay_us(30);				
}

u8 DHT11ReadByte(void)  //从DHT11中读取到一个字节  返回值为读取到一个字节的数据 
{
	u8 temp=0,i,j=0;
	for(i=0;i<8;i++)
	{
		temp<<=1;
		while(0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));//等待变高电平
		while(1 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))//计算高电平时长
		{
			Delay_us(1);
			j++;
		}
		if(j>=30)          //超过30us确认为1
		{
			temp = temp|0x01;
			j=0;
		}
		j=0;		
	}
	return temp;
}

void DHT11Read(u8 *RH_temp,u8 *RL_temp,u8 *TH_temp,u8 *TL_temp,u8 *CK_temp)// 从DHT11中读取数据
{
	while(1)
	{
		DHT11WriteStart();//给读取前导信号
		DHT11_PortIN();//设置端口为输入状态
		if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))   
		{
			while(0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));//低电平的响应信号，80us
			while(1 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));//紧接着是80us的高电平数据准备信号
			*RH_temp = DHT11ReadByte();//湿度高8位
			*RL_temp = DHT11ReadByte();//湿度低8位
			*TH_temp = DHT11ReadByte();//温度高8位
			*TL_temp = DHT11ReadByte();//温度低8位
			*CK_temp = DHT11ReadByte();//校验和
			DHT11_PortOUT();
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			return;
		}
		Delay_ms(10);
	}
}

void DHT11_Data(void)   //读取温度湿度值
{
	u8 TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
	DHT11Read(&RH_temp,&RL_temp,&TH_temp,&TL_temp,&CK_temp);
	USART3_Num(TH_temp);
//	LCD_Clear(LCD_COLOR_YELLOW);
//	Display_Num_Int(TH_temp,LCD_LINE_5,150);
//	Display_Point_float();
//	Display_Num_Int(TL_temp,LCD_LINE_5,195);
//	Display_Num_Int(RH_temp,LCD_LINE_6,150);
//	Display_Point_float2();
//	Display_Num_Int(RL_temp,LCD_LINE_6,195);
}


