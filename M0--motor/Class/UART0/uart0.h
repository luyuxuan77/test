#ifndef __UART0_H
#define __UART0_H

#include "bsp.h"


/* 在本工程当中，有关串口中断的#define最好与对应.h在一起，否则可能会出现无法进入中断现象 */
/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn

#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA


#define UART0_RX_PIN	DL_GPIO_PIN_1
#define UART0_TX_PIN	DL_GPIO_PIN_28

#define UART0_RX_PINCM	IOMUX_PINCM2
#define UART0_TX_PINCM	IOMUX_PINCM3

#define UART0_RX_FUNC	IOMUX_PINCM2_PF_UART0_RX
#define UART0_TX_FUNC	IOMUX_PINCM3_PF_UART0_TX

#define u8 unsigned char


void uart0_init(int baud);
void doubleToStr(double value, char *str, int precision);
void UART_0_INST_IRQHandler(void);

#endif
