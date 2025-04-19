#ifndef UART0_H
#define UART0_H
#include <stdbool.h>
#include <stdint.h>
#include "tiva_sys.h"
#include "tiva_lib.h"


extern volatile char c;
void UART0_IRQHandler(void);
void uart0_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
#endif
