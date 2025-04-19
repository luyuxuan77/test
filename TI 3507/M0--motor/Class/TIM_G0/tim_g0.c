#include "tim_g0.h"
#include "bsp.h"

/*
 * 定时器时钟配置源自总线时钟(BUSCLK)，经过分频 (5000000 Hz)
 * 计算定时器时钟频率的公式如下：
 *   timerClkFreq = (定时器时钟源 / (定时器时钟分频比 * (定时器时钟预分频值 + 1)))
 *   例如：50000 Hz = 5000000 Hz / (8 * (99 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 99U,
};

/*
 * 定时器的装载值（即计数器开始计数的地方）是根据定时周期与定时器时钟频率计算得出的，计算公式如下：
 *   TIMER_0_INST_LOAD_VALUE = (定时周期 * 定时器时钟频率) - 1
 *   例如：TIMER_0_INST_LOAD_VALUE = (50ms * 50000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

void tim_g0_init(void)
{
	DL_TimerG_reset(TIMER_0_INST);												// 重置定时器G0模块
	DL_TimerG_enablePower(TIMER_0_INST);										// 使能定时器G0
	DL_TimerG_setClockConfig(TIMER_0_INST,										// 设置时钟
	(DL_TimerG_ClockConfig *) &gTIMER_0ClockConfig);							

    DL_TimerG_initTimerMode(TIMER_0_INST,										// 设置模式
        (DL_TimerG_TimerConfig *) &gTIMER_0TimerConfig);
	
	DL_Timer_enableInterrupt(TIMER_0_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);	// 使能中断
	
    DL_TimerG_enableClock(TIMER_0_INST);										// 使能时钟

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);										// 开启中断
	
	DL_TimerG_startCounter(TIMER_0_INST);										// 启动计时器
}

/* 中断服务函数用作定时器 */
int Flag,Count,Time;
void TIMER_0_INST_IRQHandler(void)
{
	Time++;
	if(Count!=0)
	{
		if(Time>=Count)
		{
			Time=0;
			Count=0;
			Flag=1;
		}
	}
	else
	{
		Time=0;
	}

}



