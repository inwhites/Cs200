#include "Include.h"
int Kp = 0;
int Kd = 0;
int Error_Current = 0;//�ò������ݴ������������
int Error_Last = 0;
int Steering_Engine_Out = 0;
int Steering_Engine_Add_Out = 0;
int Steering_Engine(void)
{
	//�˴�Ӧ����Ӽ���Error_Current���� ���ݴ������ɼ��������ݼ���
	Kp = (Error_Current * Error_Current)/10+20;  //10 20 ������Ҫ���
  Steering_Engine_Add_Out = (int)(Kp * Error_Current + Kd * (Error_Current - Error_Last));
  Steering_Engine_Out = Steering_Engine_Add_Out + 8000;//8000����ֵ
  Error_Last = Error_Current; 
	return Steering_Engine_Out;
}
