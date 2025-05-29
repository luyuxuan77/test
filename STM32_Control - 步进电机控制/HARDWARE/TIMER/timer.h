#ifndef TIMER_H
#define TIMER_H
#include "sys.h"

#define	TIM6_Count_Value	TIM6->CNT  // TIM6计数值
#define	TIM7_Count_Value	TIM7->CNT  // TIM7计数值
#define	TIM9_Count_Value	TIM9->CNT  // TIM7计数值

#define TIM14_Count_Value	TIM14->CNT  // TIM14计数值

extern int Cnt,Mid_Val,Mid_Val_Bias;

void TIM6_Int_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);
void TIM9_Int_Init(u16 arr,u16 psc);
#endif

