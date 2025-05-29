#include "led.h"

/**************************************************************************
�������ܣ�����LED�������ú���
����˵������
��ڲ�������
����  ֵ����
��    ����LED����ΪPA0
**************************************************************************/
void led_init(void)
{
    // ��ʼ���������Ϊ��������������ࡢʹ�����������ø�����ǿ�ȡ����ø���̬
    DL_GPIO_initDigitalOutputFeatures(LED_PINCM, IOMUX_PINCM_INV_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                                  DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);

    DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0);  // ��ʼ״̬Ϊ����
    DL_GPIO_enableOutput(GPIOA, DL_GPIO_PIN_0);  // ʹ�����
}



//TIMG6λ��PD1��Դ��ʱ��Ƶ��Ϊ80M
static const DL_TimerG_ClockConfig gRGBClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 9U				//10��Ƶ
};

//TIMG8λ��PD0��Դ��ʱ��Ƶ��Ϊ40M
static const DL_TimerG_ClockConfig gRGB_BClockConfig = {
    .clockSel = DL_TIMER_CLOCK_BUSCLK,
    .divideRatio = DL_TIMER_CLOCK_DIVIDE_1,
    .prescale = 4U				//5��Ƶ
};
static const DL_TimerG_PWMConfig gRGBConfig = {
    .pwmMode = DL_TIMER_PWM_MODE_EDGE_ALIGN,
    .period = 256,
    .startTimer = DL_TIMER_START,
};


//RGB��ʼ��
void RGB_init()
{
	DL_TimerG_reset(RGB_RG_INST);
	DL_TimerG_reset(RGB_B_INST);
	DL_TimerG_enablePower(RGB_RG_INST);
	DL_TimerG_enablePower(RGB_B_INST);
	
	/* �������� */
    DL_GPIO_initPeripheralOutputFunction(GPIO_RGB_R_IOMUX,GPIO_RGB_R_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_RGB_R_PORT, GPIO_RGB_R_PIN);
    DL_GPIO_initPeripheralOutputFunction(GPIO_RGB_G_IOMUX,GPIO_RGB_G_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_RGB_G_PORT, GPIO_RGB_G_PIN);
	DL_GPIO_initPeripheralOutputFunction(GPIO_RGB_B_IOMUX,GPIO_RGB_B_IOMUX_FUNC);
    DL_GPIO_enableOutput(GPIO_RGB_B_PORT, GPIO_RGB_B_PIN);
	
	/* ��ʱ��G6ʱ������ */
    DL_TimerG_setClockConfig(
        RGB_RG_INST, (DL_TimerG_ClockConfig *) &gRGBClockConfig);
	/* ��ʱ��G8ʱ������ */
	DL_TimerG_setClockConfig(
        RGB_B_INST, (DL_TimerG_ClockConfig *) &gRGB_BClockConfig);

	/* ��ʱ��G6�������� */
    DL_TimerG_initPWMMode(
        RGB_RG_INST, (DL_TimerG_PWMConfig *) &gRGBConfig);
	/* ��ʱ��G8�������� */
	DL_TimerG_initPWMMode(
        RGB_B_INST, (DL_TimerG_PWMConfig *) &gRGBConfig);

	/* ��Ӧͨ��R���á� */
    DL_TimerG_setCaptureCompareValue(RGB_RG_INST, 1, DL_TIMER_CC_0_INDEX);		//��Ӧͨ�� 0 �Ƚ�ֵ����  256��ռ�ձ�
    DL_TimerG_setCaptureCompareOutCtl(RGB_RG_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,	//��׽�Ƚ������������
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,			//��ʼ��ƽ�ͣ��޷�ת�� �źŷ�������ֵ
		DL_TIMERG_CAPTURE_COMPARE_0_INDEX);	
    DL_TimerG_setCaptCompUpdateMethod(RGB_RG_INST,									//���ò�׽�Ƚ�Ӱ�ӼĴ������·���
	DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);		//д�� CCACT.SWFRCACT �Ĵ�����ֵ������Ч
	
	/* ��Ӧͨ��G���á� */
    DL_TimerG_setCaptureCompareValue(RGB_RG_INST, 1, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(RGB_RG_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
    DL_TimerG_setCaptCompUpdateMethod(RGB_RG_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

	/* ��Ӧͨ��B���á� */
    DL_TimerG_setCaptureCompareValue(RGB_B_INST, 1, DL_TIMER_CC_1_INDEX);
    DL_TimerG_setCaptureCompareOutCtl(RGB_B_INST, DL_TIMER_CC_OCTL_INIT_VAL_LOW,
		DL_TIMER_CC_OCTL_INV_OUT_DISABLED, DL_TIMER_CC_OCTL_SRC_FUNCVAL,
		DL_TIMERG_CAPTURE_COMPARE_1_INDEX);
    DL_TimerG_setCaptCompUpdateMethod(RGB_B_INST, DL_TIMER_CC_UPDATE_METHOD_IMMEDIATE, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);

	/* ʹ�ܶ�ʱ��G6ʱ�� */
    DL_TimerG_enableClock(RGB_RG_INST);
	/* ʹ�ܶ�ʱ��G8ʱ�� */
	DL_TimerG_enableClock(RGB_B_INST);
	
	/* ���� CCP ���� */
    DL_TimerG_setCCPDirection(RGB_RG_INST , DL_TIMER_CC0_OUTPUT | DL_TIMER_CC1_OUTPUT );
	DL_TimerG_setCCPDirection(RGB_B_INST , DL_TIMER_CC1_OUTPUT );
	
	/*������ʱ��*/
	DL_TimerG_startCounter(RGB_RG_INST);
	DL_TimerG_startCounter(RGB_B_INST);
}



//RGB����
void RGBSet(int	R,int G,int B)
{
	 DL_TimerG_setCaptureCompareValue(RGB_RG_INST, R, DL_TIMER_CC_0_INDEX);
	 DL_TimerG_setCaptureCompareValue(RGB_RG_INST, G, DL_TIMER_CC_1_INDEX);
	  DL_TimerG_setCaptureCompareValue(RGB_B_INST, B, DL_TIMER_CC_1_INDEX);
}