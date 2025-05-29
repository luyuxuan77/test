#include "tim_g6.h"

/*
 * 定时器时钟配置源自总线时钟(BUSCLK) / 1 (80000000 Hz)
 * 定时器时钟频率计算公式如下：
 *   timerClkFreq = (定时器时钟源 / (定时器时钟分频比 * (定时器时钟预分频值 + 1)))
 *   例如：800000 Hz = 80000000 Hz / (1 * (99 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_6ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 99U,
};

/*
 * 定时器装载值（即计数器开始计数的起点）计算如下：
 *   (定时周期 * 定时器时钟频率) - 1
 *   例如：TIMER_6_INST_LOAD_VALUE = (10 ms * 800000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_6TimerConfig = {
    .period     = TIMER_6_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

void tim_g6_init(void)
{
	/* 复位TIMER_6_INST定时器模块 */
    DL_TimerG_reset(TIMER_6_INST);
	/* TIMER_6_INST定时器上电使能 */
    DL_TimerG_enablePower(TIMER_6_INST);
	/* 设置定时器的时钟配置 */
    DL_TimerG_setClockConfig(TIMER_6_INST,
                             (DL_TimerG_ClockConfig *)&gTIMER_6ClockConfig);
	/* 初始化定时器模式 */
    DL_TimerG_initTimerMode(TIMER_6_INST,
                            (DL_TimerG_TimerConfig *)&gTIMER_6TimerConfig);
	/* 使能TIMER_6_INST定时器中断 */
    DL_TimerG_enableInterrupt(TIMER_6_INST, DL_TIMERG_INTERRUPT_ZERO_EVENT);
	/* 定时器TIMER_6_INST时钟使能 */
    DL_TimerG_enableClock(TIMER_6_INST);
	/* 定时器TIMER_6_INST中断使能 */
    NVIC_EnableIRQ(TIMER_6_INST_INT_IRQN); 
	/* 启动定时器TIMER_6_INST */
    DL_TimerG_startCounter(TIMER_6_INST);
}


int Last_Speed = 0;

/* 中断服务函数 */
void TIMER_6_INST_IRQHandler(void)
{
	switch (DL_Timer_getPendingInterrupt(TIMER_6_INST))
		{
			case GPTIMER_CPU_INT_IIDX_STAT_Z:
			{
//				Track_Direction_Control(70);//循迹代码，方向环做外环
				L1_Motor.speed = -get_encoder(1);
				Last_Speed = L1_Motor.speed;
				if(L1_Motor.speed == -544)//解决跳变
				{
					L1_Motor.speed = Last_Speed;
				}
				R1_Motor.speed = get_encoder(3);
				L2_Motor.speed = get_encoder(4);
				R2_Motor.speed = -get_encoder(2);
				
				Pid_control(&L1_Speed_Pid, L1_Motor.speed, SPEED_PID_TYPE);
				Pid_control(&R1_Speed_Pid, R1_Motor.speed, SPEED_PID_TYPE);
				Pid_control(&L2_Speed_Pid, L2_Motor.speed, SPEED_PID_TYPE);
				Pid_control(&R2_Speed_Pid, R2_Motor.speed, SPEED_PID_TYPE);
				
				L1_Motor.motor_pwm = Pid_OutLimit(&L1_Speed_Pid, 3200);
				R1_Motor.motor_pwm = Pid_OutLimit(&R1_Speed_Pid, 3200);
				L2_Motor.motor_pwm = Pid_OutLimit(&L2_Speed_Pid, 3200);
				R2_Motor.motor_pwm = Pid_OutLimit(&R2_Speed_Pid, 3200);
				
				motor_4(L1_Motor.motor_pwm,R1_Motor.motor_pwm,L2_Motor.motor_pwm,R2_Motor.motor_pwm);
			}break;
			default:break;
		}

}
