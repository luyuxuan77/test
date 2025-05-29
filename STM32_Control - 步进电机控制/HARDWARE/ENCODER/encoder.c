#include "encoder.h"
#include "motor.h"

/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	RCC->APB1ENR |= 1<<0;  // TIM2ʱ��ʹ��
	
	/* �Ѷ�ʱ����ʼ��Ϊ������ģʽ */ 
	TIM2->PSC = 0x0;  // Ԥ��Ƶ��
	TIM2->ARR = ENCODER_PERIOD_COUNT;  // �趨�������Զ���װֵ
	TIM2->CCMR1 |= 1<<0;  // ����ģʽ��IC1FP1ӳ�䵽TI1��
	TIM2->CCMR1 |= 1<<8;  // ����ģʽ��IC2FP2ӳ�䵽TI2��
	TIM2->CCER &= 0<<1;  // IC1������
	TIM2->CCER &= 0<<5;  // IC2������
	TIM2->SMCR |= 3<<0;  // SMS='011' ���е�������������غ��½�����Ч
	TIM2->CR1 |= 0x01;  // CEN=1��ʹ�ܶ�ʱ��
	
	RCC->AHB1ENR |= 1<<0;  // ʹ��PORTAʱ��
	RCC->AHB1ENR |= 1<<1;  // ʹ��PORTBʱ��
	
	GPIO_Set(GPIOA, PIN15, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_Set(GPIOB, PIN3, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOA, 15, 1);
	GPIO_AF_Set(GPIOB, 3, 1);
	
	MY_NVIC_Init(2, 3, TIM2_IRQn, 2);  // �жϷ���������ռ���ȼ�2����Ӧ���ȼ�3
	TIM2->SR &= ~(1<<0);  // ��������жϱ�־
	TIM2->CNT = 0;  // ʹ�ܸ����ж�ǰ�������ֵ������������������
	TIM2->DIER |= 1<<0;  // ʹ�ܸ����ж�
}

/**************************************************************************
�������ܣ���TIM3��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	RCC->APB1ENR |= 1<<1;  // TIM3ʱ��ʹ��
	
	/* �Ѷ�ʱ����ʼ��Ϊ������ģʽ */ 
	TIM3->PSC = 0x0;  // Ԥ��Ƶ��
	TIM3->ARR = ENCODER_PERIOD_COUNT;  // �趨�������Զ���װֵ
	TIM3->CCMR1 |= 1<<0;  // ����ģʽ��IC1FP1ӳ�䵽TI1��
	TIM3->CCMR1 |= 1<<8;  // ����ģʽ��IC2FP2ӳ�䵽TI2��
	TIM3->CCER |= 0<<1;  // IC1������
	TIM3->CCER |= 0<<5;  // IC2������
	TIM3->SMCR |= 3<<0;  // SMS='011' ���е�������������غ��½�����Ч
	TIM3->CR1 |= 0x01;  // CEN=1��ʹ�ܶ�ʱ��
	
	RCC->AHB1ENR|=1<<1;  // ʹ��PORTBʱ��
	GPIO_Set(GPIOB, PIN4|PIN5, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOB, 4, 2);
	GPIO_AF_Set(GPIOB, 5, 2);
	
	MY_NVIC_Init(2, 3, TIM3_IRQn, 2);  // �жϷ���������ռ���ȼ�2����Ӧ���ȼ�3
	TIM3->SR &= ~(1<<0);  // ��������жϱ�־
	TIM3->CNT = 0;  // ʹ�ܸ����ж�ǰ�������ֵ������������������
	TIM3->DIER |= 1<<0;  // ʹ�ܸ����ж�
}
/**************************************************************************
�������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	RCC->APB1ENR |= 1<<2;  // TIM4ʱ��ʹ��
	
	/* �Ѷ�ʱ����ʼ��Ϊ������ģʽ */ 
	TIM4->PSC = 0x0;//Ԥ��Ƶ��
	TIM4->ARR = ENCODER_PERIOD_COUNT;  // �趨�������Զ���װֵ
	TIM4->CCMR1 |= 1<<0;  // ����ģʽ��IC1FP1ӳ�䵽TI1��
	TIM4->CCMR1 |= 1<<8;  // ����ģʽ��IC2FP2ӳ�䵽TI2��
	TIM4->CCER |= 0<<1;  // IC1������
	TIM4->CCER |= 0<<5;  // IC2������
	TIM4->SMCR |= 3<<0;  // SMS='011' ���е�������������غ��½�����Ч
	TIM4->CR1 |= 0x01;  // CEN=1��ʹ�ܶ�ʱ��
	
	RCC->AHB1ENR |= 1<<1;  // ʹ��PORTBʱ��
	GPIO_Set(GPIOB, PIN6|PIN7, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOB, 6, 2);
	GPIO_AF_Set(GPIOB, 7, 2);
	
	MY_NVIC_Init(2, 3, TIM4_IRQn, 2);  // �жϷ���������ռ���ȼ�2����Ӧ���ȼ�3
	TIM4->SR &= ~(1<<0);  // ��������жϱ�־
	TIM4->CNT = 0;  // ʹ�ܸ����ж�ǰ�������ֵ������������������
	TIM4->DIER |= 1<<0;  // ʹ�ܸ����ж�
}

/**************************************************************************
�������ܣ���TIM5��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM5(void)
{
	RCC->APB1ENR |= 1<<3;  // TIM5ʱ��ʹ��
	
	/* �Ѷ�ʱ����ʼ��Ϊ������ģʽ */ 
	TIM5->PSC = 0x0;  // Ԥ��Ƶ��
	TIM5->ARR = ENCODER_PERIOD_COUNT;  // �趨�������Զ���װֵ
	TIM5->CCMR1 |= 1<<0;  // ����ģʽ��IC1FP1ӳ�䵽TI1��
	TIM5->CCMR1 |= 1<<8;  // ����ģʽ��IC2FP2ӳ�䵽TI2��
	TIM5->CCER |= 0<<1;  // IC1������
	TIM5->CCER |= 0<<5;  // IC2������
	TIM5->SMCR |= 3<<0;  // SMS='011' ���е�������������غ��½�����Ч
	TIM5->CR1 |= 0x01;  // CEN=1��ʹ�ܶ�ʱ��
	
	RCC->AHB1ENR |= 1<<0;  // ʹ��PORTAʱ��
	GPIO_Set(GPIOA, PIN0|PIN1, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOA, 0, 2);
	GPIO_AF_Set(GPIOA, 1, 2);
	
	MY_NVIC_Init(2, 3, TIM5_IRQn, 2);  // �жϷ���������ռ���ȼ�2����Ӧ���ȼ�3
	TIM5->SR &= ~(1<<0);  // ��������жϱ�־
	TIM5->CNT = 0;  // ʹ�ܸ����ж�ǰ�������ֵ������������������
	TIM5->DIER |= 1<<0;  // ʹ�ܸ����ж�
}

/*������������ݽṹ���������
  Tim_Encoder_Data[0]��Ӧtim2
  Tim_Encoder_Data[1]��Ӧtim3
  Tim_Encoder_Data[2]��Ӧtim4
  Tim_Encoder_Data[3]��Ӧtim5
  ��Ӳ��tim��ʱ����Ӧ���������׸���tim_number*/
Encoder_Data_TypeDef Tim_Encoder_Data[4] = { {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 0} };

/**************************************************************************
�������ܣ���ʱ��2�жϷ���������ʱ��2����ֵ���½����жϣ�Tim_Encoder_Data[0]�������ڸı�
��ڲ������ޣ��������ݰ�����Ӳ�����ӣ�Ӳ���ӣ����������׸��ģ�
����  ֵ����
**************************************************************************/
void TIM2_IRQHandler(void)
{
	if (TIM2->SR & 0x0001)  // �ж��Ƿ��������ж�
	{
		if (TIM2->CR1 & 0x0010)  // �������ݼ�����
		{
			Tim_Encoder_Data[0].period_count--;  // ��������--
		}
		else  // ��������������
		{
			Tim_Encoder_Data[0].period_count++;  // ��������++
		}
		TIM2->SR &= ~(1<<0);  // ����жϱ�־λ
	}

}
/**************************************************************************
�������ܣ���ʱ��3�жϷ���������ʱ��3����ֵ���½����жϣ�Tim_Encoder_Data[1]�������ڸı�
��ڲ������ޣ��������ݰ�����Ӳ�����ӣ�Ӳ���ӣ����������׸��ģ�
����  ֵ����
**************************************************************************/
void TIM3_IRQHandler(void)
{
	if (TIM3->SR & 0x0001)  // �ж��Ƿ��������ж�
	{
		if (TIM3->CR1 & 0x0010)  // �������ݼ�����
		{
			Tim_Encoder_Data[1].period_count--;  // ��������--
		}
		else  // ��������������
		{
			Tim_Encoder_Data[1].period_count++;  // ��������++
		}
		TIM3->SR &= ~(1<<0);  // ����жϱ�־λ
	}

}
/**************************************************************************
�������ܣ���ʱ��4�жϷ���������ʱ��4����ֵ���½����жϣ�Tim_Encoder_Data[2]�������ڸı�
��ڲ������ޣ��������ݰ�����Ӳ�����ӣ�Ӳ���ӣ����������׸��ģ�
����  ֵ����
**************************************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM4->SR & 0x0001)  // �ж��Ƿ��������ж�
	{
		if (TIM4->CR1 & 0x0010)  // �������ݼ�����
		{
			Tim_Encoder_Data[2].period_count--;  // ��������--
		}
		else  // ��������������
		{
			Tim_Encoder_Data[2].period_count++;  // ��������++
		}
		TIM4->SR &= ~(1<<0);  // ����жϱ�־λ
	}

}
/**************************************************************************
�������ܣ���ʱ��5�жϷ���������ʱ��5����ֵ���½����жϣ�Tim_Encoder_Data[3]�������ڸı�
��ڲ������ޣ��������ݰ�����Ӳ�����ӣ�Ӳ���ӣ����������׸��ģ�
����  ֵ����
**************************************************************************/
void TIM5_IRQHandler(void)
{
	if (TIM5->SR & 0x0001)  // �ж��Ƿ��������ж�
	{
		if (TIM5->CR1 & 0x0010)  // �������ݼ�����
		{
			Tim_Encoder_Data[3].period_count--;  // ��������--
		}
		else  // ��������������
		{
			Tim_Encoder_Data[3].period_count++;  // ��������++
		}
		TIM5->SR &= ~(1<<0);  // ����жϱ�־λ
	}

}

/**************************************************************************
�������ܣ��õ�����������ֵ���������ݰ�����Ӳ�����ӣ�Ӳ���ӣ����������׸��ģ�
��ڲ��������������ݽṹ�����ָ�룬���а������Ӧ�������ĸ��ֲ���
����  ֵ��������������Լ���ֵ
**************************************************************************/
int Get_Encoder_Count(Encoder_Data_TypeDef* encoder_data)
{
	if (encoder_data->tim_number == 2)
	{
		encoder_data->value = TIM2->CNT + (Tim_Encoder_Data[0].period_count * (ENCODER_PERIOD_COUNT + 1));  // �õ�����������ֵ����ǰ����ֵ+(����������*���ڼ���ֵ)
	}
	else if (encoder_data->tim_number == 3)
	{
		encoder_data->value = TIM3->CNT + (Tim_Encoder_Data[1].period_count * (ENCODER_PERIOD_COUNT + 1));  // �õ�����������ֵ����ǰ����ֵ+(����������*���ڼ���ֵ)
	}
	else if (encoder_data->tim_number == 4)
	{
		encoder_data->value = TIM4->CNT + (Tim_Encoder_Data[2].period_count * (ENCODER_PERIOD_COUNT + 1));  // �õ�����������ֵ����ǰ����ֵ+(����������*���ڼ���ֵ)
	}
	else if (encoder_data->tim_number == 5)
	{
		encoder_data->value = TIM5->CNT + (Tim_Encoder_Data[3].period_count * (ENCODER_PERIOD_COUNT + 1));  // �õ�����������ֵ����ǰ����ֵ+(����������*���ڼ���ֵ)
	}

#if	L_Change_Diretion
	if (encoder_data == &L1_Encoder || encoder_data == &L2_Encoder)	encoder_data->value = -encoder_data->value;
#endif

#if R_Change_Diretion
	if (encoder_data == &R1_Encoder || encoder_data == &R2_Encoder)	encoder_data->value = -encoder_data->value;
#endif
	return	encoder_data->value;
}

/**************************************************************************
�������ܣ��õ�����ٶ�
��ڲ��������������ݽṹ�����ָ�루�ں�����������������������ټ��(ms)
����  ֵ�����ת���ٶ�(תÿ��)	//������ý���һ����ʱ���ڲ����ٶȣ���Ȼ�ᵼ�²��ټ����׼ȷ
**************************************************************************/
float Get_Motor_Speed( Encoder_Data_TypeDef* encoder_data, u16 time )
{
	int count_chage, old_count, new_count;
	float Speed;
	
	old_count = encoder_data->value;  // �ɼ���ֵ��ֵ
	new_count = Get_Encoder_Count(encoder_data);  // �õ��¼���ֵ
	count_chage = new_count - old_count;  // ����ֵ�仯��
	
	/*�ٶȼ��㣺 �ٶ�(r/min) =  ((count_chage / ROTO_RATIO / REDUCTION_RATIO) / (time / 1000)) * 60 */
	Speed = (60000 * count_chage) / (time * ROTO_RATIO * REDUCTION_RATIO);
	return Speed;
}

