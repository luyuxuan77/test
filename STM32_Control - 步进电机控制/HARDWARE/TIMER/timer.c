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
�������ܣ�TIM6�жϷ�����
��ڲ�������
����  ֵ����
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
			step_motor_1(pid_result,0,200);  //˳ʱ�뷽��
		}
		else if(Cnt>Mid_Val && Cnt<2800)
		{
			step_motor_1(pid_result,0,200);  //��ʱ�뷽��
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
�������ܣ�TIM7�жϷ�����
��ڲ�������
����  ֵ����
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
//			step_motor_1(-200,500,0,30);  //˳ʱ�뷽��
//		}
//		else if(Pid_OutLimit(&Encoder1_Pid,20000)<0&&Pid_OutLimit(&Encoder1_Pid,20000)>-10000)
//		{
//			step_motor_1(200,500,0,30);  //��ʱ�뷽��
//		}
//		else
//		{
//			Emm_V5_Stop_Now(1,0);
//			delay_ms(10);
//		}
//		TIM2->CNT=0;
//		printf("%d\r\n",Cnt);

			
//		if (Automobile_Data_Recorder.Car_State_Flag == Line_Walking_State)  // С��״̬��Ѳ��״̬
//		{
//			//CCD����pid���ƣ��õ���ǰƫ��ֵ����pid��������ٶȻ�Ŀ��ֵ
//			CCD_Track_Direction_Control(89);
//			
//			//�ٶȻ�pid���ƣ�������pwm�Ƚ�ֵ
//			Pid_control(&L1_Speed_Pid, L1_Motor.speed, SPEED_PID_TYPE);
//			Pid_control(&R1_Speed_Pid, R1_Motor.speed, SPEED_PID_TYPE);
//			
//			//���pwm�Ƚ�ֵ�޷�
//			L1_Motor.motor_pwm = Pid_OutLimit(&L1_Speed_Pid, 15000);
//			R1_Motor.motor_pwm = Pid_OutLimit(&R1_Speed_Pid, 15000);
//			
//			//���pwm�����
//			Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
//			Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
//			
//			Send_To_Debug(0);
//		}
//		else if (Automobile_Data_Recorder.Car_State_Flag == Specific_Action_Execute_State)  // С��״̬��ִ�����⶯��״̬
//		{
//			//�õ���ǰ�ĵ��λ��
//			L1_Motor.location = Get_Encoder_Count(&L1_Encoder);
//			R1_Motor.location = Get_Encoder_Count(&R1_Encoder);
//			
//			//λ�ÿ���λ�û�pid���ƣ�����ٶȻ�Ŀ��ֵ
//			Pid_control(&L1_Position_position_Pid, L1_Motor.location, POSITION_Position_PID_TYPE);
//			Pid_control(&R1_Position_position_Pid, R1_Motor.location, POSITION_Position_PID_TYPE);
//			
//			//�ٶȻ�Ŀ��ֵ�޷�
//			L1_Position_speed_Pid.SetPoint = Pid_OutLimit(&L1_Position_position_Pid, 150);
//			R1_Position_speed_Pid.SetPoint = Pid_OutLimit(&R1_Position_position_Pid, 150);
//			
//			//λ�ÿ����ٶȻ�pid���ƣ�������pwm�Ƚ�ֵ
//			Pid_control(&L1_Position_speed_Pid, L1_Motor.speed, POSITION_Speed_PID_TYPE);
//			Pid_control(&R1_Position_speed_Pid, R1_Motor.speed, POSITION_Speed_PID_TYPE);
//			
//			//���pwm�Ƚ�ֵ�޷�
//			L1_Motor.motor_pwm = Pid_OutLimit(&L1_Position_speed_Pid, 15000);
//			R1_Motor.motor_pwm = Pid_OutLimit(&R1_Position_speed_Pid, 15000);
//			
//			//���pwm�����
//			Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
//			Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
//			
//			//���С�����⶯��ִ����ϣ��򡣡������Զ���
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
//		else if (Automobile_Data_Recorder.Car_State_Flag == Stop_And_Switch_State)  // С��ֹͣ��С��״̬ת��״̬
//		{
//			//��ʼ����PID������ʹ��PID������Ϊ�㣬��ֹӰ���ʼ����
//			Pid_Init( &L1_Position_position_Pid , &L1_PID_Value_Position_position );  // ��ʼ��PID������λ�ÿ���λ�û�����1��
//			Pid_Init( &R1_Position_position_Pid , &R1_PID_Value_Position_position );  // ��ʼ��PID������λ�ÿ���λ�û�����1��
//			Pid_Init( &L1_Position_speed_Pid , &L1_PID_Value_Position_speed );  // ��ʼ��PID������λ�ÿ����ٶȻ�����1��
//			Pid_Init( &R1_Position_speed_Pid , &R1_PID_Value_Position_speed );  // ��ʼ��PID������λ�ÿ����ٶȻ�����1��
//			Pid_Init( &L1_Speed_Pid , &L1_PID_Value_Speed );  // ��ʼ��PID�������ٶȿ��ƣ���1��
//			Pid_Init( &R1_Speed_Pid , &R1_PID_Value_Speed );  // ��ʼ��PID�������ٶȿ��ƣ���1��
//			Pid_Init( &CCD_Direction_Pid , &PID_Value_CCD_Direction[0] );  // ��ʼ��PID�������ٶȿ��Ʒ��򻷣�CCD
//			
//			Automobile_Data_Recorder.Car_State_Flag = Specific_Action_Execute_State;  // С��״̬Ϊִ�����⶯��״̬
//			Specific_Action_Execute();		//���⶯��ѡ��
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
�������ܣ���TIM6��ʼ��Ϊ��ʱ���ж�ģʽ
��ڲ�����arr:��ʱ���Զ���װ��ֵ��psc:Ԥ��Ƶֵ	ps:arr=1000,psc=84.��ʱʱ��1ms
����  ֵ����
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
�������ܣ���TIM7��ʼ��Ϊ��ʱ���ж�ģʽ
��ڲ�����arr:��ʱ���Զ���װ��ֵ��psc:Ԥ��Ƶֵ	ps:arr=1000,psc=84.��ʱʱ��1ms
����  ֵ����
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
�������ܣ�TIM14��ʼ��������TIM14����������
��ڲ�����arr:��ʱ���Զ���װ��ֵ��psc:Ԥ��Ƶֵ	ps:arr=1000,psc=84.��ʱʱ��1ms
����  ֵ����
**************************************************************************/
void TIM9_Int_Init(u16 arr,u16 psc)
{
	RCC->APB2ENR |= 1<<16;
	TIM9->ARR = arr;  // ��������6000.0ms
	TIM9->PSC = psc;  // 0.1ms��һ����
	TIM9->DIER |= 1<<0;
	TIM9->CR1 |= 0X01;
	MY_NVIC_Init(3 ,2, TIM1_BRK_TIM9_IRQn, 3);
}
