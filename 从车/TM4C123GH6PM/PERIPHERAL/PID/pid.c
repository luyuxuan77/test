#include "pid.h"

//定义位PID参数结构体数组
//定义数组的原因:为了好看与对称
PID_TypeDef Position_position_Pid[4];	//位置控制位置环(外环)4组数据，对应4个电机
PID_TypeDef	Position_speed_Pid[4];		//位置控制速度环(内环)4组数据，对应4个电机
PID_TypeDef Speed_Pid[4];				//速度控制4组数据，对应4个电机

//定义位置控制位置环p，i，d三值结构体变量数组
//PID_Value_Position[0]，0号电机位置控制位置环p，i，d三值储存
//PID_Value_Position[1]，1号电机位置控制位置环p，i，d三值储存
//PID_Value_Position[2]，2号电机位置控制位置环p，i，d三值储存
//PID_Value_Position[3]，3号电机位置控制位置环p，i，d三值储存
//与硬件电机对应，p，i，d三值大小与电机有关，此处定义该结构体数组有利于电机p，i，d三值储存
#if	POSITION_Position_PID_TYPE == 0	/*位置式P，I，D参数大小*/
p_i_d_Value_TypeDef PID_Value_Position_position[4] = {{ 0.48 , 0.0 , 0.1 },
													  { 0.48 , 0.0 , 0.1 },
													  { 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 }};	//位置控制位置环pid参数大小
#else								/*增量式P，I，D参数大小*/
p_i_d_Value_TypeDef PID_Value_Position_position[4] = {{ 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 }};	//位置控制位置环环pid参数大小
#endif
//定义位置控制速度环p，i，d三值结构体变量数组
//PID_Value_Position_speed[0]，0号电机位置控制速度环p，i，d三值储存
//PID_Value_Position_speed[1]，1号电机位置控制速度环p，i，d三值储存
//PID_Value_Position_speed[2]，2号电机位置控制速度环p，i，d三值储存
//PID_Value_Position_speed[3]，3号电机位置控制速度环p，i，d三值储存
//与硬件电机对应，p，i，d三值大小与电机有关，此处定义该结构体数组有利于电机p，i，d三值储存
#if	POSITION_Speed_PID_TYPE == 0	/*位置式P，I，D参数大小*/
p_i_d_Value_TypeDef PID_Value_Position_speed[4] = {{ 17.0725 , 10.0081 , 0.0 },
												   { 17.0725 , 10.0081 , 0.0 },
												   { 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 }};	//位置控制速度环pid参数大小
#else								/*增量式P，I，D参数大小*/
p_i_d_Value_TypeDef PID_Value_Position_speed[4] = {{ 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 }};	//位置控制速度环pid参数大小
#endif


//定义速度控制p，i，d三值结构体变量数组
//PID_Value_Speed[0]，0号电机速度控制p，i，d三值储存
//PID_Value_Speed[1]，1号电机速度控制p，i，d三值储存
//PID_Value_Speed[2]，2号电机速度控制p，i，d三值储存
//PID_Value_Speed[3]，3号电机速度控制p，i，d三值储存
//与硬件电机对应，p，i，d三值大小与电机有关，此处定义该结构体数组有利于电机p，i，d三值储存
#if	SPEED_PID_TYPE == 0		/*位置式P，I，D参数大小*/
p_i_d_Value_TypeDef PID_Value_Speed[4] = {{ 190 , 7.6 , 0.78 },
										  { 190 , 7.6 , 0.78 },
										  { 0 , 0 , 0 },
										  { 0 , 0 , 0 }}; 	//速度控制pid参数大小
#else						/*增量式P，I，D参数大小*/
p_i_d_Value_TypeDef PID_Value_Speed[4] = {{ 0.0 , 0.0 , 0.0 },
										  { 0.0 , 0.0 , 0.0 },
										  { 0.0 , 0.0 , 0.0 },
										  { 0.0 , 0.0 , 0.0 }};	//速度控制pid参数大小
#endif

/**************************************************************************
函数功能：PID初始化函数，将PID算法中用到的参数清零，p i d三值复位
入口参数：PID_TypeDef:要清零的PID参数集合，pid_value:p i d三值的复位值集合
返回  值：无
**************************************************************************/
void Pid_Init( PID_TypeDef* PID, p_i_d_Value_TypeDef* pid_Value )
{
	PID->SetPoint = 0.0;
	PID->ActualValue = 0.0;
	PID->SumError = 0.0;
	PID->Error = 0.0;
	PID->LastError = 0.0;
	PID->PrevError = 0.0;
	PID->Proportion = pid_Value->KP;
	PID->Integral = pid_Value->KI;
	PID->Derivative = pid_Value->KD;
}

/**************************************************************************
函数功能：PID闭环控制算法
入口参数：PID_TypeDef:PID参数集合，Feedback_value:反馈值/真实值，Mode:模式选择，0位置式，1增量式
返回  值：经PID算法计算后的输出值
**************************************************************************/
int32_t Pid_control( PID_TypeDef* PID, float Feedback_value, unsigned char Mode )
{  
//	printf("Feedback_value=%.2f\r\n",Feedback_value);

		if ( PID->SumError > 10000)
		{
			PID->SumError = 10000;
		}
		else if (PID->SumError < -10000)
		{
			PID->SumError = -10000;
		}
	PID->Error = PID->SetPoint - Feedback_value;
//	printf("PID->Error=%.2f\r\n",PID->Error);
	if (Mode == 0)
	{
		PID->SumError += PID->Error;
		PID->ActualValue = PID->Proportion * PID->Error
							+ PID->Integral * PID->SumError
							+ PID->Derivative * (PID->Error - PID->LastError);
		PID->LastError = PID->Error;
	}
	else
	{
		PID->ActualValue += PID->Proportion * (PID->Error - PID->LastError)
							+ PID->Integral * PID->Error
							+ PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError);
		PID->PrevError = PID->LastError;
		PID->LastError = PID->Error;
	}
//	printf("ActualValue=%.2f\r\n",PID->ActualValue);
	return PID->ActualValue;
}

/**************************************************************************
函数功能：PID输出幅值限幅
入口参数：PID_TypeDef:PID参数集合，limit:幅值大小(-limit~limit)
返回  值：经限制后的PID输出值
**************************************************************************/
int32_t Pid_OutLimit( PID_TypeDef* PID, int32_t limit)
{
	if ( PID->ActualValue > limit)
	{
		PID->ActualValue = limit;
	}
	else if (PID->ActualValue < -limit)
	{
		PID->ActualValue = -limit;
	}
	return PID->ActualValue;
}
