#include "Include.h"
float Current_angle = 0;
float Error_current = 0;   //error[0]
float Error_last = 0;      //error[-1]
float Error_prev = 0;      //error[-2]
float Angle_increment = 0;
int   Angle_Add_out = 0;
int   Angle_out = 0;

float Speed_kp = 6;
float Speed_ki = 2;
float Speed_kd = 1;

int Yaw_MPU = 0;  //���ڼ�¼�����������ǰ�Ƕ�

int Speed_Calculate(float Set_angle)        
{
  Current_angle = Yaw_MPU ; 
  Error_current = Set_angle - Current_angle;
	if(Error_current > 0)  //�����ת
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);//��λ���������ת
		Angle_Add_out = (int)(Speed_ki * Error_current)+ Speed_kp * (Error_current - Error_last) + Speed_kd * (Error_current - 2*Error_last + Error_prev);
    Angle_out += Angle_Add_out;
		Error_prev = Error_last;
		Error_last = Error_current;
	}
	else if(Error_current <= 0)  //�����ת
	{	
		Error_current=-Error_current;
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);//��λ���������ת
		Angle_Add_out = (int)(Speed_ki * Error_current)+ Speed_kp * (Error_current - Error_last) + Speed_kd * (Error_current - 2*Error_last + Error_prev);
		Angle_out += Angle_Add_out;
		Error_prev = Error_last;
		Error_last = Error_current;
	}
  if(Angle_out > 2000)//��������
    Angle_out = 2000;
  else if(Angle_out < 0)
    Angle_out = 0;
  return Angle_out;
}
