#include "car_control.h"
#include "encoder.h"
#include "pid.h"

//定义行车记录仪结构体变量，储存行车数据
Automobile_Data_Recorder_TypeDef Automobile_Data_Recorder = 
{
	Line_Walking_State,	//巡线状态
	{Special_1, Special_2},	//动作组
	0	//执行到第0个动作
};

/******************************************************************************
函数功能：特殊动作选择函数，依次执行动作组中的动作
入口参数：无
返回  值：无
*******************************************************************************/ 
void Specific_Action_Execute(void)
{
	switch(Automobile_Data_Recorder.Specific_Action_Group[Automobile_Data_Recorder.Specific_Action_Group_i])
	{
		case Stop:			Car_Stop();		break;
		case Advance:		Go_Straight();	break;
		case Left_Turn:		Left_turn();	break;
		case Right_Turn:	Right_turn();	break;
		case Turn_Round:	Turn_About();	break;
		case Park:			Car_Park();		break;
		case Special_1:		Car_Special1();	break;
		case Special_2:		Car_Special2();	break;
		default:	break;
	}
	if (Automobile_Data_Recorder.Specific_Action_Group_i < Specific_Action_Group_Amount - 1)	Automobile_Data_Recorder.Specific_Action_Group_i++;
}

/******************************************************************************
函数功能：特殊动作--直行
入口参数：无
返回  值：无
*******************************************************************************/ 
void Go_Straight(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 1000;
	R1_Position_position_Pid.SetPoint = old_r1_encoder + 1000;
}

/******************************************************************************
函数功能：特殊动作--左转
入口参数：无
返回  值：无
*******************************************************************************/ 
void Left_turn(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 500;
	R1_Position_position_Pid.SetPoint = old_r1_encoder + 2500;
}

/******************************************************************************
函数功能：特殊动作--右转
入口参数：无
返回  值：无
*******************************************************************************/ 
void Right_turn(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 2500;
	R1_Position_position_Pid.SetPoint = old_r1_encoder + 500;
}

/******************************************************************************
函数功能：特殊动作--掉头
入口参数：无
返回  值：无
*******************************************************************************/ 
void Turn_About(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 1500;
	R1_Position_position_Pid.SetPoint = old_r1_encoder - 1500;
}

/******************************************************************************
函数功能：特殊动作--停车
入口参数：无
返回  值：无
*******************************************************************************/ 
void Car_Park(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 500;
	R1_Position_position_Pid.SetPoint = old_r1_encoder + 500;
}

/******************************************************************************
函数功能：特殊动作--停车
入口参数：无
返回  值：无
*******************************************************************************/ 
void Car_Stop(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder;
	R1_Position_position_Pid.SetPoint = old_r1_encoder;
}

/******************************************************************************
函数功能：自设特殊动作1
入口参数：无
返回  值：无
*******************************************************************************/ 
void Car_Special1(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 1500;
	R1_Position_position_Pid.SetPoint = old_r1_encoder + 1500;
}

/******************************************************************************
函数功能：自设特殊动作2
入口参数：无
返回  值：无
*******************************************************************************/ 
void Car_Special2(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 1000;
	R1_Position_position_Pid.SetPoint = old_r1_encoder - 1000;
}
