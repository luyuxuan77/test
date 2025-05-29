#include "tim_g8_pwm.h"

/*
 * 定时器时钟配置源自系统时钟 / 1 (40000000 Hz)
 * 定时器时钟频率计算公式如下：
 *   timerClkFreq = (定时器时钟源 / (定时器时钟分频比 * (定时器时钟预分频值 + 1)))
 *   例如：4000000 Hz = 40000000 Hz / (1 * (9 + 1))
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
	/* 复位PWM_1_INST定时器 */
    DL_TimerG_reset(PWM_1_INST);
	/* PWM_1_INST定时器上电使能 */
    DL_TimerG_enablePower(PWM_1_INST);
	/* 设置GPIO_PWM_1_C0，GPIO_PWM_1_C1作为PWM输出引脚 */
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C0_IOMUX, GPIO_PWM_1_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C0_PORT, GPIO_PWM_1_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C1_IOMUX, GPIO_PWM_1_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C1_PORT, GPIO_PWM_1_C1_PIN);
	/* 配置PWM_1_INST定时器时钟 */
    DL_TimerG_setClockConfig(
        PWM_1_INST, (DL_TimerG_ClockConfig *)&gPWM_1ClockConfig);
	/* 初始化PWM_1_INST定时器为PWM模式 */
    DL_TimerG_initPWMMode(
        PWM_1_INST, (DL_TimerG_PWMConfig *)&gPWM_1Config);
	/* 设置捕获/比较比较输出控制,设置通道0的初始输出为低电平，禁用输出反相，使用功能值作为输出源 */
    DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
                                      DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
                                      DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	/* 设置捕获/比较更新方法为立即更新 */
    DL_TimerG_setCaptCompUpdateMethod(PWM_1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	/* 设置通道0的捕获/比较值为800 */
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, 800, DL_TIMER_CC_0_INDEX);

	/* 设置捕获/比较比较输出控制,设置通道1的初始输出为低电平，禁用输出反相，使用功能值作为输出源 */
    DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
                                      DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
                                      DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
	/* 设置捕获/比较更新方法为立即更新 */
    DL_TimerG_setCaptCompUpdateMethod(PWM_1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
	/* 设置通道1的捕获/比较值为500 */
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, 500, DL_TIMER_CC_1_INDEX);
	/* 使能PWM_1_INST定时器的时钟 */
    DL_TimerG_enableClock(PWM_1_INST);
	
    DL_TimerG_setCCPDirection(PWM_1_INST, DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT);

    /* 使能PWM_1_INST定时器的时钟 */
    DL_TimerG_startCounter(PWM_1_INST);
}
