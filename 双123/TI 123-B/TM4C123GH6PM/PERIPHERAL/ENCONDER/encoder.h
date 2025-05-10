#ifndef ENCODER_H
#define ENCODER_H
#include <stdbool.h>
#include <stdint.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

#define QEI_MAX_POSITION	0xFFFFFFFF	//编码器最大计数值

#define L	0	//左轮编号设置	0-->QEI0
#define	R	1	//右轮编号设置	1-->QEI1

#define L_Change_Diretion	1	//左轮计数器方向改变
#define	R_Change_Diretion	1	//右轮计数器方向改变

void QEI0_Init(void);
void QEI1_Init(void);

int32_t Get_QEI_Count(unsigned char num);
float Get_Motor_Speed(unsigned char num, uint16_t time);
#endif
