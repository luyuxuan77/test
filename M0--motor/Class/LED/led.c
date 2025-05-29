#include "led.h"

/**************************************************************************
函数功能：板载LED引脚配置函数
函数说明：无
入口参数：无
返回  值：无
其    它：LED引脚为PA0
**************************************************************************/
void led_init(void)
{
    // 初始化输出引脚为禁用引脚输出反相、使能下拉、禁用高驱动强度、禁用高阻态
    DL_GPIO_initDigitalOutputFeatures(LED_PINCM, IOMUX_PINCM_INV_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                                  DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);

    DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0);  // 初始状态为拉高
    DL_GPIO_enableOutput(GPIOA, DL_GPIO_PIN_0);  // 使能输出
}



//TIMG6位于PD1电源域，时钟频率为80M
static const DL_TimerG_ClockConfig gRGBClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 9U				//10分频
};

//TIMG8位于PD0电源域，时钟频率为40M
static const DL_TimerG_ClockConfig gRGB_BClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 4U				//5分频
};
static const DL_TimerG_PWMConfig gRGBConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 256,
    .startTimer = DL_TIMER_START,
};


//RGB初始化
void RGB_init()
{
	DL_TimerG_reset(RGB_RG_INST);
	DL_TimerG_reset(RGB_B_INST);
	DL_TimerG_enablePower(RGB_RG_INST);
	DL_TimerG_enablePower(RGB_B_INST);
	
	/* 引脚设置 */
    DL_GPIO_initPeripheralOutputFunction(GPIO_RGB_R_IOMUX,GPIO_RGB_R_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_RGB_R_PORT, GPIO_RGB_R_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_RGB_G_IOMUX,GPIO_RGB_G_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_RGB_G_PORT, GPIO_RGB_G_PIN);
	DL_GPIO_initPeripheralOutputFunction(GPIO_RGB_B_IOMUX,GPIO_RGB_B_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_RGB_B_PORT, GPIO_RGB_B_PIN);
	
	/* 定时器G6时钟设置 */
    DL_TimerG_setClockConfig(
        RGB_RG_INST, (DL_TimerG_ClockConfig *) &gRGBClockConfig);
	/* 定时器G8时钟设置 */
	DL_TimerG_setClockConfig(
        RGB_B_INST, (DL_TimerG_ClockConfig *) &gRGB_BClockConfig);

	/* 定时器G6功能设置 */
    DL_TimerG_initPWMMode(
        RGB_RG_INST, (DL_TimerG_PWMConfig *) &gRGBConfig);
	/* 定时器G8功能设置 */
	DL_TimerG_initPWMMode(
        RGB_B_INST, (DL_TimerG_PWMConfig *) &gRGBConfig);

	/* 对应通道R设置· */
    DL_TimerG_setCaptureCompareValue(RGB_RG_INST, 1, DL_TIMER_CC_0_INDEX);		//对应通道 0 比较值设置  256满占空比
    DL_TimerG_setCaptureCompareOutCtl(RGB_RG_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,	//捕捉比较输出控制设置
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,			//起始电平低，无反转， 信号发生器数值
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);	
    DL_TimerG_setCaptCompUpdateMethod(RGB_RG_INST,									//配置捕捉比较影子寄存器更新方法
	DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);		//写入 CCACT.SWFRCACT 寄存器的值立即生效
	
	/* 对应通道G设置· */
    DL_TimerG_setCaptureCompareValue(RGB_RG_INST, 1, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(RGB_RG_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
    DL_TimerG_setCaptCompUpdateMethod(RGB_RG_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

	/* 对应通道B设置· */
    DL_TimerG_setCaptureCompareValue(RGB_B_INST, 1, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(RGB_B_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
    DL_TimerG_setCaptCompUpdateMethod(RGB_B_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

	/* 使能定时器G6时钟 */
    DL_TimerG_enableClock(RGB_RG_INST);
	/* 使能定时器G8时钟 */
	DL_TimerG_enableClock(RGB_B_INST);
	
	/* 设置 CCP 方向 */
    DL_TimerG_setCCPDirection(RGB_RG_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );
	DL_TimerG_setCCPDirection(RGB_B_INST , DL_TIMER_CC1_OUTPUT );
	
	/*启动计时器*/
	DL_TimerG_startCounter(RGB_RG_INST);
	DL_TimerG_startCounter(RGB_B_INST);
}



//RGB设置
void RGBSet(int	R,int G,int B)
{
	 DL_TimerG_setCaptureCompareValue(RGB_RG_INST, R, DL_TIMER_CC_0_INDEX);
	 DL_TimerG_setCaptureCompareValue(RGB_RG_INST, G, DL_TIMER_CC_1_INDEX);
	  DL_TimerG_setCaptureCompareValue(RGB_B_INST, B, DL_TIMER_CC_1_INDEX);
}