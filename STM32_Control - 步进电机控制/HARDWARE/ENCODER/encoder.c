#include "encoder.h"
#include "motor.h"

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	RCC->APB1ENR |= 1<<0;  // TIM2时钟使能
	
	/* 把定时器初始化为编码器模式 */ 
	TIM2->PSC = 0x0;  // 预分频器
	TIM2->ARR = ENCODER_PERIOD_COUNT;  // 设定计数器自动重装值
	TIM2->CCMR1 |= 1<<0;  // 输入模式，IC1FP1映射到TI1上
	TIM2->CCMR1 |= 1<<8;  // 输入模式，IC2FP2映射到TI2上
	TIM2->CCER &= 0<<1;  // IC1不反向
	TIM2->CCER &= 0<<5;  // IC2不反向
	TIM2->SMCR |= 3<<0;  // SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM2->CR1 |= 0x01;  // CEN=1，使能定时器
	
	RCC->AHB1ENR |= 1<<0;  // 使能PORTA时钟
	RCC->AHB1ENR |= 1<<1;  // 使能PORTB时钟
	
	GPIO_Set(GPIOA, PIN15, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_Set(GPIOB, PIN3, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOA, 15, 1);
	GPIO_AF_Set(GPIOB, 3, 1);
	
	MY_NVIC_Init(2, 3, TIM2_IRQn, 2);  // 中断服务函数，抢占优先级2，响应优先级3
	TIM2->SR &= ~(1<<0);  // 清除更新中断标志
	TIM2->CNT = 0;  // 使能更新中断前清除计数值，清除计数器的误计数
	TIM2->DIER |= 1<<0;  // 使能更新中断
}

/**************************************************************************
函数功能：把TIM3初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	RCC->APB1ENR |= 1<<1;  // TIM3时钟使能
	
	/* 把定时器初始化为编码器模式 */ 
	TIM3->PSC = 0x0;  // 预分频器
	TIM3->ARR = ENCODER_PERIOD_COUNT;  // 设定计数器自动重装值
	TIM3->CCMR1 |= 1<<0;  // 输入模式，IC1FP1映射到TI1上
	TIM3->CCMR1 |= 1<<8;  // 输入模式，IC2FP2映射到TI2上
	TIM3->CCER |= 0<<1;  // IC1不反向
	TIM3->CCER |= 0<<5;  // IC2不反向
	TIM3->SMCR |= 3<<0;  // SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM3->CR1 |= 0x01;  // CEN=1，使能定时器
	
	RCC->AHB1ENR|=1<<1;  // 使能PORTB时钟
	GPIO_Set(GPIOB, PIN4|PIN5, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOB, 4, 2);
	GPIO_AF_Set(GPIOB, 5, 2);
	
	MY_NVIC_Init(2, 3, TIM3_IRQn, 2);  // 中断服务函数，抢占优先级2，响应优先级3
	TIM3->SR &= ~(1<<0);  // 清除更新中断标志
	TIM3->CNT = 0;  // 使能更新中断前清除计数值，清除计数器的误计数
	TIM3->DIER |= 1<<0;  // 使能更新中断
}
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	RCC->APB1ENR |= 1<<2;  // TIM4时钟使能
	
	/* 把定时器初始化为编码器模式 */ 
	TIM4->PSC = 0x0;//预分频器
	TIM4->ARR = ENCODER_PERIOD_COUNT;  // 设定计数器自动重装值
	TIM4->CCMR1 |= 1<<0;  // 输入模式，IC1FP1映射到TI1上
	TIM4->CCMR1 |= 1<<8;  // 输入模式，IC2FP2映射到TI2上
	TIM4->CCER |= 0<<1;  // IC1不反向
	TIM4->CCER |= 0<<5;  // IC2不反向
	TIM4->SMCR |= 3<<0;  // SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM4->CR1 |= 0x01;  // CEN=1，使能定时器
	
	RCC->AHB1ENR |= 1<<1;  // 使能PORTB时钟
	GPIO_Set(GPIOB, PIN6|PIN7, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOB, 6, 2);
	GPIO_AF_Set(GPIOB, 7, 2);
	
	MY_NVIC_Init(2, 3, TIM4_IRQn, 2);  // 中断服务函数，抢占优先级2，响应优先级3
	TIM4->SR &= ~(1<<0);  // 清除更新中断标志
	TIM4->CNT = 0;  // 使能更新中断前清除计数值，清除计数器的误计数
	TIM4->DIER |= 1<<0;  // 使能更新中断
}

/**************************************************************************
函数功能：把TIM5初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM5(void)
{
	RCC->APB1ENR |= 1<<3;  // TIM5时钟使能
	
	/* 把定时器初始化为编码器模式 */ 
	TIM5->PSC = 0x0;  // 预分频器
	TIM5->ARR = ENCODER_PERIOD_COUNT;  // 设定计数器自动重装值
	TIM5->CCMR1 |= 1<<0;  // 输入模式，IC1FP1映射到TI1上
	TIM5->CCMR1 |= 1<<8;  // 输入模式，IC2FP2映射到TI2上
	TIM5->CCER |= 0<<1;  // IC1不反向
	TIM5->CCER |= 0<<5;  // IC2不反向
	TIM5->SMCR |= 3<<0;  // SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM5->CR1 |= 0x01;  // CEN=1，使能定时器
	
	RCC->AHB1ENR |= 1<<0;  // 使能PORTA时钟
	GPIO_Set(GPIOA, PIN0|PIN1, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);
	GPIO_AF_Set(GPIOA, 0, 2);
	GPIO_AF_Set(GPIOA, 1, 2);
	
	MY_NVIC_Init(2, 3, TIM5_IRQn, 2);  // 中断服务函数，抢占优先级2，响应优先级3
	TIM5->SR &= ~(1<<0);  // 清除更新中断标志
	TIM5->CNT = 0;  // 使能更新中断前清除计数值，清除计数器的误计数
	TIM5->DIER |= 1<<0;  // 使能更新中断
}

/*定义编码器数据结构体变量数组
  Tim_Encoder_Data[0]对应tim2
  Tim_Encoder_Data[1]对应tim3
  Tim_Encoder_Data[2]对应tim4
  Tim_Encoder_Data[3]对应tim5
  与硬件tim定时器对应，不可轻易更改tim_number*/
Encoder_Data_TypeDef Tim_Encoder_Data[4] = { {2, 0, 0}, {3, 0, 0}, {4, 0, 0}, {5, 0, 0} };

/**************************************************************************
函数功能：定时器2中断服务函数，定时器2计数值更新进入中断，Tim_Encoder_Data[0]计数周期改变
入口参数：无（函数内容包含软硬件连接（硬连接），不可轻易更改）
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{
	if (TIM2->SR & 0x0001)  // 判断是否发生更新中断
	{
		if (TIM2->CR1 & 0x0010)  // 计数器递减计数
		{
			Tim_Encoder_Data[0].period_count--;  // 计数周期--
		}
		else  // 计数器递增计数
		{
			Tim_Encoder_Data[0].period_count++;  // 计数周期++
		}
		TIM2->SR &= ~(1<<0);  // 清除中断标志位
	}

}
/**************************************************************************
函数功能：定时器3中断服务函数，定时器3计数值更新进入中断，Tim_Encoder_Data[1]计数周期改变
入口参数：无（函数内容包含软硬件连接（硬连接），不可轻易更改）
返回  值：无
**************************************************************************/
void TIM3_IRQHandler(void)
{
	if (TIM3->SR & 0x0001)  // 判断是否发生更新中断
	{
		if (TIM3->CR1 & 0x0010)  // 计数器递减计数
		{
			Tim_Encoder_Data[1].period_count--;  // 计数周期--
		}
		else  // 计数器递增计数
		{
			Tim_Encoder_Data[1].period_count++;  // 计数周期++
		}
		TIM3->SR &= ~(1<<0);  // 清除中断标志位
	}

}
/**************************************************************************
函数功能：定时器4中断服务函数，定时器4计数值更新进入中断，Tim_Encoder_Data[2]计数周期改变
入口参数：无（函数内容包含软硬件连接（硬连接），不可轻易更改）
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM4->SR & 0x0001)  // 判断是否发生更新中断
	{
		if (TIM4->CR1 & 0x0010)  // 计数器递减计数
		{
			Tim_Encoder_Data[2].period_count--;  // 计数周期--
		}
		else  // 计数器递增计数
		{
			Tim_Encoder_Data[2].period_count++;  // 计数周期++
		}
		TIM4->SR &= ~(1<<0);  // 清除中断标志位
	}

}
/**************************************************************************
函数功能：定时器5中断服务函数，定时器5计数值更新进入中断，Tim_Encoder_Data[3]计数周期改变
入口参数：无（函数内容包含软硬件连接（硬连接），不可轻易更改）
返回  值：无
**************************************************************************/
void TIM5_IRQHandler(void)
{
	if (TIM5->SR & 0x0001)  // 判断是否发生更新中断
	{
		if (TIM5->CR1 & 0x0010)  // 计数器递减计数
		{
			Tim_Encoder_Data[3].period_count--;  // 计数周期--
		}
		else  // 计数器递增计数
		{
			Tim_Encoder_Data[3].period_count++;  // 计数周期++
		}
		TIM5->SR &= ~(1<<0);  // 清除中断标志位
	}

}

/**************************************************************************
函数功能：得到编码器计数值（函数内容包含软硬件连接（硬连接），不可轻易更改）
入口参数：编码器数据结构体变量指针，其中包括其对应编码器的各种参数
返回  值：电机编码器绝对计数值
**************************************************************************/
int Get_Encoder_Count(Encoder_Data_TypeDef* encoder_data)
{
	if (encoder_data->tim_number == 2)
	{
		encoder_data->value = TIM2->CNT + (Tim_Encoder_Data[0].period_count * (ENCODER_PERIOD_COUNT + 1));  // 得到编码器计数值，当前计数值+(计数周期数*周期计数值)
	}
	else if (encoder_data->tim_number == 3)
	{
		encoder_data->value = TIM3->CNT + (Tim_Encoder_Data[1].period_count * (ENCODER_PERIOD_COUNT + 1));  // 得到编码器计数值，当前计数值+(计数周期数*周期计数值)
	}
	else if (encoder_data->tim_number == 4)
	{
		encoder_data->value = TIM4->CNT + (Tim_Encoder_Data[2].period_count * (ENCODER_PERIOD_COUNT + 1));  // 得到编码器计数值，当前计数值+(计数周期数*周期计数值)
	}
	else if (encoder_data->tim_number == 5)
	{
		encoder_data->value = TIM5->CNT + (Tim_Encoder_Data[3].period_count * (ENCODER_PERIOD_COUNT + 1));  // 得到编码器计数值，当前计数值+(计数周期数*周期计数值)
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
函数功能：得到电机速度
入口参数：编码器数据结构体变量指针（内含编码器各参数），电机测速间隔(ms)
返回  值：电机转动速度(转每分)	//测速最好仅在一个定时器内测电机速度，不然会导致测速间隔不准确
**************************************************************************/
float Get_Motor_Speed( Encoder_Data_TypeDef* encoder_data, u16 time )
{
	int count_chage, old_count, new_count;
	float Speed;
	
	old_count = encoder_data->value;  // 旧计数值赋值
	new_count = Get_Encoder_Count(encoder_data);  // 得到新计数值
	count_chage = new_count - old_count;  // 计数值变化量
	
	/*速度计算： 速度(r/min) =  ((count_chage / ROTO_RATIO / REDUCTION_RATIO) / (time / 1000)) * 60 */
	Speed = (60000 * count_chage) / (time * ROTO_RATIO * REDUCTION_RATIO);
	return Speed;
}

