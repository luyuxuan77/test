#ifndef ADC_H
#define ADC_H
#include "sys.h" 

void Adc3_Init(void);  // ADC3��ʼ��
u16 Get_Adc3(u8 ch);  // ���ĳ��ͨ��ֵ
u16 Get_Adc3_Average(u8 ch,u8 times);  // �õ�ĳ��ͨ����������������ƽ��ֵ
#endif

