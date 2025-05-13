#ifndef UART0_H
#define UART0_H
#include <stdbool.h>
#include <stdint.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

void UART0_IRQHandler(void);
void uart0_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count);

/* 新增命令发送函数 */
void send_cmd_set(void);
void send_speed(uint16_t speed);
void send_station(uint32_t pulse_count);
void send_emergency_stop(void);
extern volatile int response_flag; // 全局状态标志

#endif
