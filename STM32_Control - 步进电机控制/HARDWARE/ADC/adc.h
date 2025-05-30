#ifndef ADC_H
#define ADC_H
#include "sys.h" 

void Adc3_Init(void);  // ADC3初始化
u16 Get_Adc3(u8 ch);  // 获得某个通道值
u16 Get_Adc3_Average(u8 ch,u8 times);  // 得到某个通道给定次数采样的平均值
#endif

