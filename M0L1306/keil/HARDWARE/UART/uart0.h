#ifndef __UART0_H_
#define __UART0_H_
#include "bsp.h"


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_9
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_21
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM10)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM10_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM22_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_0_FBRD_32_MHZ_115200_BAUD                                      (23)





/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for PIN_0: GPIOA.0 with pinCMx 1 on package pin 1 */
// groups represented: ["GPIO_GRP_1","GPIO_GRP_2","GPIO_GRP_0"]
// pins affected: ["PIN_1","PIN_2","PIN_0"]
#define GPIO_MULTIPLE_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_MULTIPLE_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_GRP_0_PIN_0_IIDX                                (DL_GPIO_IIDX_DIO0)
#define GPIO_GRP_0_PIN_0_PIN                                     (DL_GPIO_PIN_0)
#define GPIO_GRP_0_PIN_0_IOMUX                                    (IOMUX_PINCM1)
/* Port definition for Pin Group GPIO_GRP_1 */
#define GPIO_GRP_1_PORT                                                  (GPIOA)

/* Defines for PIN_1: GPIOA.1 with pinCMx 2 on package pin 2 */
#define GPIO_GRP_1_PIN_1_IIDX                                (DL_GPIO_IIDX_DIO1)
#define GPIO_GRP_1_PIN_1_PIN                                     (DL_GPIO_PIN_1)
#define GPIO_GRP_1_PIN_1_IOMUX                                    (IOMUX_PINCM2)
/* Port definition for Pin Group GPIO_GRP_2 */
#define GPIO_GRP_2_PORT                                                  (GPIOA)

/* Defines for PIN_2: GPIOA.2 with pinCMx 3 on package pin 6 */
#define GPIO_GRP_2_PIN_2_IIDX                                (DL_GPIO_IIDX_DIO2)
#define GPIO_GRP_2_PIN_2_PIN                                     (DL_GPIO_PIN_2)
#define GPIO_GRP_2_PIN_2_IOMUX                                    (IOMUX_PINCM3)
void uart0_init(void);
void uart0_send_char(char ch);
#endif
