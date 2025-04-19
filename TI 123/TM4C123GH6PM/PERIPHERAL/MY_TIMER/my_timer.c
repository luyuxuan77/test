#include "my_timer.h"
#include "encoder.h"
#include "control.h"
#include "mymath.h"
#include "motor.h"
#include "uart0.h"
#include "pid.h"
#include "ccd.h"
#include "uart1.h"
unsigned char Laser_Command[9] = {0xAA,0x00,0x00,0x20,0x00,0x01,0x00,0x06,0x27};
/**************************************************************************
�������ܣ�TIM0�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIMER0_IRQHandler(void)
{
	MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // ����жϱ�־λ
//	MAP_IntMasterDisable();  // �����ж�
//	/* ������жϺ�������Ҫ���������*/
//	MAP_IntMasterEnable();  // �����ж�
	L1_Motor.speed = Get_Motor_Speed(L, 1);
	R1_Motor.speed = Get_Motor_Speed(R, 1);
	//printf("%f,100\r\n",L1_Motor.speed);
	Pid_control(&L1_Speed_Pid, L1_Motor.speed, SPEED_PID_TYPE);
	Pid_control(&R1_Speed_Pid, R1_Motor.speed, SPEED_PID_TYPE);
	
	L1_Motor.motor_pwm = Pid_OutLimit(&L1_Speed_Pid, 10000);
	R1_Motor.motor_pwm = Pid_OutLimit(&R1_Speed_Pid, 10000);
	
//	Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
//	Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
	//Get_CCD_Value();

}
char cheat = 0,times = 0,Flag = 0;
/**************************************************************************
�������ܣ�TIM1�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIMER1_IRQHandler(void)
{
	times++;
	if(cheat != 0)
	{
		if(times > cheat)
		{
			Flag = 1;
			cheat = 0;
			times = 0;
		}
	}
	else
	{
		times = 0;
	}
	for(int i = 0;i < 9;i++)
	{
		UARTCharPut(UART7_BASE,Laser_Command[i]);
	}
	MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);  // ����жϱ�־λ
//	MAP_IntMasterDisable();  // �����ж�
//	/* ������жϺ�������Ҫ���������*/
//	MAP_IntMasterEnable();  // �����ж�
	
}

/**************************************************************************
�������ܣ���TIM0��ʼ������ʱ���ж�ģʽ
��ڲ�����time����ʱʱ��(ms)
����  ֵ����
**************************************************************************/
void Timer0_Init(uint16_t time)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //ʹ�� TIMER0 ����
	
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  //��ʱ��ģʽ�趨Ϊ����������ģʽ
	TimerLoadSet(TIMER0_BASE, TIMER_A, ((MAP_SysCtlClockGet() / 1000) * time) - 1);  //���ö�ʱ��װ��ֵ����time ms��һ���ж�
	
	TimerIntRegister( TIMER0_BASE, TIMER_A, TIMER0_IRQHandler);  //ע�ᶨʱ���жϺ���
	MAP_IntPrioritySet(INT_TIMER0A, 0);	//�����ж����ȼ���TM4C123G���ж����ȼ���8����0���
	MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  //ʹ�ܶ�ʱ��0�ж�
	MAP_IntEnable(INT_TIMER0A);  //������ʱ��0�ж�
	
	MAP_IntMasterEnable();  //�����������ж�
	MAP_TimerEnable(TIMER0_BASE, TIMER_A);  //ʹ�� TIMER_A ��ʱ��
}

/**************************************************************************
�������ܣ���TIM1��ʼ������ʱ���ж�ģʽ
��ڲ�����time����ʱʱ��(ms)
����  ֵ����
**************************************************************************/
void Timer1_Init(uint16_t time)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  //ʹ�� TIMER1 ����

	MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);  //��ʱ��ģʽ�趨Ϊ����������ģʽ
	MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, ((MAP_SysCtlClockGet() / 1000) * time) - 1);  //���ö�ʱ��װ��ֵ����time ms��һ���ж�

	TimerIntRegister( TIMER1_BASE, TIMER_A, TIMER1_IRQHandler);  //ע�ᶨʱ���жϺ���
	MAP_IntPrioritySet(INT_TIMER1A, 1);	//�����ж����ȼ���TM4C123G���ж����ȼ���8����0���
	MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);  //ʹ�ܶ�ʱ��1�ж�
	MAP_IntEnable(INT_TIMER1A);  //������ʱ��1�ж�
	
	MAP_IntMasterEnable();  //�����������ж�
	MAP_TimerEnable(TIMER1_BASE, TIMER_A);  //ʹ�� TIMER_A ��ʱ��
}
