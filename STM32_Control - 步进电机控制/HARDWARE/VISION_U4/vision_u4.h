#ifndef VISION_U4_H
#define VISION_U4_H 
#include "sys.h"
#include "stdio.h"

#define USART4_REC_LEN  		200  // 定义最大接收字节数 200
	  	
extern u8  USART4_RX_BUF[USART4_REC_LEN];  // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART4_RX_STA;  // 接收状态标记

void uart4_init(u32 pclk2,u32 bound); 
void uart4_send(u8 data);

void recieveData(void);
#endif

