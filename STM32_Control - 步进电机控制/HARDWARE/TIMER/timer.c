#include "pid.h"
#include "ccd.h"
#include "led.h"
#include "oled.h"
#include "delay.h"
#include "debug.h"
#include "timer.h"
#include "usart.h"
#include "motor.h"
#include "usart3.h"
#include "mymath.h"
#include "stdlib.h"
#include "encoder.h"
#include "control.h"
#include "Emm_V5.h"


/**************************************************************************
函数功能：TIM6中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM6_DAC_IRQHandler(void)
{
	if(TIM6->SR&0X0001)
	{
		Encoder2_Pid.SetPoint=Mid_Val;
		Pid_control(&Encoder2_Pid, Cnt, 1);
		pid_result = Pid_OutLimit(&Encoder2_Pid,3000);
		OLED_ShowNum(0,0,Cnt,5,24);
		OLED_ShowNum(0,40,ABS(pid_result),5,24);
		OLED_Refresh_Gram();
		
		if(Cnt > Mid_Val - Mid_Val_Bias&& Cnt < Mid_Val + Mid_Val_Bias)
		{
			Cnt=Mid_Val;
		}
		
		if(Cnt>1300 && Cnt<Mid_Val)
		{
			step_motor_1(pid_result,0,200);  //顺时针方向
		}
		else if(Cnt>Mid_Val && Cnt<2800)
		{
			step_motor_1(pid_result,0,200);  //逆时针方向
		}
		else
		{
			Emm_V5_Stop_Now(1,0); 
			delay_us(200);
		}
		
		delay_us(200);
		Emm_V5_Read_Sys_Params(1, S_CPOS);
		
		Read_Motor_Cur_Pos();
		
		
//		else if(Motor_Cur_Pos < 10)
//		{
//			delay_us(200);
//			Emm_V5_Pos_Control(1,0,200,0,200,1,0);
//			delay_us(200);
//			Emm_V5_Read_Sys_Params(1, S_CPOS);
//			
//			Read_Motor_Cur_Pos();
//			LED1_ON;
//		}
//		Emm_V5_Read_Sys_Params(1, S_VEL);
//		pid_result = My_D_algorithm(Cnt);
//		Read_Motor_Cur_Vel();
//		printf("%d,%f\n",pid_result,Motor_Cur_Vel);
	}
	TIM6->SR&=~(1<<0);
}

/**************************************************************************
函数功能：TIM7中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
int Cnt=0,Mid_Val=2058,Mid_Val_Bias=30;

void TIM7_IRQHandler(void)
{
	if(TIM7->SR&0X0001)
	{
		
//		Cnt = TIM2->CNT;
//		printf("%d\n",Cnt);
//		if(TIM2->CNT>=4096)
//		{
//			TIM2->CNT=0;
//			Cnt=0;
//		}
		
		
//		OLED_ShowNum(0,0,Cnt,5,24);
//		OLED_Refresh_Gram();
		
//		Pid_control(&Encoder1_Pid, Cnt, ENCODER_PID_TYPE);
//		
//		OLED_ShowNum(0,20,Pid_OutLimit(&Encoder1_Pid,20000),5,24);
//		OLED_ShowNum(0,40,Pid_OutLimit(&Encoder1_Pid,2000000),7,24);
//		OLED_Refresh_Gram();
//		if(Pid_OutLimit(&Encoder1_Pid,20000)>0&&Pid_OutLimit(&Encoder1_Pid,20000)<10000)
//		{
//			step_motor_1(-200,500,0,30);  //顺时针方向
//		}
//		else if(Pid_OutLimit(&Encoder1_Pid,20000)<0&&Pid_OutLimit(&Encoder1_Pid,20000)>-10000)
//		{
//			step_motor_1(200,500,0,30);  //逆时针方向
//		}
//		else
//		{
//			Emm_V5_Stop_Now(1,0);
//			delay_ms(10);
//		}
//		TIM2->CNT=0;
//		printf("%d\r\n",Cnt);

			
//		if (Automobile_Data_Recorder.Car_State_Flag == Line_Walking_State)  // 小车状态，巡线状态
//		{
//			//CCD方向环pid控制，得到当前偏移值，经pid运算输出速度环目标值
//			CCD_Track_Direction_Control(89);
//			
//			//速度环pid控制，输出电机pwm比较值
//			Pid_control(&L1_Speed_Pid, L1_Motor.speed, SPEED_PID_TYPE);
//			Pid_control(&R1_Speed_Pid, R1_Motor.speed, SPEED_PID_TYPE);
//			
//			//电机pwm比较值限幅
//			L1_Motor.motor_pwm = Pid_OutLimit(&L1_Speed_Pid, 15000);
//			R1_Motor.motor_pwm = Pid_OutLimit(&R1_Speed_Pid, 15000);
//			
//			//输出pwm给电机
//			Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
//			Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
//			
//			Send_To_Debug(0);
//		}
//		else if (Automobile_Data_Recorder.Car_State_Flag == Specific_Action_Execute_State)  // 小车状态。执行特殊动作状态
//		{
//			//得到当前的电机位置
//			L1_Motor.location = Get_Encoder_Count(&L1_Encoder);
//			R1_Motor.location = Get_Encoder_Count(&R1_Encoder);
//			
//			//位置控制位置环pid控制，输出速度环目标值
//			Pid_control(&L1_Position_position_Pid, L1_Motor.location, POSITION_Position_PID_TYPE);
//			Pid_control(&R1_Position_position_Pid, R1_Motor.location, POSITION_Position_PID_TYPE);
//			
//			//速度环目标值限幅
//			L1_Position_speed_Pid.SetPoint = Pid_OutLimit(&L1_Position_position_Pid, 150);
//			R1_Position_speed_Pid.SetPoint = Pid_OutLimit(&R1_Position_position_Pid, 150);
//			
//			//位置控制速度环pid控制，输出电机pwm比较值
//			Pid_control(&L1_Position_speed_Pid, L1_Motor.speed, POSITION_Speed_PID_TYPE);
//			Pid_control(&R1_Position_speed_Pid, R1_Motor.speed, POSITION_Speed_PID_TYPE);
//			
//			//电机pwm比较值限幅
//			L1_Motor.motor_pwm = Pid_OutLimit(&L1_Position_speed_Pid, 15000);
//			R1_Motor.motor_pwm = Pid_OutLimit(&R1_Position_speed_Pid, 15000);
//			
//			//输出pwm到电机
//			Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
//			Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
//			
//			//如果小车特殊动作执行完毕，则。。。可自定义
//			if (abs((int)(L1_Motor.location - L1_Position_position_Pid.SetPoint)) < 100 
//				&&abs((int)(R1_Motor.location - R1_Position_position_Pid.SetPoint) < 100) 
//				&&Automobile_Data_Recorder.Specific_Action_Group_i < Specific_Action_Group_Amount - 1)
//			{
//				Automobile_Data_Recorder.Car_State_Flag = Line_Walking_State;
//				Get_CCD_Value();
//				delay_ms(50);
//				Get_CCD_Value();
//			}
//			
//		}
//		else if (Automobile_Data_Recorder.Car_State_Flag == Stop_And_Switch_State)  // 小车停止及小车状态转换状态
//		{
//			//初始化各PID参数，使各PID各参数为零，防止影响初始控制
//			Pid_Init( &L1_Position_position_Pid , &L1_PID_Value_Position_position );  // 初始化PID参数，位置控制位置环，左1轮
//			Pid_Init( &R1_Position_position_Pid , &R1_PID_Value_Position_position );  // 初始化PID参数，位置控制位置环，右1轮
//			Pid_Init( &L1_Position_speed_Pid , &L1_PID_Value_Position_speed );  // 初始化PID参数，位置控制速度环，左1轮
//			Pid_Init( &R1_Position_speed_Pid , &R1_PID_Value_Position_speed );  // 初始化PID参数，位置控制速度环，右1轮
//			Pid_Init( &L1_Speed_Pid , &L1_PID_Value_Speed );  // 初始化PID参数，速度控制，左1轮
//			Pid_Init( &R1_Speed_Pid , &R1_PID_Value_Speed );  // 初始化PID参数，速度控制，右1轮
//			Pid_Init( &CCD_Direction_Pid , &PID_Value_CCD_Direction[0] );  // 初始化PID参数，速度控制方向环，CCD
//			
//			Automobile_Data_Recorder.Car_State_Flag = Specific_Action_Execute_State;  // 小车状态为执行特殊动作状态
//			Specific_Action_Execute();		//特殊动作选择
//		}
	}
	TIM7->SR&=~(1<<0);
}


void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM9->SR&0X0001)
	{
		Cnt = TIM2->CNT;
//		delay_us(200);
//		Emm_V5_Read_Sys_Params(1, S_CPOS);
//		Read_Motor_Cur_Pos();
//		if(Motor_Cur_Pos > 90)
//		{
//			delay_us(200);
//			Emm_V5_Pos_Control(1,1,100,0,100,0,0);
//			delay_us(200);
//			Emm_V5_Stop_Now(1,0);
//		}
		
	}
	TIM9->SR&=~(1<<0);
}

	
	


/**************************************************************************
函数功能：把TIM6初始化为定时器中断模式
入口参数：arr:定时器自动重装载值；psc:预分频值	ps:arr=1000,psc=84.定时时间1ms
返回  值：无
**************************************************************************/
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<4;
	TIM6->ARR = arr;
	TIM6->PSC = psc;
	TIM6->DIER |= 1<<0;
	TIM6->CR1 |= 0X01;
	MY_NVIC_Init(3, 1, TIM6_DAC_IRQn, 2);
}

/**************************************************************************
函数功能：把TIM7初始化为定时器中断模式
入口参数：arr:定时器自动重装载值；psc:预分频值	ps:arr=1000,psc=84.定时时间1ms
返回  值：无
**************************************************************************/
void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR |= 1<<5;
	TIM7->ARR = arr;
	TIM7->PSC = psc;
	TIM7->DIER |= 1<<0;	
	TIM7->CR1 |= 0X01;
	MY_NVIC_Init(3 ,3, TIM7_IRQn, 3);
}

/**************************************************************************
函数功能：TIM14初始化，启动TIM14，做计数器
入口参数：arr:定时器自动重装载值；psc:预分频值	ps:arr=1000,psc=84.定时时间1ms
返回  值：无
**************************************************************************/
void TIM9_Int_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR |= 1<<16;
	TIM9->ARR = arr;  // 最多计数到6000.0ms
	TIM9->PSC = psc;  // 0.1ms计一个数
	TIM9->DIER |= 1<<0;
	TIM9->CR1 |= 0X01;
	MY_NVIC_Init(3 ,2, TIM1_BRK_TIM9_IRQn, 3);
}
