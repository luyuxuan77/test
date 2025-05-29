#include "adc.h"
#include "delay.h"

//��ʼ��ADC3
void Adc3_Init(void)
{
	//�ȳ�ʼ��IO��
 	RCC->APB2ENR |= 1<<10;  // ʹ��ADC3ʱ�� 
	RCC->APB2RSTR |= 1<<8;  // ADCs��λ
	RCC->APB2RSTR &= ~(1<<8);  // ��λ����
	ADC->CCR = 3<<16;  // ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz
 	
	ADC3->CR1 = 0;  // CR1��������
	ADC3->CR2 = 0;  // CR2��������
	ADC3->CR1 |= 0<<24;  // 12λģʽ
	ADC3->CR1 |= 0<<8;  // ��ɨ��ģʽ	
	
	ADC3->CR2 &= ~(1<<1);  // ����ת��ģʽ
 	ADC3->CR2 &= ~(1<<11);  // �Ҷ���	
	ADC3->CR2 |= 0<<28;  // �������
	
	ADC3->SQR1 &= ~(0XF<<20);
	ADC3->SQR1 |= 0<<20;  // 1��ת���ڹ��������� Ҳ����ֻת����������1
	//����ͨ��5�Ĳ���ʱ��
	ADC3->SMPR2 &= ~(7<<(3*5));  // ͨ��5����ʱ�����
 	ADC3->SMPR2 |= 7<<(3*5);  // ͨ��5  480������,��߲���ʱ�������߾�ȷ��
 	ADC3->CR2 |= 1<<0;  // ����ADת����
}

//���ADCֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc3(u8 ch)
{
	//����ת������	  		 
	ADC3->SQR3 &= 0XFFFFFFE0;  // ��������1 ͨ��ch
	ADC3->SQR3 |= ch;		  			    
	ADC3->CR2 |= 1<<30;  // ��������ת��ͨ�� 
	while(!(ADC3->SR&1<<1));  // �ȴ�ת������	 	   
	return ADC3->DR;  // ����adcֵ	
}

//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc3_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t = 0; t < times; t++)
	{
		temp_val += Get_Adc3(ch);
		delay_ms(5);
	}
	return temp_val/times;
}
