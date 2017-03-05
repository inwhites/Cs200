/******************************************************************
功能：串口
连线方法：此部分定义两个串口 USART2 PA2 PA3 TX RX USART3 PD8 PD9 TX RX
功能详述：由于考虑到systick定时器在开启其他中断时延时的问题（待解决）
          此部分定义了两种传递char* 的方法 一种延时用systick 一种直接for
					此部分包含的函数可以传递 单字符 字符串 多个数字
制作人：朱麒文
时间：2015.4.23
版本号：1.0
*******************************************************************/
#include "Include.h"
void USART3_Config(long int Baud)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);  //TX PD8
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);  //RX PD9
  
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  USART_InitStructure.USART_BaudRate = Baud;   //波特率115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enble TX and RX
  USART_Init(USART3, &USART_InitStructure);
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE); 
  USART_Cmd(USART3, ENABLE); // enable USART3
}

void USART2_Config(long int Baud)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);//TX PA2
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);//RX PA3
	
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
  USART_InitStructure.USART_BaudRate = Baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enble TX and RX
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
  USART_Cmd(USART2, ENABLE); // enable USART2
}


void USART3_Puts_it(char * data)
{
	int i;	 
	for (i=0;data[i]!='\0';i++)
	{
		USART_SendData( USART3 , data[i]);
	  Delay_Us(300);
	}
}


void USART2_Puts(char * data)
{
	int i;	 	
	for (i=0;data[i]!='\0';i++)
	{
		USART_SendData( USART2 , data[i]);
	  Delay_us(100);
	}
}

void USART2_Puts_it(char * data)
{
	int i;	 	
	for (i=0;data[i]!='\0';i++)
	{
		USART_SendData( USART2 , data[i]);
	  Delay_Us(100);
	}
}

void USART2_Num(int data)
{
	char str[5];
	sprintf(str,"%d",data);
	USART2_Puts(str);
}


void USART2_Num_it(int data)
{
	char str[5];
	sprintf(str,"%d",data);
	USART2_Puts_it(str);
}

void USART3_Num_it(int data)
{
	char str[5];
	sprintf(str,"%d",data);
	USART3_Puts_it(str);
}

void USART1_Config(long int Baud)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //TX PA9
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  //RX PA10
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  USART_InitStructure.USART_BaudRate = Baud;   //波特率115200
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enble TX and RX
  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); 
  USART_Cmd(USART1, ENABLE); // enable USART3
}

void USART3_Puts(char * data)
{
	int i;	 
	for (i=0;data[i]!='\0';i++)
	{
		USART_SendData( USART3 , data[i]);
	  Delay_Us(600);
	}
}

void USART3_Num(int data)
{
	char str[5];
	str[0]=data/1000+48;
	data=data%1000;
	str[1]=(data/100+48);
	data=data%100;
	str[2]=(data/10+48);
	data=data%10;
	str[3]=(data+48);
	str[4]='\0';
	USART3_Puts_it(str);
}

void USART1_Puts(char * data)
{
	int i;	 
	for (i=0;data[i]!='\0';i++)
	{
		USART_SendData( USART1 , data[i]);
	  Delay_Us(300);
	}
}

void USART1_Num(int data)
{
	char str[5];
	str[0]=data/1000+48;
	data=data%1000;
	str[1]=(data/100+48);
	data=data%100;
	str[2]=(data/10+48);
	data=data%10;
	str[3]=(data+48);
	str[4]='\0';
	USART1_Puts(str);
}