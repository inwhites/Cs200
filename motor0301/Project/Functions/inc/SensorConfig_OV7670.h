#include "stm32f4xx.h"

typedef struct Register
{
	uint8_t Address;			       /*�Ĵ�����ַ*/
	uint8_t Value;		           /*�Ĵ���ֵ*/
}Register_Info;


extern Register_Info Sensor_Config[];
