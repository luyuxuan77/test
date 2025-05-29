#include "sys.h"
#include "led.h"
#include "motor.h"
#include "bluetooch_u2.h"

u8 BT_dat = 0;
void USART2_IRQHandler(void)
{
	if(USART2->SR&(1<<5))  // ���յ�����
	{	 
		BT_dat = USART2->DR;
	}
}

//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void usart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // �õ�USARTDIV@OVER8=0
	mantissa = temp;  // �õ���������
	fraction = (temp-mantissa)*16;  // �õ�С������@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<0;  // ʹ��PORTA��ʱ��
	RCC->APB1ENR |= 1<<17;  // ʹ�ܴ���2ʱ��
	GPIO_Set(GPIOA, PIN2|PIN3, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PA2 PA3���ù���,�������
 	GPIO_AF_Set(GPIOA, 2, 7);  // PA2	TX
	GPIO_AF_Set(GPIOA, 3, 7);  // PA3	RX 

 
	USART2->BRR=mantissa;  // ����������	 
	USART2->CR1 &= ~(1<<15);  // ����OVER8=0 
	USART2->CR1 |= 1<<3;  // ���ڷ���ʹ�� 
	USART2->CR1 |= 1<<2;  // ���ڽ���ʹ��
	USART2->CR1 |= 1<<5;  // ���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3, 2, USART2_IRQn, 2);
	USART2->CR1 |= 1<<13;  // ����ʹ��
}


//����һ���ֽ�
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);
}

void usart2_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i = 0; i < len; i++)  // ѭ����������
	{
		while((USART2->SR&0X40)==0);  // �ȴ����ͽ���
		USART2->DR=data[i];
	}
	while((USART2->SR&0X40)==0);  // �ȴ����ͽ���
}

