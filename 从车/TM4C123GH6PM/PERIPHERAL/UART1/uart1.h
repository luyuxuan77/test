#ifndef __UART1_H
#define __UART1_H

#include <stdint.h>  // 确保包含标准类型定义

/* 缓冲区配置 */
#define MEM_BUFFER_SIZE         1024  // 内存传输缓冲区大小（单位：字）
#define UART_TXBUF_SIZE         256   // 发送缓冲区字节数
#define UART_RXBUF_SIZE         256   // 接收缓冲区字节数

/* 新增距离值全局变量声明 */
volatile extern uint32_t g_u32Distance;  // 存储解析后的距离值（单位：毫米）

/* 现有缓冲区声明 */
extern uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];  // DMA源缓冲区
extern uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];  // DMA目标缓冲区
extern uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];     // 发送数据缓冲区
extern uint8_t g_ui8RxBufA[UART_RXBUF_SIZE];    // 乒乓缓冲区A
extern uint8_t g_ui8RxBufB[UART_RXBUF_SIZE];    // 乒乓缓冲区B
extern uint32_t g_ui32RxBufACount;              // 缓冲区A有效数据计数
extern uint32_t g_ui32RxBufBCount;              // 缓冲区B有效数据计数

/* 函数原型 */
void uart1_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void InitUART1Transfer(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void USART1_IRQHandler(void);

#endif /* __UART1_H */
