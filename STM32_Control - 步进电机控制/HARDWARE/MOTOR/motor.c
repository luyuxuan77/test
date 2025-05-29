#include "motor.h"
#include "Emm_V5.h"

/**************************************************************************
函数功能：电机初始化配置，把PC6,PC7,PC7,PC9初始化为输出口，把TIM8初始化为PWM输出模式，4路通道输出10KHzPWM波
入口参数：无
返回  值：无
**************************************************************************/
void Motor_Init(void)
{
	RCC->AHB1ENR |= 1<<2;  // 使能GPIOC
	GPIO_Set(GPIOC, PIN2|PIN3|PIN4|PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);  // 使能IO口
	GPIO_Set(GPIOC, PIN6|PIN7|PIN8|PIN9, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);  // PWM输出口
	GPIO_AF_Set(GPIOC, 6, 3);
	GPIO_AF_Set(GPIOC, 7, 3);
	GPIO_AF_Set(GPIOC, 8, 3);
	GPIO_AF_Set(GPIOC, 9, 3);
	
	RCC->APB2ENR |= 1<<1;  // TIM8使能
	TIM8->ARR =	Motor_ARR;  // 电机功率越大，PWM频率越低
	TIM8->PSC = Motor_PSC;  // APB2ENR 168M,APB1ENR 84M
	TIM8->CR1 |= 0X01;
	TIM8->CR1 |= 1<<7;
	TIM8->BDTR |= 1<<15;  // 高级定时器输出PWM 主输出使能
	//CH1
	TIM8->CCMR1 |= 6<<4;
	TIM8->CCMR1 |= 1<<3;
	TIM8->CCER |= 1<<0;
	TIM8->CCER |= 1<<1;
	//CH2
	TIM8->CCMR1 |= 6<<12;
	TIM8->CCMR1 |= 1<<11;
	TIM8->CCER |= 1<<4;
	TIM8->CCER |= 1<<5;
	//CH3
	TIM8->CCMR2 |= 6<<4;
	TIM8->CCMR2 |= 1<<3;
	TIM8->CCER |= 1<<8;
	TIM8->CCER |= 1<<9;
	//CH4
	TIM8->CCMR2 |= 6<<12;
	TIM8->CCMR2 |= 1<<11;
	TIM8->CCER |= 1<<12;
	TIM8->CCER |= 1<<13;
	
	//使电机初始不转
	Set_MotorDutyCycle(&Motor_Data[0], 0);
	Set_MotorDutyCycle(&Motor_Data[1], 0);
	Set_MotorDutyCycle(&Motor_Data[2], 0);
	Set_MotorDutyCycle(&Motor_Data[3], 0);
}

/*定义电机数据结构体变量数组
  Motor_Data[0]对应ch1通道
  Motor_Data[1]对应ch2通道
  Motor_Data[2]对应ch3通道
  Motor_Data[3]对应ch4通道
  与硬件tim8通道对应，不可轻易更改ch_number*/
Motor_Data_TypeDef	Motor_Data[4] = {{ 1, 0, 0, 0, 0, 0 },
									 { 2, 0, 0, 0, 0, 0 },
									 { 3, 0, 0, 0, 0, 0 },
									 { 4, 0, 0, 0, 0, 0 },};

/**************************************************************************
函数功能：设置电机函数，改变电机的方向与输入的pwm比较值
入口参数：电机数据结构体变量指针，其中包括其对应电机的各种参数
返回  值：无（配置底层硬件，不可轻易更改）
**************************************************************************/
void Set_Motor(Motor_Data_TypeDef* motor_data)
{
	if (motor_data->ch_number == 1)  // 电机对应的定时器通道数判断
	{
		MOTOR0_DIRECTION = motor_data->direction;  // 电机方向脚电平转换
		MOTOR0_CCR = motor_data->motor_pwm;  // 电机pwm占空比计算
	}
	else if (motor_data->ch_number == 2)
	{
		MOTOR1_DIRECTION = motor_data->direction;
		MOTOR1_CCR = motor_data->motor_pwm;
	}
	else if (motor_data->ch_number == 3)
	{
		MOTOR2_DIRECTION = motor_data->direction;
		MOTOR2_CCR = motor_data->motor_pwm;
	}
	else if (motor_data->ch_number == 4)
	{
		MOTOR3_DIRECTION = motor_data->direction;
		MOTOR3_CCR = motor_data->motor_pwm;
	}
}

/**************************************************************************
函数功能：设置电机pwm占空比（简易控制电机转速）
入口参数：电机数据结构体变量指针（内含电机各参数），输出pwm占空比
返回  值：无
**************************************************************************/
void Set_MotorDutyCycle(Motor_Data_TypeDef* motor_data, float duty_cycle)
{
#if	Motor_Drive_Direction == 1
	duty_cycle = -duty_cycle;
#endif
	if (duty_cycle >= 0)
	{
		motor_data->direction = 0;
	}
	else
	{
		motor_data->direction = 1;
		duty_cycle = 99.99f + duty_cycle;
	}
	motor_data->motor_pwm = ((Motor_ARR + 1) / 100) * (99.99f - duty_cycle);  // 电机pwm计算
	
	Set_Motor(motor_data);  // 设置电机状态，将计算得来的电机方向及电机pwm占空比大小赋给电机，真正控制电机的函数
}

/**************************************************************************
函数功能：设置电机pwm比较值（精确控制电机转速）
入口参数：电机数据结构体变量指针（内含电机各参数），pwm比较值
返回  值：无
**************************************************************************/
void Set_MotorPwm(Motor_Data_TypeDef* motor_data, float pwm)
{
#if	Motor_Drive_Direction == 1
	pwm = -pwm;
#endif
	if (pwm >= 0)
	{
		motor_data->direction = 0;
	}
	else
	{
		motor_data->direction = 1;
		pwm = Motor_ARR + pwm;
	}
	motor_data->motor_pwm = Motor_ARR - pwm;  // 电机pwm计算
	
	Set_Motor(motor_data);  // 设置电机状态，将计算得来的电机方向及电机pwm占空比大小赋给电机，真正控制电机的函数
}

/**************************************************************************
函数功能：电机输入PWM比较值限幅
入口参数：电机数据结构体变量指针（内含电机各参数），pwm比较值幅值(-pwm~pwm)
返回  值：经限制后的pwm比较值
**************************************************************************/
int32_t Motor_InLimit(Motor_Data_TypeDef* motor_data, int32_t limit)
{
	if ( motor_data->motor_pwm > limit)
	{
		motor_data->motor_pwm = limit;
	}
	else if (motor_data->motor_pwm < -limit)
	{
		motor_data->motor_pwm = -limit;  // 赋负值
	}
	return motor_data->motor_pwm;
}

/**************************************************************************
函数功能：4路电机简易控制
入口参数：4路电机的pwm占空比
返回  值：无
**************************************************************************/
void motor_4(float duty_cycle1, float duty_cycle2, float duty_cycle3, float duty_cycle4)
{
    Set_MotorDutyCycle(&L1_Motor, duty_cycle1);  // L1
    Set_MotorDutyCycle(&R1_Motor, duty_cycle2);  // R1
    Set_MotorDutyCycle(&L2_Motor, duty_cycle3);  // L2
    Set_MotorDutyCycle(&R2_Motor, duty_cycle4);  // R2
}

/**************************************************************************
函数功能：2路电机简易控制
入口参数：2路电机的pwm占空比
返回  值：无
**************************************************************************/
void motor_2(float duty_cycle1, float duty_cycle2)
{
    Set_MotorDutyCycle(&L1_Motor, duty_cycle1);  // L1
    Set_MotorDutyCycle(&R1_Motor, duty_cycle2);  // R1
}

/**************************************************************************
函数功能：1路步进电机简易控制
入口参数：1路步进电机的旋转圈数
返回  值：无
**************************************************************************/

void step_motor_1(int vel, int acc, int time)
{
	if(vel>=0)
	{
		Emm_V5_Vel_Control(1,1,vel,acc,0);  //正数逆时针方向
		delay_us(time);
	}
	else
	{
		Emm_V5_Vel_Control(1,0,vel,acc,0);  //负数顺时针方向
		delay_us(time);
	}
   
}
