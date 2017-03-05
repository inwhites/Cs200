/******************************************************************
���ܣ���ʪ�ȼ��
�����ˣ�������
ʱ�䣺2015.4.23
�汾�ţ�1.0
*******************************************************************/
#include "Include.h"
void DHT11_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	  //��������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void DHT11_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	//��©���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void DHT11WriteStart()    //��DHT11д��һ����ȡ���ݵ�������
{
	DHT11_PortOUT();
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	Delay_ms(25);//���͵�ƽ����18ms
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	Delay_us(30);				
}

u8 DHT11ReadByte(void)  //��DHT11�ж�ȡ��һ���ֽ�  ����ֵΪ��ȡ��һ���ֽڵ����� 
{
	u8 temp=0,i,j=0;
	for(i=0;i<8;i++)
	{
		temp<<=1;
		while(0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));//�ȴ���ߵ�ƽ
		while(1 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))//����ߵ�ƽʱ��
		{
			Delay_us(1);
			j++;
		}
		if(j>=30)          //����30usȷ��Ϊ1
		{
			temp = temp|0x01;
			j=0;
		}
		j=0;		
	}
	return temp;
}

void DHT11Read(u8 *RH_temp,u8 *RL_temp,u8 *TH_temp,u8 *TL_temp,u8 *CK_temp)// ��DHT11�ж�ȡ����
{
	while(1)
	{
		DHT11WriteStart();//����ȡǰ���ź�
		DHT11_PortIN();//���ö˿�Ϊ����״̬
		if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))   
		{
			while(0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));//�͵�ƽ����Ӧ�źţ�80us
			while(1 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0));//��������80us�ĸߵ�ƽ����׼���ź�
			*RH_temp = DHT11ReadByte();//ʪ�ȸ�8λ
			*RL_temp = DHT11ReadByte();//ʪ�ȵ�8λ
			*TH_temp = DHT11ReadByte();//�¶ȸ�8λ
			*TL_temp = DHT11ReadByte();//�¶ȵ�8λ
			*CK_temp = DHT11ReadByte();//У���
			DHT11_PortOUT();
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			return;
		}
		Delay_ms(10);
	}
}

void DHT11_Data(void)   //��ȡ�¶�ʪ��ֵ
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


