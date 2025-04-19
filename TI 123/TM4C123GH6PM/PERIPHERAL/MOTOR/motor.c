#include "motor.h"

/**************************************************************************
函数功能：电机初始化配置，把PA2,PA3,PA4,PA5初始化为输出口，把PB6,PB7,PB4,PB5初始化为PWM输出模式，4路通道输出8KHzPWM波
入口参数：无		PA2+PB6->Motor[0]	PA3+PB7->Motor[1]
返回  值：无		PA4+PB4->Motor[2]	PA5+PB5->Motor[3]
**************************************************************************/
void Motor_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  //使能 GPIOA 外设
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);  //使能PA2|PA3|PA4|PA5引脚
	MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);	//推挽+下拉
	
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);					//PWM时钟不分频	80M
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);			//使能 PWM0
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));		//等待完全使能
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);		//使能 GPIOB
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));		//等待完全使能
	
	MAP_GPIOPinConfigure(GPIO_PB6_M0PWM0);	//配置 PB6 引脚的备用功能 M0PWM0
    MAP_GPIOPinConfigure(GPIO_PB7_M0PWM1);	//配置 PB7 引脚的备用功能 M0PWM1
	MAP_GPIOPinConfigure(GPIO_PB4_M0PWM2);	//配置 PB4 引脚的备用功能 M0PWM2
    MAP_GPIOPinConfigure(GPIO_PB5_M0PWM3);	//配置 PB5 引脚的备用功能 M0PWM3
	
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);	//配置 PWM0 外围设备使用的引脚 PB6
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);	//配置 PWM0 外围设备使用的引脚 PB7
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);	//配置 PWM0 外围设备使用的引脚 PB4
	MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);	//配置 PWM0 外围设备使用的引脚 PB5
	
	//PWM0-7对应PWM发生器0-3，两个PWM通道对应一个PWM发生器
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);//发生器0，向下计数模式，不同步计数
	MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);//发生器1，向下计数模式，不同步计数
	
	//设置pwm频率，频率 = PWM时钟 / MOTOR_PWMARR
	MAP_PWMGenPeriodSet( PWM0_BASE, PWM_GEN_0, MOTOR_PWMARR);
	MAP_PWMGenPeriodSet( PWM0_BASE, PWM_GEN_1, MOTOR_PWMARR);
	
	//设置占空比，初始pwm占空比为0，PWMGenPeriodGet可计算出一个周期内的计数总数
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) * 0.0);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_0) * 0.0);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1) * 0.0);
	MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, MAP_PWMGenPeriodGet(PWM0_BASE, PWM_GEN_1) * 0.0);
	
	PWMOutputInvert(PWM0_BASE, PWM_OUT_0_BIT, true);	//PWM0信号反转
	PWMOutputInvert(PWM0_BASE, PWM_OUT_1_BIT, true);	//PWM1信号反转
	PWMOutputInvert(PWM0_BASE, PWM_OUT_2_BIT, true);	//PWM2信号反转
	PWMOutputInvert(PWM0_BASE, PWM_OUT_3_BIT, true);	//PWM3信号反转
	
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);	//使能输出通道0
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);	//使能输出通道1
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);	//使能输出通道2
	MAP_PWMOutputState(PWM0_BASE, PWM_OUT_3_BIT, true);	//使能输出通道3
	
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_0);	//使能发生器0
	MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_1);	//使能发生器1
}


//定义电机数据结构体变量数组
//Motor_Data[0]对应pwmout0通道
//Motor_Data[1]对应pwmout1通道
//Motor_Data[2]对应pwmout2通道
//Motor_Data[3]对应pwmout3通道
//与硬件PWM0通道对应，不可轻易更改pwm_outch
Motor_Data_TypeDef	Motor_Data[4] = {{ 0, 0, 0, 0, 0, 0 },
									 { 1, 0, 0, 0, 0, 0 },
									 { 2, 0, 0, 0, 0, 0 },
									 { 3, 0, 0, 0, 0, 0 },};
