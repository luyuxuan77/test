#ifndef MOTOR_H
#define MOTOR_H
#include "sys.h"
#include "delay.h"

//电机数据结构体，内含电机各参数
typedef struct
{
	u8 ch_number;  // Tim8通道号
	u8 direction;  // 电机旋转方向，0:正转，1:反转
	float current;  // 电机电流
	float speed;  // 电机转速，单位r/m(转每分)
	float location;  // 电机转矩，编码器计数值
	int32_t motor_pwm;  // 电机PWM比较值。
}Motor_Data_TypeDef;  // 定义结构体用来储存电机的各个参数
extern Motor_Data_TypeDef Motor_Data[4];

#define	Motor_Drive_Direction	0  // 电机方向控制，0:正常；1:不正常

#define ROTO_RATIO		44  // 编码器线数*定时器倍频系数，即11 * 4 = 44(编码器转一圈，定时器记44个数)
#define REDUCTION_RATIO	30  // 电机减速比

#define Motor_ARR	(16800 - 1)  // TIM8的自动重装载值(此值不可小于100 - 1)
#define Motor_PSC	(8 - 1)  // TIM8的预分频系数

#define MOTOR0_DIRECTION 	PCout(2)  // Motor0方向脚
#define MOTOR1_DIRECTION 	PCout(3)  // Motor1方向脚
#define MOTOR2_DIRECTION 	PCout(4)  // Motor2方向脚
#define MOTOR3_DIRECTION 	PCout(5)  // Motor3方向脚
#define MOTOR0_CCR	TIM8->CCR1  // TIM8的CH1通道的预装载值，用于Motor0
#define MOTOR1_CCR	TIM8->CCR2  // TIM8的CH2通道的预装载值，用于Motor1
#define MOTOR2_CCR	TIM8->CCR3  // TIM8的CH3通道的预装载值，用于Motor2
#define MOTOR3_CCR	TIM8->CCR4  // TIM8的CH4通道的预装载值，用于Motor3

#define L1_Motor	Motor_Data[0]  // L1电机定义到Motor[0]结构体（软连接，可定情况修改），Motor_Data[0]为CH1通道（硬连接，非必要不修改）
#define	R1_Motor	Motor_Data[1]  // R1电机定义到Motor[1]结构体（软连接，可定情况修改），Motor_Data[1]为CH2通道（硬连接，非必要不修改）
#define	L2_Motor	Motor_Data[2]  // L2电机定义到Motor[2]结构体（软连接，可定情况修改），Motor_Data[2]为CH3通道（硬连接，非必要不修改）
#define	R2_Motor	Motor_Data[3]  // R2电机定义到Motor[3]结构体（软连接，可定情况修改），Motor_Data[3]为CH4通道（硬连接，非必要不修改）

void Motor_Init(void);
void Set_Motor(Motor_Data_TypeDef* motor_data);
void Set_MotorDutyCycle(Motor_Data_TypeDef* motor_data, float duty_cycle);
void Set_MotorPwm(Motor_Data_TypeDef* motor_data, float pwm);
int32_t Motor_InLimit( Motor_Data_TypeDef* motor_data, int32_t limit);
void motor_4(float duty_cycle1, float duty_cycle2, float duty_cycle3, float duty_cycle4);
void motor_2(float duty_cycle1, float duty_cycle2);
void step_motor_1(int vel, int acc, int time);

#endif

