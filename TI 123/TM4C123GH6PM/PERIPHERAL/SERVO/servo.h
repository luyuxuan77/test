#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

#define X_Servo	2	//���������ã�2-->PWM1OUT2
#define Y_Servo	3	//���������ã�3-->PWM1OUT3

#define SERVOX_PWMOUTCH		PWM1_BASE, PWM_OUT_2	//���X��pwm���ͨ����
#define SERVOY_PWMOUTCH		PWM1_BASE, PWM_OUT_3	//���Y��pwm���ͨ����

#define Servo_Kind	0	//0:�߼����(300Hz)		1:��ͨ���(50Hz)
#if Servo_Kind == 0
#define SERVO_PWMARR	31250	//���pwm�������װ��ֵ
#else
#define SERVO_PWMARR	100000	//���pwm�������װ��ֵ
#endif

void Servo_Init(void);
void Set_Servo(unsigned char servo_num, uint16_t time);
int32_t Servo_InLimit(uint16_t* time, int32_t MinValue, int32_t MaxValue);
#endif
