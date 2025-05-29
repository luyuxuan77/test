#include "tim_g8_pwm.h"

/*
 * ��ʱ��ʱ������Դ��ϵͳʱ�� / 1 (40000000 Hz)
 * ��ʱ��ʱ��Ƶ�ʼ��㹫ʽ���£�
 *   timerClkFreq = (��ʱ��ʱ��Դ / (��ʱ��ʱ�ӷ�Ƶ�� * (��ʱ��ʱ��Ԥ��Ƶֵ + 1)))
 *   ���磺4000000 Hz = 40000000 Hz / (1 * (9 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_1ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 9U};

static const DL_TimerG_PWMConfig gPWM_1Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 1000,
    .startTimer = DL_TIMER_STOP,
};

void tim_g8_pwm_init(void)
{
	/* ��λPWM_1_INST��ʱ�� */
    DL_TimerG_reset(PWM_1_INST);
	/* PWM_1_INST��ʱ���ϵ�ʹ�� */
    DL_TimerG_enablePower(PWM_1_INST);
	/* ����GPIO_PWM_1_C0��GPIO_PWM_1_C1��ΪPWM������� */
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C0_IOMUX, GPIO_PWM_1_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C0_PORT, GPIO_PWM_1_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C1_IOMUX, GPIO_PWM_1_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C1_PORT, GPIO_PWM_1_C1_PIN);
	/* ����PWM_1_INST��ʱ��ʱ�� */
    DL_TimerG_setClockConfig(
        PWM_1_INST, (DL_TimerG_ClockConfig *)&gPWM_1ClockConfig);
	/* ��ʼ��PWM_1_INST��ʱ��ΪPWMģʽ */
    DL_TimerG_initPWMMode(
        PWM_1_INST, (DL_TimerG_PWMConfig *)&gPWM_1Config);
	/* ���ò���/�ȽϱȽ��������,����ͨ��0�ĳ�ʼ���Ϊ�͵�ƽ������������࣬ʹ�ù���ֵ��Ϊ���Դ */
    DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
                                      DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
                                      DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	/* ���ò���/�Ƚϸ��·���Ϊ�������� */
    DL_TimerG_setCaptCompUpdateMethod(PWM_1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	/* ����ͨ��0�Ĳ���/�Ƚ�ֵΪ800 */
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, 800, DL_TIMER_CC_0_INDEX);

	/* ���ò���/�ȽϱȽ��������,����ͨ��1�ĳ�ʼ���Ϊ�͵�ƽ������������࣬ʹ�ù���ֵ��Ϊ���Դ */
    DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
                                      DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
                                      DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
	/* ���ò���/�Ƚϸ��·���Ϊ�������� */
    DL_TimerG_setCaptCompUpdateMethod(PWM_1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
	/* ����ͨ��1�Ĳ���/�Ƚ�ֵΪ500 */
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, 500, DL_TIMER_CC_1_INDEX);
	/* ʹ��PWM_1_INST��ʱ����ʱ�� */
    DL_TimerG_enableClock(PWM_1_INST);
	
    DL_TimerG_setCCPDirection(PWM_1_INST, DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT);

    /* ʹ��PWM_1_INST��ʱ����ʱ�� */
    DL_TimerG_startCounter(PWM_1_INST);
}
