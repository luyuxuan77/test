#ifndef ENCODER_H
#define ENCODER_H
#include <stdbool.h>
#include <stdint.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

#define QEI_MAX_POSITION	0xFFFFFFFF	//������������ֵ

#define L	0	//���ֱ������	0-->QEI0
#define	R	1	//���ֱ������	1-->QEI1

#define L_Change_Diretion	1	//���ּ���������ı�
#define	R_Change_Diretion	1	//���ּ���������ı�

void QEI0_Init(void);
void QEI1_Init(void);

int32_t Get_QEI_Count(unsigned char num);
float Get_Motor_Speed(unsigned char num, uint16_t time);
#endif
