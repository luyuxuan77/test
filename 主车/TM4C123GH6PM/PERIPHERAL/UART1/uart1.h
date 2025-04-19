#ifndef __UART1_H
#define __UART1_H

#define MEM_BUFFER_SIZE         1024  // �ڴ洫��Դ��Ŀ�껺�����Ĵ�С������Ϊ��λ��
#define UART_TXBUF_SIZE         256  // UART���ͻ�������С
#define UART_RXBUF_SIZE         256  // UART���ջ�������С

extern uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];  // Դ������
extern uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];  // Ŀ�껺����
extern uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];  // UART���ͻ�����
extern uint8_t g_ui8RxBufA[UART_RXBUF_SIZE];  // UART����ƹ�һ�����
extern uint8_t g_ui8RxBufB[UART_RXBUF_SIZE];  // UART����ƹ�һ�����
extern uint32_t g_ui32RxBufACount;  // UART����ƹ�һ���������
extern uint32_t g_ui32RxBufBCount;  // UART����ƹ�һ���������

void uart1_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void InitUART1Transfer(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void USART1_IRQHandler(void);

#endif

