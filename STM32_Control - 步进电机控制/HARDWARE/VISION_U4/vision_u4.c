#include "sys.h"
#include "led.h"
#include "math.h"
#include "delay.h"
#include "stdlib.h"
#include "vision_u4.h"

u8 USART4_RX_BUF[USART4_REC_LEN];  // ���ջ���,���USART_REC_LEN���ֽ�.
u16 USART4_RX_STA=0;  // ����״̬���

void UART4_IRQHandler(void)
{
	u8 res;
	
	if(UART4->SR&(1<<5))  // ���յ�����
	{	 
		res=UART4->DR; 
		if((USART4_RX_STA&0x8000)==0)  // ����δ���
		{
			if(USART4_RX_STA&0x4000)  // ���յ���0x0d
			{
				if(res!=0x0a) USART4_RX_STA=0;  // ���մ���,���¿�ʼ
				else USART4_RX_STA |= 0x8000;  // ���������
			}else  // ��û�յ�0X0D
			{
				if(res==0x0d)USART4_RX_STA |= 0x4000;
				else
				{
					USART4_RX_BUF[USART4_RX_STA&0X3FFF]=res;
					USART4_RX_STA++;
					if(USART4_RX_STA>(USART4_REC_LEN-1)) USART4_RX_STA=0;  // �������ݴ���,���¿�ʼ����	  
				}
			}
		}
	}
	recieveData();
} 
								 
//��ʼ��IO ����4
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart4_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // �õ�USARTDIV@OVER8=0
	mantissa = temp;  // �õ���������
	fraction = (temp-mantissa)*16;  // �õ�С������@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<2;  // ʹ��PORTC��ʱ��  
	RCC->APB1ENR |= 1<<19;  // ʹ�ܴ���4ʱ�� 
	GPIO_Set(GPIOC, PIN10|PIN11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PC10,PC11,���ù���,�������
 	GPIO_AF_Set(GPIOC, 10, 8);  // PC10,AF8	TX
	GPIO_AF_Set(GPIOC, 11, 8);  // PC11,AF8	RX

 	UART4->BRR=mantissa;  // ����������
	UART4->CR1 &= ~(1<<15);  // ����OVER8=0
	UART4->CR1 |= 1<<3;  // ���ڷ���ʹ��
	UART4->CR1 |= 1<<2;  // ���ڽ���ʹ��
	UART4->CR1 |= 1<<5;  // ���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3, 2, UART4_IRQn, 2);
	UART4->CR1 |= 1<<13;  // ����ʹ��
}

//����һ���ֽ�
void uart4_send(u8 data)
{
	UART4->DR = data;
	while((UART4->SR&0x40)==0);
}

void recieveData(void)
{

}
