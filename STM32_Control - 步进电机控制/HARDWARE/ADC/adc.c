#include "adc.h"
#include "delay.h"

//初始化ADC3
void Adc3_Init(void)
{
	//先初始化IO口
 	RCC->APB2ENR |= 1<<10;  // 使能ADC3时钟 
	RCC->APB2RSTR |= 1<<8;  // ADCs复位
	RCC->APB2RSTR &= ~(1<<8);  // 复位结束
	ADC->CCR = 3<<16;  // ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
 	
	ADC3->CR1 = 0;  // CR1设置清零
	ADC3->CR2 = 0;  // CR2设置清零
	ADC3->CR1 |= 0<<24;  // 12位模式
	ADC3->CR1 |= 0<<8;  // 非扫描模式	
	
	ADC3->CR2 &= ~(1<<1);  // 单次转换模式
 	ADC3->CR2 &= ~(1<<11);  // 右对齐	
	ADC3->CR2 |= 0<<28;  // 软件触发
	
	ADC3->SQR1 &= ~(0XF<<20);
	ADC3->SQR1 |= 0<<20;  // 1个转换在规则序列中 也就是只转换规则序列1
	//设置通道5的采样时间
	ADC3->SMPR2 &= ~(7<<(3*5));  // 通道5采样时间清空
 	ADC3->SMPR2 |= 7<<(3*5);  // 通道5  480个周期,提高采样时间可以提高精确度
 	ADC3->CR2 |= 1<<0;  // 开启AD转换器
}

//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc3(u8 ch)
{
	//设置转换序列	  		 
	ADC3->SQR3 &= 0XFFFFFFE0;  // 规则序列1 通道ch
	ADC3->SQR3 |= ch;		  			    
	ADC3->CR2 |= 1<<30;  // 启动规则转换通道 
	while(!(ADC3->SR&1<<1));  // 等待转换结束	 	   
	return ADC3->DR;  // 返回adc值	
}

//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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
