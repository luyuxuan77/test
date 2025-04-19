#ifndef __UART1_H
#define __UART1_H
#include "bsp.h"

/* Defines for UART_1 */
/* 在本工程当中，有关串口中断的#define最好与对应.h在一起，否则可能会出现无法进入中断现象 */
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


