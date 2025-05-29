#include "uart5_pc.h"
#include "debug.h"


void UART5_IRQHandler(void)
{
	u8 res;
	if(UART5->SR&(1<<5))  // ���յ�����
	{	 
		res = UART5->DR;
		debug_handle(&res);
	} 
}

//��ʼ��IO ����5
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart5_init( u32 pclk2, u32 bound )
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // �õ�USARTDIV@OVER8=0
	mantissa = temp;  // �õ���������
	fraction = (temp - mantissa)*16;  // �õ�С������@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<2;  // ʹ��PORTC��ʱ��
	RCC->AHB1ENR |= 1<<3;  // ʹ��PD��ʱ��
	RCC->APB1ENR |= 1<<20;  // ʹ�ܴ���5ʱ�� 
	GPIO_Set( GPIOC, PIN12, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU );  // PC12���ù���,�������
	GPIO_Set( GPIOD, PIN2, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU );  // PD2
 	GPIO_AF_Set(GPIOC, 12, 8);  // PC12,AF8	TX
	GPIO_AF_Set(GPIOD, 2, 8);  // PD2,AF8  RX 

 	UART5->BRR = mantissa;  // ����������	 
	UART5->CR1 &= ~(1<<15);  // ����OVER8=0 
	UART5->CR1 |= 1<<3;  // ���ڷ���ʹ�� 
	UART5->CR1 |= 1<<2;  // ���ڽ���ʹ��
	UART5->CR1 |= 1<<5;  // ���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3, 2, UART5_IRQn, 2);
	UART5->CR1 |= 1<<13;  // ����ʹ��
}

//����һ���ֽ�
void uart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40) == 0);
}

//�����ַ���
void uart5_sendString( u8 *data, u8 len )
{
	int i = 0;
	for(i = 0; i < len; i++)  // ѭ����������
	{
		while((UART5->SR&0X40) == 0);  // �ȴ����ͽ���
		UART5->DR = data[i];
	}
	while((UART5->SR&0X40) == 0);  // �ȴ����ͽ���
}

