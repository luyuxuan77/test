#include "tim_g0.h"
#include "bsp.h"

/*
 * ��ʱ��ʱ������Դ������ʱ��(BUSCLK)��������Ƶ (5000000 Hz)
 * ���㶨ʱ��ʱ��Ƶ�ʵĹ�ʽ���£�
 *   timerClkFreq = (��ʱ��ʱ��Դ / (��ʱ��ʱ�ӷ�Ƶ�� * (��ʱ��ʱ��Ԥ��Ƶֵ + 1)))
 *   ���磺50000 Hz = 5000000 Hz / (8 * (99 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_0ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_8,
    .prescale    = 99U,
};

/*
 * ��ʱ����װ��ֵ������������ʼ�����ĵط����Ǹ��ݶ�ʱ�����붨ʱ��ʱ��Ƶ�ʼ���ó��ģ����㹫ʽ���£�
 *   TIMER_0_INST_LOAD_VALUE = (��ʱ���� * ��ʱ��ʱ��Ƶ��) - 1
 *   ���磺TIMER_0_INST_LOAD_VALUE = (50ms * 50000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_0TimerConfig = {
    .period     = TIMER_0_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

void tim_g0_init(void)
{
	DL_TimerG_reset(TIMER_0_INST);												// ���ö�ʱ��G0ģ��
	DL_TimerG_enablePower(TIMER_0_INST);										// ʹ�ܶ�ʱ��G0
	DL_TimerG_setClockConfig(TIMER_0_INST,										// ����ʱ��
	(DL_TimerG_ClockConfig *) &gTIMER_0ClockConfig);							

    DL_TimerG_initTimerMode(TIMER_0_INST,										// ����ģʽ
        (DL_TimerG_TimerConfig *) &gTIMER_0TimerConfig);
	
	DL_Timer_enableInterrupt(TIMER_0_INST , DL_TIMERG_INTERRUPT_ZERO_EVENT);	// ʹ���ж�
	
    DL_TimerG_enableClock(TIMER_0_INST);										// ʹ��ʱ��

    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);										// �����ж�
	
	DL_TimerG_startCounter(TIMER_0_INST);										// ������ʱ��
}

/* �жϷ�����������ʱ�� */
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



