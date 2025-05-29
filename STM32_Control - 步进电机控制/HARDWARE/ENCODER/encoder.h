#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"	 

//编码器数据结构体，内含编码器各参数
typedef struct
{
	int tim_number;  // 定时器几
	int period_count;  // 满计数周期个数
	int value;  // 当前定时器计数值
}Encoder_Data_TypeDef;
extern Encoder_Data_TypeDef Tim_Encoder_Data[4];

#define ENCODER_PERIOD_COUNT	4095  // 编码器周期计数值

#define L_Change_Diretion	0  // 左轮计数器方向改变
#define	R_Change_Diretion	1  // 右轮计数器方向改变

#define L1_Encoder	Tim_Encoder_Data[3]  // L1编码器定义到Tim_Encoder_Data[3]结构体（软连接，可定情况修改），Tim_Encoder_Data[3]为tim5（硬连接，非必要不修改）
#define	R1_Encoder	Tim_Encoder_Data[1]  // R1编码器定义到Tim_Encoder_Data[1]结构体（软连接，可定情况修改），Tim_Encoder_Data[1]为tim3（硬连接，非必要不修改）
#define L2_Encoder	Tim_Encoder_Data[2]  // L2编码器定义到Tim_Encoder_Data[2]结构体（软连接，可定情况修改），Tim_Encoder_Data[2]为tim4（硬连接，非必要不修改）
#define R2_Encoder	Tim_Encoder_Data[0]  // R2编码器定义到Tim_Encoder_Data[0]结构体（软连接，可定情况修改），Tim_Encoder_Data[0]为tim2（硬连接，非必要不修改）

void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Encoder_Init_TIM5(void);

int Get_Encoder_Count(Encoder_Data_TypeDef* encoder_data);
float Get_Motor_Speed(Encoder_Data_TypeDef* encoder_data, u16 time);
#endif
