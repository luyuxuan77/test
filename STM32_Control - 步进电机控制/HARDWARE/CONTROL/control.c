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

//定义行车记录仪结构体变量，储存行车数据
Automobile_Data_Recorder_TypeDef Automobile_Data_Recorder = 
{
	Line_Walking_State,  // 巡线状态
	{Advance, Advance, Left_Turn, Right_Turn, Park, Stop},  // 动作组
	0  // 执行到第0个动作
};

float P=90,I=0,D=0,error,sumerror,lasterror;//需要分段pid,目前在范围内会下摆幅变大，上摆杆转动中心下移:1,0.5,0
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
函数功能：逻辑控制函数，自定义
入口参数：无
返回  值：无
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
//			step_motor_1(pid_result,0,200);  //顺时针方向
//		}
//		else if(Cnt>2050 && Cnt<2800)
//		{
//			step_motor_1(pid_result,0,200);  //逆时针方向
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
//			step_motor_1(-50,Error,0,200);  //顺时针方向
//		}
//		else if(Cnt>2050&&Cnt<3075)
//		{
//			Error=My_D_algorithm(Error);
//			step_motor_1(50,Error,0,200);  //逆时针方向
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
		default:	LED1 = !LED1;	break;
	}
	if (Automobile_Data_Recorder.Specific_Action_Group_i < Specific_Action_Group_Amount - 1)	
	{
		Automobile_Data_Recorder.Specific_Action_Group_i++;
	}
}

/******************************************************************************
函数功能：特殊动作--直行
入口参数：无
返回  值：无
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
函数功能：特殊动作--左转
入口参数：无
返回  值：无
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
函数功能：特殊动作--右转
入口参数：无
返回  值：无
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
函数功能：特殊动作--掉头
入口参数：无
返回  值：无
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
函数功能：特殊动作--停车
入口参数：无
返回  值：无
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
函数功能：特殊动作--停车
入口参数：无
返回  值：无
*******************************************************************************/ 
void Car_Stop(void)
{
	int old_l1_encoder, old_r1_encoder;
	old_l1_encoder = Get_Encoder_Count(&L1_Encoder);
	old_r1_encoder = Get_Encoder_Count(&R1_Encoder);
	
	L1_Position_position_Pid.SetPoint = old_l1_encoder;
	R1_Position_position_Pid.SetPoint = old_r1_encoder;
}
