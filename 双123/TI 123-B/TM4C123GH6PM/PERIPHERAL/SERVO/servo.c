#include "servo.h"

/******************************************************************************
函数功能：舵机初始化函数，PWM1的OUT2(PA6)|OUT3(PA7)
入口参数：无
返回  值：无
*******************************************************************************/ 
void Servo_Init(void)
{

#if Servo_Kind == 0
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_8);				//PWM时钟8分频	10MHz
#else
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_16);			//PWM时钟16分频	5MHz
#endif

	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);		//使能 PWM1
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1));	//等待完全使能
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//使能 GPIOA
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));	//等待完全使能
	
	MAP_GPIOPinConfigure(GPIO_PA6_M1PWM2);	//配置 PA6 引脚的备用功能 M1PWM2
    MAP_GPIOPinConfigure(GPIO_PA7_M1PWM3);	//配置 PA7 引脚的备用功能 M1PWM3
	
	MAP_GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6);	//配置 PWM1 外围设备使用的引脚 PA6
	MAP_GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_7);	//配置 PWM1 外围设备使用的引脚 PA7
	
	//PWM0-7对应PWM发生器0-3，两个PWM通道对应一个PWM发生器
	MAP_PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);//发生器1，向下计数模式，不同步计数
	
	//设置pwm频率，频率 = PWM时钟 / SERVO_PWMARR = 320Hz
	MAP_PWMGenPeriodSet( PWM1_BASE, PWM_GEN_1, SERVO_PWMARR);
	
	//设置占空比，初始pwm占空比为0，PWMGenPeriodGet可计算出一个周期内的计数总数
	MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, MAP_PWMGenPeriodGet(PWM1_BASE, PWM_GEN_1) * 0.0);
	MAP_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, MAP_PWMGenPeriodGet(PWM1_BASE, PWM_GEN_1) * 0.0);
	
	MAP_PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);	//使能输出通道2
	MAP_PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);	//使能输出通道3
	
	MAP_PWMGenEnable(PWM1_BASE, PWM_GEN_1);	//使能发生器1
}

/******************************************************************************
函数功能：设置舵机函数
入口参数：servo_num：舵机编号，对应某舵机；time：输入给舵机的pwm的高电平时间(us)
返回  值：无
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
函数功能：舵机输入高电平时间time限幅
入口参数：time：高电平时间(us)；MinValue：舵机限幅最小高电平时间；MaxValue：舵机限幅最大高电平时间
返回  值：限幅后的高电平时间time
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
