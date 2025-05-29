#include "sys.h"
#include "led.h"
#include "math.h"
#include "delay.h"
#include "stdlib.h"
#include "vision_u4.h"

u8 USART4_RX_BUF[USART4_REC_LEN];  // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART4_RX_STA=0;  // 接收状态标记

void UART4_IRQHandler(void)
{
	u8 res;
	
	if(UART4->SR&(1<<5))  // 接收到数据
	{	 
		res=UART4->DR; 
		if((USART4_RX_STA&0x8000)==0)  // 接收未完成
		{
			if(USART4_RX_STA&0x4000)  // 接收到了0x0d
			{
				if(res!=0x0a) USART4_RX_STA=0;  // 接收错误,重新开始
				else USART4_RX_STA |= 0x8000;  // 接收完成了
			}else  // 还没收到0X0D
			{
				if(res==0x0d)USART4_RX_STA |= 0x4000;
				else
				{
					USART4_RX_BUF[USART4_RX_STA&0X3FFF]=res;
					USART4_RX_STA++;
					if(USART4_RX_STA>(USART4_REC_LEN-1)) USART4_RX_STA=0;  // 接收数据错误,重新开始接收	  
				}
			}
		}
	}
	recieveData();
} 
								 
//初始化IO 串口4
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart4_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // 得到USARTDIV@OVER8=0
	mantissa = temp;  // 得到整数部分
	fraction = (temp-mantissa)*16;  // 得到小数部分@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<2;  // 使能PORTC口时钟  
	RCC->APB1ENR |= 1<<19;  // 使能串口4时钟 
	GPIO_Set(GPIOC, PIN10|PIN11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PC10,PC11,复用功能,上拉输出
 	GPIO_AF_Set(GPIOC, 10, 8);  // PC10,AF8	TX
	GPIO_AF_Set(GPIOC, 11, 8);  // PC11,AF8	RX

 	UART4->BRR=mantissa;  // 波特率设置
	UART4->CR1 &= ~(1<<15);  // 设置OVER8=0
	UART4->CR1 |= 1<<3;  // 串口发送使能
	UART4->CR1 |= 1<<2;  // 串口接收使能
	UART4->CR1 |= 1<<5;  // 接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3, 2, UART4_IRQn, 2);
	UART4->CR1 |= 1<<13;  // 串口使能
}

//发送一个字节
void uart4_send(u8 data)
{
	UART4->DR = data;
	while((UART4->SR&0x40)==0);
}

void recieveData(void)
{

}
