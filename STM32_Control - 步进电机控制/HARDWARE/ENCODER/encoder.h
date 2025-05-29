#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"	 

//���������ݽṹ�壬�ں�������������
typedef struct
{
	int tim_number;  // ��ʱ����
	int period_count;  // ���������ڸ���
	int value;  // ��ǰ��ʱ������ֵ
}Encoder_Data_TypeDef;
extern Encoder_Data_TypeDef Tim_Encoder_Data[4];

#define ENCODER_PERIOD_COUNT	4095  // ���������ڼ���ֵ

#define L_Change_Diretion	0  // ���ּ���������ı�
#define	R_Change_Diretion	1  // ���ּ���������ı�

#define L1_Encoder	Tim_Encoder_Data[3]  // L1���������嵽Tim_Encoder_Data[3]�ṹ�壨�����ӣ��ɶ�����޸ģ���Tim_Encoder_Data[3]Ϊtim5��Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	R1_Encoder	Tim_Encoder_Data[1]  // R1���������嵽Tim_Encoder_Data[1]�ṹ�壨�����ӣ��ɶ�����޸ģ���Tim_Encoder_Data[1]Ϊtim3��Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define L2_Encoder	Tim_Encoder_Data[2]  // L2���������嵽Tim_Encoder_Data[2]�ṹ�壨�����ӣ��ɶ�����޸ģ���Tim_Encoder_Data[2]Ϊtim4��Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define R2_Encoder	Tim_Encoder_Data[0]  // R2���������嵽Tim_Encoder_Data[0]�ṹ�壨�����ӣ��ɶ�����޸ģ���Tim_Encoder_Data[0]Ϊtim2��Ӳ���ӣ��Ǳ�Ҫ���޸ģ�

void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Encoder_Init_TIM5(void);

int Get_Encoder_Count(Encoder_Data_TypeDef* encoder_data);
float Get_Motor_Speed(Encoder_Data_TypeDef* encoder_data, u16 time);
#endif
