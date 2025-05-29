#ifndef UART5_PC_H
#define UART5_PC_H 
#include "sys.h"
#include "stdio.h"

void uart5_init(u32 pclk2, u32 bound);
void uart5_send(u8 data);
void uart5_sendString(u8 *data, u8 len);
#endif

