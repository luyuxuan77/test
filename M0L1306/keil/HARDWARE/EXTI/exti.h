#ifndef __EXTI_H_
#define __EXTI_H_
#include "bsp.h"


/* Port definition for Pin Group GPIO_KEY */
#define GPIO_KEY_PORT                                                    (GPIOA)

/* Defines for KEY: GPIOA.1 with pinCMx 2 on package pin 2 */
// pins affected by this interrupt request:["KEY"]
#define GPIO_KEY_INT_IRQN                                       (GPIOA_INT_IRQn)
#define GPIO_KEY_INT_IIDX                       (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_KEY_KEY_IIDX                                    (DL_GPIO_IIDX_DIO1)
#define GPIO_KEY_KEY_PIN                                         (DL_GPIO_PIN_1)
#define GPIO_KEY_KEY_IOMUX                                        (IOMUX_PINCM2)
/* Port definition for Pin Group GPIO_PA0 */
#define GPIO_PA_PORT                                                    (GPIOA)

/* Defines for PIN_0: GPIOA.0 with pinCMx 1 on package pin 1 */
// groups represented: ["GPIO_PA1","GPIO_PA2","GPIO_PA0"]
// pins affected: ["PIN_1","PIN_2","PIN_0"]
#define GPIO_MULTIPLE_GPIOA_INT_IRQN                            (GPIOA_INT_IRQn)
#define GPIO_MULTIPLE_GPIOA_INT_IIDX            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)

#define GPIO_PA0_PIN_0_IIDX                                  (DL_GPIO_IIDX_DIO12)
#define GPIO_PA0_PIN_0_PIN                                       (DL_GPIO_PIN_12)
#define GPIO_PA0_PIN_0_IOMUX                                      (IOMUX_PINCM13)

/* Defines for PIN_1: GPIOA.1 with pinCMx 2 on package pin 2 */
#define GPIO_PA1_PIN_1_IIDX                                  (DL_GPIO_IIDX_DIO13)
#define GPIO_PA1_PIN_1_PIN                                       (DL_GPIO_PIN_13)
#define GPIO_PA1_PIN_1_IOMUX                                      (IOMUX_PINCM14)

/* Defines for PIN_2: GPIOA.2 with pinCMx 3 on package pin 6 */
#define GPIO_PA2_PIN_2_IIDX                                  (DL_GPIO_IIDX_DIO14)
#define GPIO_PA2_PIN_2_PIN                                       (DL_GPIO_PIN_14)
#define GPIO_PA2_PIN_2_IOMUX                                      (IOMUX_PINCM15)

void EXTI_init(void);
#endif
