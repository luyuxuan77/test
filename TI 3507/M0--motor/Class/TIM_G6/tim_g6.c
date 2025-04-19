#include "tim_g6.h"

/*
 * ��ʱ��ʱ������Դ������ʱ��(BUSCLK) / 1 (80000000 Hz)
 * ��ʱ��ʱ��Ƶ�ʼ��㹫ʽ���£�
 *   timerClkFreq = (��ʱ��ʱ��Դ / (��ʱ��ʱ�ӷ�Ƶ�� * (��ʱ��ʱ��Ԥ��Ƶֵ + 1)))
 *   ���磺800000 Hz = 80000000 Hz / (1 * (99 + 1))
 */
static const DL_TimerG_ClockConfig gTIMER_6ClockConfig = {
    .clockSel    = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale    = 99U,
};

/*
 * ��ʱ��װ��ֵ������������ʼ��������㣩�������£�
 *   (��ʱ���� * ��ʱ��ʱ��Ƶ��) - 1
 *   ���磺TIMER_6_INST_LOAD_VALUE = (10 ms * 800000 Hz) - 1
 */
static const DL_TimerG_TimerConfig gTIMER_6TimerConfig = {
    .period     = TIMER_6_INST_LOAD_VALUE,
    .timerMode  = DL_TIMER_TIMER_MODE_PERIODIC,
    .startTimer = DL_TIMER_STOP,
};

void tim_g6_init(void)
{
	/* ��λTIMER_6_INST��ʱ��ģ�� */
    DL_TimerG_reset(TIMER_6_INST);
	/* TIMER_6_INST��ʱ���ϵ�ʹ�� */
    DL_TimerG_enablePower(TIMER_6_INST);
	/* ���ö�ʱ����ʱ������ */
    DL_TimerG_setClockConfig(TIMER_6_INST,
                             (DL_TimerG_ClockConfig *)&gTIMER_6ClockConfig);
	/* ��ʼ����ʱ��ģʽ */
    DL_TimerG_initTimerMode(TIMER_6_INST,
                            (DL_TimerG_TimerConfig *)&gTIMER_6TimerConfig);
	/* ʹ��TIMER_6_INST��ʱ���ж� */
    DL_TimerG_enableInterrupt(TIMER_6_INST, DL_TIMERG_INTERRUPT_ZERO_EVENT);
	/* ��ʱ��TIMER_6_INSTʱ��ʹ�� */
    DL_TimerG_enableClock(TIMER_6_INST);
	/* ��ʱ��TIMER_6_INST�ж�ʹ�� */
    NVIC_EnableIRQ(TIMER_6_INST_INT_IRQN); 
	/* ������ʱ��TIMER_6_INST */
    DL_TimerG_startCounter(TIMER_6_INST);
}


int Last_Speed = 0;

/* �жϷ����� */
void TIMER_6_INST_IRQHandler(void)
{
	switch (DL_Timer_getPendingInterrupt(TIMER_6_INST))
		{
			case GPTIMER_CPU_INT_IIDX_STAT_Z:
			{
//				Track_Direction_Control(70);//ѭ�����룬�������⻷
				L1_Motor.speed = -get_encoder(1);
				Last_Speed = L1_Motor.speed;
				if(L1_Motor.speed == -544)//�������
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
