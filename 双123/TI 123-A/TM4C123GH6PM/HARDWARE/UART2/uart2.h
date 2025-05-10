#ifndef __UART2_H
#define __UART2_H

void uart2_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART2Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void USART2_IRQHandler(void);

#endif



