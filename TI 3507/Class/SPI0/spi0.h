#ifndef spi0_h
#define spi0_h
#include "bsp.h"

/* Defines for SPI_0 */
#define SPI_0_INST                                                         SPI0
#define SPI_0_INST_IRQHandler                                   SPI0_IRQHandler
#define SPI_0_INST_INT_IRQN                                       SPI0_INT_IRQn
#define GPIO_SPI_0_PICO_PORT                                              GPIOA
#define GPIO_SPI_0_PICO_PIN                                      DL_GPIO_PIN_14
#define GPIO_SPI_0_IOMUX_PICO                                   (IOMUX_PINCM36)
#define GPIO_SPI_0_IOMUX_PICO_FUNC                   IOMUX_PINCM36_PF_SPI0_PICO

#define GPIO_SPI_0_POCI_PORT                                              GPIOA
#define GPIO_SPI_0_POCI_PIN                                      DL_GPIO_PIN_13
#define GPIO_SPI_0_IOMUX_POCI                                   (IOMUX_PINCM35)
#define GPIO_SPI_0_IOMUX_POCI_FUNC                   IOMUX_PINCM35_PF_SPI0_POCI
/* GPIO configuration for SPI_0 */
#define GPIO_SPI_0_SCLK_PORT                                              GPIOA
#define GPIO_SPI_0_SCLK_PIN                                      DL_GPIO_PIN_12
#define GPIO_SPI_0_IOMUX_SCLK                                   (IOMUX_PINCM34)
#define GPIO_SPI_0_IOMUX_SCLK_FUNC                   IOMUX_PINCM34_PF_SPI0_SCLK

#define GPIO_SPI_0_CS0_PORT                                               GPIOA
#define GPIO_SPI_0_CS0_PIN                                        DL_GPIO_PIN_8
#define GPIO_SPI_0_IOMUX_CS0                                    (IOMUX_PINCM19)
#define GPIO_SPI_0_IOMUX_CS0_FUNC                     IOMUX_PINCM19_PF_SPI0_CS0

/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOB)

/* Defines for OLED_DC: GPIOB.2 with pinCMx 15 on package	1q pin 50 */
#define OLED_OLED_DC_PIN                                         (DL_GPIO_PIN_2)
#define OLED_OLED_DC_IOMUX                                       (IOMUX_PINCM15)

/* Port definition for Pin Group GPIO_RES */
#define GPIO_RES_PORT                                                    (GPIOB)

/* Defines for PIN_0: GPIOB.8 with pinCMx 25 on package pin 60 */
#define GPIO_RES_PIN_0_PIN                                       (DL_GPIO_PIN_8)
#define GPIO_RES_PIN_0_IOMUX                                     (IOMUX_PINCM25)


void spi0_init(void);

#endif
