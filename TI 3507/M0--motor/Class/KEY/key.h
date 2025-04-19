#ifndef key_h
#define key_h
#include "bsp.h"

extern int Key_Value;

void key_init(void);

#define KEY_GPIO				GPIOA
#define GPIO_key1_IRQn		(GPIOA_INT_IRQn)



/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOA)

/* Defines for A3: GPIOA.3 with pinCMx 8 on package pin 43 */
// pins affected by this interrupt request:["A3"]
#define KEY_INT_IRQN                                            (GPIOA_INT_IRQn)
#define KEY_INT_IIDX                            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define KEY_A3_IIDX                                          (DL_GPIO_IIDX_DIO3)
#define KEY_A3_PIN                                               (DL_GPIO_PIN_3)
#define KEY_A3_IOMUX                                              (IOMUX_PINCM8)

/* Defines for KEY_A4: GPIOA.4 with pinCMx 9 on package pin 44 */
#define KEY_KEY_A4_IIDX                                      (DL_GPIO_IIDX_DIO4)
#define KEY_A4_PIN                                           (DL_GPIO_PIN_4)
#define KEY_A4_IOMUX                                          (IOMUX_PINCM9)

#endif