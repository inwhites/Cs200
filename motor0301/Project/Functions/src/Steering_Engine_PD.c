#include "Include.h"
int Kp = 0;
int Kd = 0;
int Error_Current = 0;//该参数根据传感器计算出来
int Error_Last = 0;
int Steering_Engine_Out = 0;
int Steering_Engine_Add_Out = 0;
int Steering_Engine(void)
{
	//此处应该添加计算Error_Current部分 根据传感器采集到的数据计算
	Kp = (Error_Current * Error_Current)/10+20;  //10 20 根据需要测得
  Steering_Engine_Add_Out = (int)(Kp * Error_Current + Kd * (Error_Current - Error_Last));
  Steering_Engine_Out = Steering_Engine_Add_Out + 8000;//8000中心值
  Error_Last = Error_Current; 
	return Steering_Engine_Out;
}
