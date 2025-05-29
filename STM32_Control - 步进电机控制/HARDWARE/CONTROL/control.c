#include "control.h"
#include "encoder.h"
#include "motor.h"
#include "track.h"
#include "usart.h"
#include "Emm_V5.h"
#include "timer.h"
#include "pid.h"
#include "led.h"
#include "key.h"
#include "oled.h"

//�����г���¼�ǽṹ������������г�����
Automobile_Data_Recorder_TypeDef Automobile_Data_Recorder = 
{
	Line_Walking_State,  // Ѳ��״̬
	{Advance, Advance, Left_Turn, Right_Turn, Park, Stop},  // ������
	0  // ִ�е���0������
};

float P=90,I=0,D=0,error,sumerror,lasterror;//��Ҫ�ֶ�pid,Ŀǰ�ڷ�Χ�ڻ��°ڷ�����ϰڸ�ת����������:1,0.5,0
int result;
int My_D_algorithm(int cnt)
{
	error = 2050 - cnt;
	
	if(sumerror > 2000)
	{
		sumerror=2000;
	}
	else if(sumerror < -2000)
	{
		sumerror=-2000;
	}
	
	sumerror += error;
	result = P*error + I*sumerror + D*(error-lasterror);
	lasterror = error;
	
	if(result >= 3000)
	{
		result=3000;
	}
	else if(result <= -3000)
	{
		result=-3000;
	}
	
	return result;
}



/******************************************************************************
�������ܣ��߼����ƺ������Զ���
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Control_1(void)
{
	while(1)
	{
		Emm_V5_Pos_Control(1,0,3000,0,400,1,0);
		delay_ms(200);
		Emm_V5_Stop_Now(1,0);
		delay_ms(10);
		Emm_V5_Pos_Control(1,1,3000,0,400,1,0);
		delay_ms(200);
		Emm_V5_Stop_Now(1,0);
		delay_ms(10);
		if(KEY_Scan(0)==KEY0_PRES)
		{
			LED1_ON;
			delay_ms(20);
			LED1_OFF;
			TIM2->CNT=0;
			Cnt=0;
			return;
		}
	}
}

int Error=0,pid_result=0;
void Control_2(void)
{
	while(1)
	{
		Error=Mid_Val-Cnt;
		if(Cnt>Mid_Val)
		{
			Error=Cnt-Mid_Val;
		}
		OLED_ShowNum(70,0,Error,4,24);
//		pid_result = My_D_algorithm(Cnt);
//		OLED_ShowNum(0,0,Cnt,5,24);
//		OLED_ShowNum(0,40,ABS(pid_result),5,24);
//		OLED_Refresh_Gram();
//		
//		if(Cnt > 2035 && Cnt < 2065)
//		{
//			Cnt=2050;
//		}
//		
//		if(Cnt>1300 && Cnt<2050)
//		{
//			step_motor_1(pid_result,0,200);  //˳ʱ�뷽��
//		}
//		else if(Cnt>2050 && Cnt<2800)
//		{
//			step_motor_1(pid_result,0,200);  //��ʱ�뷽��
//		}
//		else
//		{
//			Emm_V5_Stop_Now(1,0);
//			delay_us(200);
//		}
		
//		OLED_ShowNum(70,20,ABS(Pid_OutLimit(&Encoder1_Pid,2000)),5,24);
		
//		printf("%d\n",Cnt);
//		if(Cnt<2050&&Cnt>1025)
//		{
//			Error=My_D_algorithm(Error);
//			step_motor_1(-50,Error,0,200);  //˳ʱ�뷽��
//		}
//		else if(Cnt>2050&&Cnt<3075)
//		{
//			Error=My_D_algorithm(Error);
//			step_motor_1(50,Error,0,200);  //��ʱ�뷽��
//		}
//		else if(Cnt==2050)
//		{
//			Emm_V5_Stop_Now(1,0);
//			delay_us(200);
//			Emm_V5_En_Control(1,1,0);
//			delay_us(200);
//		}
//		else
//		{
//			Emm_V5_Stop_Now(1,0);
//			delay_us(200);
//		}
		
//		Emm_V5_Pos_Control(1,0,3000,0,400,1,0);
//		delay_ms(200);
//		Emm_V5_Stop_Now(1,0);
//		delay_ms(10);
//		Emm_V5_Pos_Control(1,1,3000,0,400,1,0);
//		delay_ms(200);
//		Emm_V5_Stop_Now(1,0);
//		delay_ms(10);
		if(KEY_Scan(0)==KEY0_PRES)
		{
			LED1_ON;
			delay_ms(20);
			LED1_OFF;
			TIM2->CNT=0;
			Cnt=0;
			return;
		}
	}
}
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
		default:	LED1 = !LED1;	break;
	}
	if (Automobile_Data_Recorder.Specific_Action_Group_i < Specific_Action_Group_Amount - 1)	
	{
		Automobile_Data_Recorder.Specific_Action_Group_i++;
	}
}

/******************************************************************************
�������ܣ����⶯��--ֱ��
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Go_Straight(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_Encoder_Count(&L1_Encoder);
	old_r1_encoder = Get_Encoder_Count(&R1_Encoder);
	
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
	old_l1_encoder = Get_Encoder_Count(&L1_Encoder);
	old_r1_encoder = Get_Encoder_Count(&R1_Encoder);
	
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
	old_l1_encoder = Get_Encoder_Count(&L1_Encoder);
	old_r1_encoder = Get_Encoder_Count(&R1_Encoder);
	
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
	old_l1_encoder = Get_Encoder_Count(&L1_Encoder);
	old_r1_encoder = Get_Encoder_Count(&R1_Encoder);
	
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
	old_l1_encoder = Get_Encoder_Count(&L1_Encoder);
	old_r1_encoder = Get_Encoder_Count(&R1_Encoder);
	
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
	old_l1_encoder = Get_Encoder_Count(&L1_Encoder);
	old_r1_encoder = Get_Encoder_Count(&R1_Encoder);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder;
	R1_Position_position_Pid.SetPoint = old_r1_encoder;
}
