#ifndef VISION_U4_H
#define VISION_U4_H 
#include "sys.h"
#include "stdio.h"

#define USART4_REC_LEN  		200  // �����������ֽ��� 200
	  	
extern u8  USART4_RX_BUF[USART4_REC_LEN];  // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART4_RX_STA;  // ����״̬���

void uart4_init(u32 pclk2,u32 bound); 
void uart4_send(u8 data);

void recieveData(void);
#endif

