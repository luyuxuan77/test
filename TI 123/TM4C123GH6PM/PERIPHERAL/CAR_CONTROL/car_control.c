#include "car_control.h"
#include "encoder.h"
#include "pid.h"

//�����г���¼�ǽṹ������������г�����
Automobile_Data_Recorder_TypeDef Automobile_Data_Recorder = 
{
	Line_Walking_State,	//Ѳ��״̬
	{Special_1, Special_2},	//������
	0	//ִ�е���0������
};

/******************************************************************************
�������ܣ����⶯��ѡ����������ִ�ж������еĶ���
��ڲ�������
����  ֵ����
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
�������ܣ����⶯��--ֱ��
��ڲ�������
����  ֵ����
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
�������ܣ����⶯��--��ת
��ڲ�������
����  ֵ����
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
�������ܣ����⶯��--��ת
��ڲ�������
����  ֵ����
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
�������ܣ����⶯��--��ͷ
��ڲ�������
����  ֵ����
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
�������ܣ����⶯��--ͣ��
��ڲ�������
����  ֵ����
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
�������ܣ����⶯��--ͣ��
��ڲ�������
����  ֵ����
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
�������ܣ��������⶯��1
��ڲ�������
����  ֵ����
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
�������ܣ��������⶯��2
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Car_Special2(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_QEI_Count(L);
	old_r1_encoder = Get_QEI_Count(R);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder + 1000;
	R1_Position_position_Pid.SetPoint = old_r1_encoder - 1000;
}
