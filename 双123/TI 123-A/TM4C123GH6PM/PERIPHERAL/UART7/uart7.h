#ifndef __UART7_H
#define __UART7_H
#include "stdbool.h"
extern unsigned char Distance;
void uart7_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART7Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void USART7_IRQHandler(void);

#endif



