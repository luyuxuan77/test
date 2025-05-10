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
函数功能：TIM0中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIMER0_IRQHandler(void)
{
	MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // 清除中断标志位
//	MAP_IntMasterDisable();  // 禁用中断
//	/* 下面放中断函数中需要处理的内容*/
//	MAP_IntMasterEnable();  // 启用中断
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
函数功能：TIM1中断服务函数
入口参数：无
返回  值：无
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
	MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);  // 清除中断标志位
//	MAP_IntMasterDisable();  // 禁用中断
//	/* 下面放中断函数中需要处理的内容*/
//	MAP_IntMasterEnable();  // 启用中断
	
}

/**************************************************************************
函数功能：把TIM0初始化，定时器中断模式
入口参数：time：定时时间(ms)
返回  值：无
**************************************************************************/
void Timer0_Init(uint16_t time)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //使能 TIMER0 外设
	
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);  //定时器模式设定为连续减计数模式
	TimerLoadSet(TIMER0_BASE, TIMER_A, ((MAP_SysCtlClockGet() / 1000) * time) - 1);  //配置定时器装载值，过time ms进一次中断
	
	TimerIntRegister( TIMER0_BASE, TIMER_A, TIMER0_IRQHandler);  //注册定时器中断函数
	MAP_IntPrioritySet(INT_TIMER0A, 0);	//设置中断优先级，TM4C123G的中断优先级有8个，0最高
	MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  //使能定时器0中断
	MAP_IntEnable(INT_TIMER0A);  //启动定时器0中断
	
	MAP_IntMasterEnable();  //启动处理器中断
	MAP_TimerEnable(TIMER0_BASE, TIMER_A);  //使能 TIMER_A 定时器
}

/**************************************************************************
函数功能：把TIM1初始化，定时器中断模式
入口参数：time：定时时间(ms)
返回  值：无
**************************************************************************/
void Timer1_Init(uint16_t time)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  //使能 TIMER1 外设

	MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);  //定时器模式设定为连续减计数模式
	MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, ((MAP_SysCtlClockGet() / 1000) * time) - 1);  //配置定时器装载值，过time ms进一次中断

	TimerIntRegister( TIMER1_BASE, TIMER_A, TIMER1_IRQHandler);  //注册定时器中断函数
	MAP_IntPrioritySet(INT_TIMER1A, 1);	//设置中断优先级，TM4C123G的中断优先级有8个，0最高
	MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);  //使能定时器1中断
	MAP_IntEnable(INT_TIMER1A);  //启动定时器1中断
	
	MAP_IntMasterEnable();  //启动处理器中断
	MAP_TimerEnable(TIMER1_BASE, TIMER_A);  //使能 TIMER_A 定时器
}
