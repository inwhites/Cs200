#include "stm32f4xx.h"

typedef struct Register
{
	uint8_t Address;			       /*¼Ä´æÆ÷µØÖ·*/
	uint8_t Value;		           /*¼Ä´æÆ÷Öµ*/
}Register_Info;


extern Register_Info Sensor_Config[];
