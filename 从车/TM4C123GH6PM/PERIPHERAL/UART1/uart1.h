#ifndef __UART1_H
#define __UART1_H

#include <stdint.h>  // ȷ��������׼���Ͷ���

/* ���������� */
#define MEM_BUFFER_SIZE         1024  // �ڴ洫�仺������С����λ���֣�
#define UART_TXBUF_SIZE         256   // ���ͻ������ֽ���
#define UART_RXBUF_SIZE         256   // ���ջ������ֽ���

/* ��������ֵȫ�ֱ������� */
volatile extern uint32_t g_u32Distance;  // �洢������ľ���ֵ����λ�����ף�

/* ���л��������� */
extern uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];  // DMAԴ������
extern uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];  // DMAĿ�껺����
extern uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];     // �������ݻ�����
extern uint8_t g_ui8RxBufA[UART_RXBUF_SIZE];    // ƹ�һ�����A
extern uint8_t g_ui8RxBufB[UART_RXBUF_SIZE];    // ƹ�һ�����B
extern uint32_t g_ui32RxBufACount;              // ������A��Ч���ݼ���
extern uint32_t g_ui32RxBufBCount;              // ������B��Ч���ݼ���

/* ����ԭ�� */
void uart1_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void InitUART1Transfer(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void USART1_IRQHandler(void);

#endif /* __UART1_H */
