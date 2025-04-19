#ifndef	MOTOR_H
#define	MOTOR_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

//电机数据结构体，内含电机各参数
typedef struct
{
	unsigned char pwm_outch;	//PWM0通道号
	unsigned char direction;	//电机旋转方向，0:正转，1:反转
	float current;				//电机电流
	float speed;				//电机转速，单位r/m(转每分)
	float location;				//电机转矩，编码器计数值
	int32_t motor_pwm;			//电机PWM比较值。
}Motor_Data_TypeDef;			//定义结构体用来储存电机的各个参数
extern Motor_Data_TypeDef Motor_Data[4];

#define ROTO_RATIO		44	//编码器线数*定时器倍频系数，即11 * 4 = 44(编码器转一圈，定时器记44个数)
#define REDUCTION_RATIO	30	//电机减速比

#define	MOTOR_PWMARR	(10000 - 1)	//PWM的自动重装载值
#define MOTOR_PWMCLOCK	80000000	//PWM时钟

#define MOTOR0_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2)	//motor0方向脚拉高
#define MOTOR0_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0)			//motor0方向脚拉低
#define MOTOR1_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3)	//motor1方向脚拉高
#define MOTOR1_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0)			//motor1方向脚拉低
#define MOTOR2_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4)	//motor2方向脚拉高
#define MOTOR2_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0)			//motor2方向脚拉低
#define MOTOR3_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5)	//motor3方向脚拉高
#define MOTOR3_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0)			//motor3方向脚拉低

#define	MOTOR0_PWMOUTCH		PWM0_BASE, PWM_OUT_0	//motor0的pwm输出通道
#define	MOTOR1_PWMOUTCH		PWM0_BASE, PWM_OUT_1	//motor1的pwm输出通道
#define	MOTOR2_PWMOUTCH		PWM0_BASE, PWM_OUT_2	//motor2的pwm输出通道
#define	MOTOR3_PWMOUTCH		PWM0_BASE, PWM_OUT_3	//motor3的pwm输出通道

#define L1_Motor	Motor_Data[0]	//L1电机定义到Motor[0]结构体（软连接，可定情况修改），Motor_Data[0]为PWM0_OUT0通道（硬连接，非必要不修改）
#define	R1_Motor	Motor_Data[1]	//R1电机定义到Motor[1]结构体（软连接，可定情况修改），Motor_Data[1]为PWM0_OUT1通道（硬连接，非必要不修改）
#define	L2_Motor	Motor_Data[2]	//L2电机定义到Motor[2]结构体（软连接，可定情况修改），Motor_Data[2]为PWM0_OUT2通道（硬连接，非必要不修改）
#define	R2_Motor	Motor_Data[3]	//R2电机定义到Motor[3]结构体（软连接，可定情况修改），Motor_Data[3]为PWM0_OUT3通道（硬连接，非必要不修改）

void Motor_Init(void);
void Set_Motor(Motor_Data_TypeDef* motor_data);
void Set_MotorDutyCycle(Motor_Data_TypeDef* motor_data, float duty_cycle);
void Set_MotorPwm(Motor_Data_TypeDef* motor_data, float pwm);
int32_t Motor_InLimit( Motor_Data_TypeDef* motor_data, int32_t limit);
void motor_4_speed(float duty_cycle1, float duty_cycle2, float duty_cycle3, float duty_cycle4);
void motor_2(float duty_cycle1, float duty_cycle2);
#endif

