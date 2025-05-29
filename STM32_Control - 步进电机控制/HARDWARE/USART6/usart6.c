#include "sys.h"
#include "usart6.h"

void USART6_IRQHandler(void)
{
	if(USART6->SR&(1<<5))  // ���յ�����
	{
		
		
	}
}									 
//��ʼ��IO ����6
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void usart6_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // �õ�USARTDIV@OVER8=0
	mantissa = temp;  // �õ���������
	fraction = (temp-mantissa)*16;  // �õ�С������@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<6;  // ʹ��PORTG��ʱ��  
	RCC->APB2ENR |= 1<<5;  // ʹ�ܴ���6ʱ�� 
	GPIO_Set(GPIOG, PIN9|PIN14, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PC6,PC7,���ù���,�������
 	GPIO_AF_Set(GPIOG, 14, 8);  // PG14,AF8	TX
	GPIO_AF_Set(GPIOG, 9, 8);  // PG9,AF8  	RX 
 	
	USART6->BRR=mantissa;  // ����������	 
	USART6->CR1 &= ~(1<<15);  // ����OVER8=0 
	USART6->CR1 |= 1<<3;  // ���ڷ���ʹ�� 
	USART6->CR1 |= 1<<2;  // ���ڽ���ʹ��
	USART6->CR1 |= 1<<5;  // ���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3, 2, USART6_IRQn, 2);
	USART6->CR1 |= 1<<13;  // ����ʹ��
}
