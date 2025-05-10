#ifndef __UART1_H
#define __UART1_H

#define MEM_BUFFER_SIZE         1024  // 内存传输源和目标缓冲区的大小（以字为单位）
#define UART_TXBUF_SIZE         256  // UART发送缓冲区大小
#define UART_RXBUF_SIZE         256  // UART接收缓冲区大小

extern uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];  // 源缓冲区
extern uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];  // 目标缓冲区
extern uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];  // UART发送缓冲区
extern uint8_t g_ui8RxBufA[UART_RXBUF_SIZE];  // UART接收乒乓缓冲区
extern uint8_t g_ui8RxBufB[UART_RXBUF_SIZE];  // UART接收乒乓缓冲区
extern uint32_t g_ui32RxBufACount;  // UART接收乒乓缓冲区计数
extern uint32_t g_ui32RxBufBCount;  // UART接收乒乓缓冲区计数

void uart1_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void InitUART1Transfer(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void USART1_IRQHandler(void);

#endif

