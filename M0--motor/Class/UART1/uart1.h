#ifndef __UART1_H
#define __UART1_H
#include "bsp.h"

/* Defines for UART_1 */
/* �ڱ����̵��У��йش����жϵ�#define������Ӧ.h��һ�𣬷�����ܻ�����޷������ж����� */
#define UART1_GPIO		GPIOB

#define UART1_RX_PIN	DL_GPIO_PIN_5
#define UART1_TX_PIN	DL_GPIO_PIN_4

#define UART1_RX_PINCM	IOMUX_PINCM18
#define UART1_TX_PINCM	IOMUX_PINCM17

#define UART1_RX_FUNC	IOMUX_PINCM18_PF_UART1_RX
#define UART1_TX_FUNC	IOMUX_PINCM17_PF_UART1_TX

void uart1_init(int baud);
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len);
#endif


