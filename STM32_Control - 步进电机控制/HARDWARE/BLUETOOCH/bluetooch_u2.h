#ifndef BLUETOOCH_U2_H
#define BLUETOOCH_U2_H 
#include "sys.h"
#include "stdio.h"

extern u8 BT_dat;

void usart2_init(u32 pclk2,u32 bound);
void usart2_send(u8 data);
void usart2_sendString(u8 *data,u8 len);

#endif

