#include "bsp.h"

/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerA_ClockConfig gPWM_0ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerA_PWMConfig gPWM_0Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period = 3200,
    .isTimerWithFourCC = true,
    .startTimer = DL_TIMER_STOP,
};

void tima0_pwm_init(void)
{
	DL_TimerA_reset(PWM_0_INST);
	DL_TimerA_enablePower(PWM_0_INST);
	
	/* 引脚设置 */
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C0_IOMUX,GPIO_PWM_0_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C0_PORT, GPIO_PWM_0_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C1_IOMUX,GPIO_PWM_0_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C1_PORT, GPIO_PWM_0_C1_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C2_IOMUX,GPIO_PWM_0_C2_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C2_PORT, GPIO_PWM_0_C2_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C3_IOMUX,GPIO_PWM_0_C3_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C3_PORT, GPIO_PWM_0_C3_PIN);
	
	/* 定时器A0时钟设置 */
    DL_TimerA_setClockConfig(
        PWM_0_INST, (DL_TimerA_ClockConfig *) &gPWM_0ClockConfig);

	/* 定时器A0功能设置 */
    DL_TimerA_initPWMMode(
        PWM_0_INST, (DL_TimerA_PWMConfig *) &gPWM_0Config);

	/* 对应通道 0 设置· */
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_0_INDEX);		//对应通道 0 比较值设置  3200满占空比
    DL_TimerA_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,	//捕捉比较输出控制设置
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,			//起始电平低，无反转， 信号发生器数值
		DL_TIMERA_CAPTURE_COMPARE_0_INDEX);	
    DL_TimerA_setCaptCompUpdateMethod(PWM_0_INST,									//配置捕捉比较影子寄存器更新方法
	DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_0_INDEX);		//写入 CCACT.SWFRCACT 寄存器的值立即生效
	
	/* 对应通道 1 设置· */
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_1_INDEX);
    DL_TimerA_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_1_INDEX);
    DL_TimerA_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_1_INDEX);

	/* 对应通道 2 设置· */
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_2_INDEX);
    DL_TimerA_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_ENABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_2_INDEX);
    DL_TimerA_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_2_INDEX);

	/* 对应通道 3 设置· */
    DL_TimerA_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_3_INDEX);
    DL_TimerA_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_ENABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERA_CAPTURE_COMPARE_3_INDEX);
    DL_TimerA_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERA_CAPTURE_COMPARE_3_INDEX);

	/* 使能定时器A0时钟 */
    DL_TimerA_enableClock(PWM_0_INST);
	
	/* 设置 CCP 方向 */
    DL_TimerA_setCCPDirection(PWM_0_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT | DL_TIMER_CC2_OUTPUT | DL_TIMER_CC3_OUTPUT );
	
	/*启动计时器*/
	DL_TimerA_startCounter(PWM_0_INST);
	
}


