#include "distance.h"


Distance_TypeDef Distance_Data;

unsigned char Get_Distance(void)
{
	return Distance;
}

unsigned char Ger_Distance_Offset(unsigned char Distance)
{
	Distance_Data.Distance_length = Get_Distance();
	return Distance_Data.Distance_offset = Distance_Data.Distance_length - Distance ;
}

PID_TypeDef Distance_Pid;/*!pid�㷨!���õ��Ĳ�������*/
p_i_d_Value_TypeDef PID_Value_Distance[3] = {{ 5, 0.0, 0 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�
														{ 0.0, 0.0, 0.0 },/*���������֣�΢��3ֵ��С�ṹ��*/
														{ 0.0, 0.0, 0.0 }};

void Track_Direction_Distance_Control(float speed)
{
	Get_Grayscale_Offset();
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 300);
	
	Ger_Distance_Offset(20);
	Pid_control(&Distance_Pid, Distance_Data.Distance_offset, 0);
	Pid_OutLimit(&Distance_Pid, 50);
	
	L1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue + Distance_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue + Distance_Pid.ActualValue;
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}
