#include "servo.h"

/******************************************************************************
�������ܣ������ʼ��������PWM1��OUT2(PA6)|OUT3(PA7)
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Servo_Init(void)
{

#if Servo_Kind == 0
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_8);				//PWMʱ��8��Ƶ	10MHz
#else
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_16);			//PWMʱ��16��Ƶ	5MHz
#endif

	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);		//ʹ�� PWM1
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));	//�ȴ���ȫʹ��
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//ʹ�� GPIOA
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));	//�ȴ���ȫʹ��
	
	MAP_GPIOPinConfigure(GPIO_PA6_M1PWM2);	//���� PA6 ���ŵı��ù��� M1PWM2
    MAP_GPIOPinConfigure(GPIO_PA7_M1PWM3);	//���� PA7 ���ŵı��ù��� M1PWM3
	
	MAP_GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6);	//���� PWM1 ��Χ�豸ʹ�õ����� PA6
	MAP_GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_7);	//���� PWM1 ��Χ�豸ʹ�õ����� PA7
	
	//PWM0-7��ӦPWM������0-3������PWMͨ����Ӧһ��PWM������
	MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);//������1�����¼���ģʽ����ͬ������
	
	//����pwmƵ�ʣ�Ƶ�� = PWMʱ�� / SERVO_PWMARR = 320Hz
	MAP_PWMGenPeriodSet( PWM1_BASE, PWM_GEN_1, SERVO_PWMARR);
	
	//����ռ�ձȣ���ʼpwmռ�ձ�Ϊ0��PWMGenPeriodGet�ɼ����һ�������ڵļ�������
	MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, MAP_PWMGenPeriodGet(PWM1_BASE, PWM_GEN_1) * 0.0);
	MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, MAP_PWMGenPeriodGet(PWM1_BASE, PWM_GEN_1) * 0.0);
	
	MAP_PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);	//ʹ�����ͨ��2
	MAP_PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);	//ʹ�����ͨ��3
	
	MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_1);	//ʹ�ܷ�����1
}

/******************************************************************************
�������ܣ����ö������
��ڲ�����servo_num�������ţ���Ӧĳ�����time������������pwm�ĸߵ�ƽʱ��(us)
����  ֵ����
*******************************************************************************/ 
void Set_Servo(unsigned char servo_num, uint16_t time)
{
	uint16_t pwm;
#if Servo_Kind == 0
	pwm = time * 10;
#else
	pwm = time * 10 / 2;
#endif
	
	if (servo_num == X_Servo)
	{
		MAP_PWMPulseWidthSet(SERVOX_PWMOUTCH, pwm);
	}
	else if (servo_num == Y_Servo)
	{
		MAP_PWMPulseWidthSet(SERVOY_PWMOUTCH, pwm);
	}
}

/******************************************************************************
�������ܣ��������ߵ�ƽʱ��time�޷�
��ڲ�����time���ߵ�ƽʱ��(us)��MinValue������޷���С�ߵ�ƽʱ�䣻MaxValue������޷����ߵ�ƽʱ��
����  ֵ���޷���ĸߵ�ƽʱ��time
*******************************************************************************/ 
int32_t Servo_InLimit(uint16_t* time, int32_t MinValue, int32_t MaxValue)
{
	if ( *time > MaxValue)
	{
		*time = MaxValue;
	}
	else if (*time < MinValue)
	{
		*time = MinValue;
	}
	return *time;
}
