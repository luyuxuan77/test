#include "motor.h"

/**************************************************************************
�������ܣ������ʼ�����ã���PA2,PA3,PA4,PA5��ʼ��Ϊ����ڣ���PB6,PB7,PB4,PB5��ʼ��ΪPWM���ģʽ��4·ͨ�����8KHzPWM��
��ڲ�������		PA2+PB6->Motor[0]	PA3+PB7->Motor[1]
����  ֵ����		PA4+PB4->Motor[2]	PA5+PB5->Motor[3]
**************************************************************************/
void Motor_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  //ʹ�� GPIOA ����
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);  //ʹ��PA2|PA3|PA4|PA5����
	MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);	//����+����
	
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);					//PWMʱ�Ӳ���Ƶ	80M
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);			//ʹ�� PWM0
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));		//�ȴ���ȫʹ��
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);		//ʹ�� GPIOB
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));		//�ȴ���ȫʹ��
	
	MAP_GPIOPinConfigure(GPIO_PB6_M0PWM0);	//���� PB6 ���ŵı��ù��� M0PWM0
    MAP_GPIOPinConfigure(GPIO_PB7_M0PWM1);	//���� PB7 ���ŵı��ù��� M0PWM1
	MAP_GPIOPinConfigure(GPIO_PB4_M0PWM2);	//���� PB4 ���ŵı��ù��� M0PWM2
    MAP_GPIOPinConfigure(GPIO_PB5_M0PWM3);	//���� PB5 ���ŵı��ù��� M0PWM3
	
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);	//���� PWM0 ��Χ�豸ʹ�õ����� PB6
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);	//���� PWM0 ��Χ�豸ʹ�õ����� PB7
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);	//���� PWM0 ��Χ�豸ʹ�õ����� PB4
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);	//���� PWM0 ��Χ�豸ʹ�õ����� PB5
	
	//PWM0-7��ӦPWM������0-3������PWMͨ����Ӧһ��PWM������
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);//������0�����¼���ģʽ����ͬ������
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);//������1�����¼���ģʽ����ͬ������
	
	//����pwmƵ�ʣ�Ƶ�� = PWMʱ�� / MOTOR_PWMARR
	MAP_PWMGenPeriodSet( PWM0_BASE, PWM_GEN_0, MOTOR_PWMARR);
	MAP_PWMGenPeriodSet( PWM0_BASE, PWM_GEN_1, MOTOR_PWMARR);
	
	//����ռ�ձȣ���ʼpwmռ�ձ�Ϊ0��PWMGenPeriodGet�ɼ����һ�������ڵļ�������
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) * 0.0);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) * 0.0);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1) * 0.0);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1) * 0.0);
	
	PWMOutputInvert(PWM0_BASE, PWM_OUT_0_BIT, true);	//PWM0�źŷ�ת
	PWMOutputInvert(PWM0_BASE, PWM_OUT_1_BIT, true);	//PWM1�źŷ�ת
	PWMOutputInvert(PWM0_BASE, PWM_OUT_2_BIT, true);	//PWM2�źŷ�ת
	PWMOutputInvert(PWM0_BASE, PWM_OUT_3_BIT, true);	//PWM3�źŷ�ת
	
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);	//ʹ�����ͨ��0
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);	//ʹ�����ͨ��1
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);	//ʹ�����ͨ��2
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);	//ʹ�����ͨ��3
	
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_0);	//ʹ�ܷ�����0
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_1);	//ʹ�ܷ�����1
}


//���������ݽṹ���������
//Motor_Data[0]��Ӧpwmout0ͨ��
//Motor_Data[1]��Ӧpwmout1ͨ��
//Motor_Data[2]��Ӧpwmout2ͨ��
//Motor_Data[3]��Ӧpwmout3ͨ��
//��Ӳ��PWM0ͨ����Ӧ���������׸���pwm_outch
Motor_Data_TypeDef	Motor_Data[4] = {{ 0, 0, 0, 0, 0, 0 },
									 { 1, 0, 0, 0, 0, 0 },
									 { 2, 0, 0, 0, 0, 0 },
									 { 3, 0, 0, 0, 0, 0 },};
