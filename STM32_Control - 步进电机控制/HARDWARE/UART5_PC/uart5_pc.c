#include "uart5_pc.h"
#include "debug.h"


void UART5_IRQHandler(void)
{
	u8 res;
	if(UART5->SR&(1<<5))  // 接收到数据
	{	 
		res = UART5->DR;
		debug_handle(&res);
	} 
}

//初始化IO 串口5
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart5_init( u32 pclk2, u32 bound )
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // 得到USARTDIV@OVER8=0
	mantissa = temp;  // 得到整数部分
	fraction = (temp - mantissa)*16;  // 得到小数部分@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<2;  // 使能PORTC口时钟
	RCC->AHB1ENR |= 1<<3;  // 使能PD口时钟
	RCC->APB1ENR |= 1<<20;  // 使能串口5时钟 
	GPIO_Set( GPIOC, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU );  // PC12复用功能,上拉输出
	GPIO_Set( GPIOD, PIN2, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU );  // PD2
 	GPIO_AF_Set(GPIOC, 12, 8);  // PC12,AF8	TX
	GPIO_AF_Set(GPIOD, 2, 8);  // PD2,AF8  RX 

 	UART5->BRR = mantissa;  // 波特率设置	 
	UART5->CR1 &= ~(1<<15);  // 设置OVER8=0 
	UART5->CR1 |= 1<<3;  // 串口发送使能 
	UART5->CR1 |= 1<<2;  // 串口接收使能
	UART5->CR1 |= 1<<5;  // 接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3, 2, UART5_IRQn, 2);
	UART5->CR1 |= 1<<13;  // 串口使能
}

//发送一个字节
void uart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40) == 0);
}

//发送字符串
void uart5_sendString( u8 *data, u8 len )
{
	int i = 0;
	for(i = 0; i < len; i++)  // 循环发送数据
	{
		while((UART5->SR&0X40) == 0);  // 等待发送结束
		UART5->DR = data[i];
	}
	while((UART5->SR&0X40) == 0);  // 等待发送结束
}

