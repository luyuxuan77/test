#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

#define X_Servo	2	//舵机编号设置，2-->PWM1OUT2
#define Y_Servo	3	//舵机编号设置，3-->PWM1OUT3

#define SERVOX_PWMOUTCH		PWM1_BASE, PWM_OUT_2	//舵机X的pwm输出通道号
#define SERVOY_PWMOUTCH		PWM1_BASE, PWM_OUT_3	//舵机Y的pwm输出通道号

#define Servo_Kind	0	//0:高级舵机(300Hz)		1:普通舵机(50Hz)
#if Servo_Kind == 0
#define SERVO_PWMARR	31250	//舵机pwm的最大重装载值
#else
#define SERVO_PWMARR	100000	//舵机pwm的最大重装载值
#endif

void Servo_Init(void);
void Set_Servo(unsigned char servo_num, uint16_t time);
int32_t Servo_InLimit(uint16_t* time, int32_t MinValue, int32_t MaxValue);
#endif
