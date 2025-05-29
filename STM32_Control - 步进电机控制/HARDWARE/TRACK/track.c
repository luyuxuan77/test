#include "track.h"
#include "motor.h"
#include "usart.h"
#include "pid.h"

/**************************************************************************
函数功能：七路灰度初始化
入口参数：无
返回  值：无		//接7路灰度接外设接口板8路循迹接口，因为七路循迹接口与按键冲突
**************************************************************************/
void Line7_Init(void)
{
	RCC->AHB1ENR |= 1<<6;
	GPIO_Set(GPIOG, PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6, GPIO_MODE_IN, 0, 0, GPIO_PUPD_PD);
}

/**************************************************************************
函数功能：漫反射激光传感器循迹模块初始化
入口参数：无
返回  值：无	
**************************************************************************/
void Laser_Sensor_Init(void)
{
	RCC->AHB1ENR |= 1<<6;
	GPIO_Set(GPIOG, PIN7, GPIO_MODE_IN, 0, 0, GPIO_PUPD_PD);
}


//定义灰度参数结构体变量
Grayscale_TypeDef Grayscale_Data = { 1, 0, 0 };

/**************************************************************************
函数功能：七路灰度开环循迹，不同灰度值对应不同的电机速度
入口参数：Mode模式控制，高速、低速等等，可自定义
返回  值：无	
**************************************************************************/
void Track_Line7(u8 Mode)
{
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	if (Mode == 1)
	{
		switch (Grayscale_Data.grayscale_digital)
		{
			case 0x88:	motor_4( 30, 30, 30, 30);	break;  // 1 0001000
			
			case 0x8C:	motor_4( 30, 30, 20, 20 );	break;  // 1 0001100
			case 0x84:	motor_4( 20, 20, -10, -10 );break;  // 1 0000100
			case 0x82:	motor_4( 30, 30, 20, 20 );	break;  // 1 0000010
			
			case 0x98:	motor_4( 20, 20, 30, 30 );	break;  // 1 0011000
			case 0x90:	motor_4( -10, -10, 20, 20 );break;  // 1 0010000
			case 0xA0:	motor_4( 20, 20, 30, 30 );	break;  // 1 0100000
			default:	break;
		}
	}
	else if (Mode == 2)
	{
		switch (Grayscale_Data.grayscale_digital)
		{
			case 0x88:	motor_4( 50, 50, 50, 50 );	break;  // 1 0001000
			
			case 0x8C:	motor_4( 50, 50, 35, 35 );	break;  // 1 0001100
			case 0x84:	motor_4( 35, 35, -10, -10 );break;  // 1 0000100
			
			case 0x98:	motor_4( 35, 35, 50, 50 );	break;  // 1 0011000
			case 0x90:	motor_4( -10, -10, 35, 35 );break;  // 1 0010000
			default:	break;
		}
	}

}

/**************************************************************************
函数功能：七路灰度识别路况，不同的路况对应不同的逻辑
入口参数：无
返回  值：无	
**************************************************************************/
void Road_Condition_Recognition(void)
{
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	switch (Grayscale_Data.grayscale_digital)
	{
		case 0xFF:	;	break;  // 1 1111111
			
		case 0x8F:	;	break;  // 1 0001111
		case 0x87:	;	break;  // 1 0000111
	
		case 0xF8:	;	break;  // 1 1111000
		case 0xF0:	;	break;  // 1 1110000
		default:	break;
	}
}

/**************************************************************************
函数功能：得到七路灰度偏移量(模拟)
入口参数：无
返回  值：无	，模拟量赋值给Grayscale_Data.grayscale_offset
**************************************************************************/
int Get_Grayscale_Offset(void)
{
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	Grayscale_Data.grayscale_state = 1;
	switch (Grayscale_Data.grayscale_digital | 0xC1)
	{
		case 0xC9:	Grayscale_Data.grayscale_offset = 0;	break;  // 1 1001001
			
		case 0xCD:	Grayscale_Data.grayscale_offset = 60;	break;  // 1 1001101
		case 0xC5:	Grayscale_Data.grayscale_offset = 120;	break;  // 1 1000101
		case 0xC7:	Grayscale_Data.grayscale_offset = 180;	break;  // 1 1000111
		case 0xC3:	Grayscale_Data.grayscale_offset = 240;	break;  // 1 1000011
		
		case 0xD9:	Grayscale_Data.grayscale_offset = -60;	break;  // 1 1011001
		case 0xD1:	Grayscale_Data.grayscale_offset = -120;	break;  // 1 1010001
		case 0xF1:	Grayscale_Data.grayscale_offset = -180;	break;  // 1 1110001
		case 0xE1:	Grayscale_Data.grayscale_offset = -240;	break;  // 1 1100001
		default:	Grayscale_Data.grayscale_state = 0; break;
	}
	return Grayscale_Data.grayscale_offset;
}
//定义灰度传感器的pid参数结构体变量和p，i，d三值结构体变量数组
PID_TypeDef Grayscale_Pid;
p_i_d_Value_TypeDef PID_Value_Grayscale[3] = {{ 18.0, 0.0, 5.0 },	//预存三组pid参数，用于不同的巡线速度；
											  { 0.0, 0.0, 0.0 },
											  { 55.0, 0.0, 10.0 }};

/******************************************************************************
函数功能：灰度传感器的pid循迹（巡线速度改变时，p，i，d参数的值也要改变）
入口参数：pwm:pwm比较值，控制电机巡线速度
返回  值：无，改变输出给电机的pwm比较值
*******************************************************************************/ 
void Track_Pid(u16 pwm)
{
	Get_Grayscale_Offset();
	Pid_control(&Grayscale_Pid, Grayscale_Data.grayscale_offset, 0);

	Pid_OutLimit(&Grayscale_Pid, 7000);
	
	L1_Motor.motor_pwm = pwm + Grayscale_Pid.ActualValue;
	R1_Motor.motor_pwm = pwm - Grayscale_Pid.ActualValue;
	Motor_InLimit(&L1_Motor, 15000);
	Motor_InLimit(&R1_Motor, 15000);
	
	Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
	Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
}

//定义灰度传感器方向环pid参数结构体变量和p，i，d三值结构体变量数组
PID_TypeDef Grayscale_Direction_Pid;
p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3] = {{ 0.3, 0.0, 0.1 },	//预存三组pid参数，用于不同的巡线速度；
														{ 0.0, 0.0, 0.0 },
														{ 0.0, 0.0, 0.0 }};

/******************************************************************************
函数功能：灰度传感器做循迹方向环（巡线速度改变时，p，i，d参数的值也要改变）
入口参数：speed:巡线速度
返回  值：无，改变电机速度环目标值
*******************************************************************************/ 
void Track_Direction_Control(float speed)
{
	Get_Grayscale_Offset();
	
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 100);
	
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue;
}
