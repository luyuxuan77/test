#include "sys.h"
#include "usart6.h"

void USART6_IRQHandler(void)
{
	if(USART6->SR&(1<<5))  // 接收到数据
	{
		
		
	}
}									 
//初始化IO 串口6
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void usart6_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // 得到USARTDIV@OVER8=0
	mantissa = temp;  // 得到整数部分
	fraction = (temp-mantissa)*16;  // 得到小数部分@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<6;  // 使能PORTG口时钟  
	RCC->APB2ENR |= 1<<5;  // 使能串口6时钟 
	GPIO_Set(GPIOG, PIN9|PIN14, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PC6,PC7,复用功能,上拉输出
 	GPIO_AF_Set(GPIOG, 14, 8);  // PG14,AF8	TX
	GPIO_AF_Set(GPIOG, 9, 8);  // PG9,AF8  	RX 
 	
	USART6->BRR=mantissa;  // 波特率设置	 
	USART6->CR1 &= ~(1<<15);  // 设置OVER8=0 
	USART6->CR1 |= 1<<3;  // 串口发送使能 
	USART6->CR1 |= 1<<2;  // 串口接收使能
	USART6->CR1 |= 1<<5;  // 接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3, 2, USART6_IRQn, 2);
	USART6->CR1 |= 1<<13;  // 串口使能
}
