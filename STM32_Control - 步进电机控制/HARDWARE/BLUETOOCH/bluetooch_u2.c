#include "sys.h"
#include "led.h"
#include "motor.h"
#include "bluetooch_u2.h"

u8 BT_dat = 0;
void USART2_IRQHandler(void)
{
	if(USART2->SR&(1<<5))  // 接收到数据
	{	 
		BT_dat = USART2->DR;
	}
}

//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void usart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // 得到USARTDIV@OVER8=0
	mantissa = temp;  // 得到整数部分
	fraction = (temp-mantissa)*16;  // 得到小数部分@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<0;  // 使能PORTA口时钟
	RCC->APB1ENR |= 1<<17;  // 使能串口2时钟
	GPIO_Set(GPIOA, PIN2|PIN3, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PA2 PA3复用功能,上拉输出
 	GPIO_AF_Set(GPIOA, 2, 7);  // PA2	TX
	GPIO_AF_Set(GPIOA, 3, 7);  // PA3	RX 

 
	USART2->BRR=mantissa;  // 波特率设置	 
	USART2->CR1 &= ~(1<<15);  // 设置OVER8=0 
	USART2->CR1 |= 1<<3;  // 串口发送使能 
	USART2->CR1 |= 1<<2;  // 串口接收使能
	USART2->CR1 |= 1<<5;  // 接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3, 2, USART2_IRQn, 2);
	USART2->CR1 |= 1<<13;  // 串口使能
}


//发送一个字节
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);
}

void usart2_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i = 0; i < len; i++)  // 循环发送数据
	{
		while((USART2->SR&0X40)==0);  // 等待发送结束
		USART2->DR=data[i];
	}
	while((USART2->SR&0X40)==0);  // 等待发送结束
}

