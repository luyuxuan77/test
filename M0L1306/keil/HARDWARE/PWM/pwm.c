#include "pwm.h"
#include "bsp.h"

/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_0ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerG_PWMConfig gPWM_0Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period = 2000,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_START,
};

/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_1ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerG_PWMConfig gPWM_1Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period = 2000,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_START,
};

/*
 * Timer clock configuration to be sourced by  / 1 (32000000 Hz)
 * timerClkFreq = (timerClkSrc / (timerClkDivRatio * (timerClkPrescale + 1)))
 *   32000000 Hz = 32000000 Hz / (1 * (0 + 1))
 */
static const DL_TimerG_ClockConfig gPWM_2ClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 0U
};

static const DL_TimerG_PWMConfig gPWM_2Config = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN_UP,
    .period = 2000,
    .isTimerWithFourCC = false,
    .startTimer = DL_TIMER_START,
};

void pwm_init(void)
{
	/* ��λPWM_1_INST��ʱ�� */
    DL_GPIO_reset(GPIOA);
    DL_TimerG_reset(PWM_0_INST);
    DL_TimerG_reset(PWM_1_INST);
    DL_TimerG_reset(PWM_2_INST);
	/* PWM_1_INST��ʱ���ϵ�ʹ�� */
	DL_GPIO_enablePower(GPIOA);
    DL_TimerG_enablePower(PWM_0_INST);
    DL_TimerG_enablePower(PWM_1_INST);
    DL_TimerG_enablePower(PWM_2_INST);
	/* ����GPIO_PWM_1_C0��GPIO_PWM_1_C1��ΪPWM������� */
	DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C0_IOMUX,GPIO_PWM_0_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C0_PORT, GPIO_PWM_0_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_0_C1_IOMUX,GPIO_PWM_0_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_0_C1_PORT, GPIO_PWM_0_C1_PIN);
	
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C0_IOMUX,GPIO_PWM_1_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C0_PORT, GPIO_PWM_1_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_1_C1_IOMUX,GPIO_PWM_1_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_1_C1_PORT, GPIO_PWM_1_C1_PIN);
	
	DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_2_C0_IOMUX,GPIO_PWM_2_C0_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_2_C0_PORT, GPIO_PWM_2_C0_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_PWM_2_C1_IOMUX,GPIO_PWM_2_C1_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_PWM_2_C1_PORT, GPIO_PWM_2_C1_PIN);
	/* ����PWM_1_INST��ʱ��ʱ�� */
    DL_TimerG_setClockConfig(
        PWM_0_INST, (DL_TimerG_ClockConfig *) &gPWM_0ClockConfig);
		
	DL_TimerG_setClockConfig(
        PWM_1_INST, (DL_TimerG_ClockConfig *) &gPWM_1ClockConfig);
		
	DL_TimerG_setClockConfig(
        PWM_2_INST, (DL_TimerG_ClockConfig *) &gPWM_2ClockConfig);
	/* ��ʼ��PWM_1_INST��ʱ��ΪPWMģʽ */
    DL_TimerG_initPWMMode(
        PWM_0_INST, (DL_TimerG_PWMConfig *) &gPWM_0Config);
		
	DL_TimerG_initPWMMode(
        PWM_1_INST, (DL_TimerG_PWMConfig *) &gPWM_1Config);
		
	DL_TimerG_initPWMMode(
        PWM_2_INST, (DL_TimerG_PWMConfig *) &gPWM_2Config);
	/* ���ò���/�ȽϱȽ��������,����ͨ��0/1/2�ĳ�ʼ���Ϊ�͵�ƽ������������࣬ʹ�ù���ֵ��Ϊ���Դ */
		DL_TimerG_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
		DL_TimerG_setCaptureCompareOutCtl(PWM_0_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
		
		DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
		DL_TimerG_setCaptureCompareOutCtl(PWM_1_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
		
		DL_TimerG_setCaptureCompareOutCtl(PWM_2_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
		DL_TimerG_setCaptureCompareOutCtl(PWM_2_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
		/* ���ò���/�Ƚϸ��·���Ϊ�������� */
		DL_TimerG_setCaptCompUpdateMethod(PWM_0_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

		DL_TimerG_setCaptCompUpdateMethod(PWM_1_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

		DL_TimerG_setCaptCompUpdateMethod(PWM_2_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
		/* ����ͨ��0/1/2�Ĳ���/�Ƚ�ֵΪ999 */
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_0_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_1_INDEX);

		
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_0_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_1_INDEX);

		
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_0_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_1_INDEX);

		/* ʹ��PWM_1_INST��ʱ����ʱ�� */
		DL_TimerG_enableClock(PWM_0_INST);
		DL_TimerG_setCCPDirection(PWM_0_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );

		DL_TimerG_enableClock(PWM_1_INST);
		DL_TimerG_setCCPDirection(PWM_1_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );

		DL_TimerG_enableClock(PWM_2_INST);
		DL_TimerG_setCCPDirection(PWM_2_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );

		/* ʹ��PWM_1/2/3_INST��ʱ����ʱ�� */
		DL_TimerG_enableClock(PWM_0_INST);
		DL_TimerG_enableClock(PWM_1_INST);
		DL_TimerG_enableClock(PWM_2_INST);

}

void AH_BL (int speed)
{
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed, DL_TIMER_CC_0_INDEX);
		
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_0_INDEX);
}
void AH_CL (int speed)
{
			
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_0_INDEX);

}
void BH_CL (int speed)
{
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, speed, DL_TIMER_CC_0_INDEX);
		
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_0_INDEX);
}
void BH_AL (int speed)
{
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, 0, DL_TIMER_CC_0_INDEX);
		
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, speed, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_0_INDEX);
}
void CH_AL (int speed)
{
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, speed, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_0_INST,0 , DL_TIMER_CC_0_INDEX);
		

}
void CH_BL (int speed)
{
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMER_CC_0_INDEX);
	
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_2_INST, speed, DL_TIMER_CC_0_INDEX);
		
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, speed, DL_TIMER_CC_1_INDEX);
		DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, DL_TIMER_CC_0_INDEX);
}
int NUM;
int ms=8000;
extern int int_status;
void motor(void)
{    
	for(int i=500;i>1;i--)
	{
		switch(NUM)
		{
			case 0:
			AH_BL(1200);
			break;
			case 1:
			AH_CL(1200);
			break;
			case 2:
			BH_CL(1200);
			break;
			case 3:
			BH_AL(1200);
			break;
			case 4:
			CH_AL(1200);
			break;
			case 5:
			CH_BL(1200);
			break;
		}
	if(NUM<5)
	{
		NUM++;
	}
	else
	{
		NUM = 0;
	}
	if(ms<8000)
	{
	ms=ms;
	}
	else
	{
	ms=(ms*14)/15;
	}
	delay_us(ms);
	}
	int_status = 1;
}