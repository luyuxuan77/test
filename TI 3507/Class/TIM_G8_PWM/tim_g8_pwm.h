#ifndef __TIM_G8_PWM_H_
#define __TIM_G8_PWM_H_
#include <ti/driverlib/driverlib.h>
#include "bsp.h"


/* Defines for PWM_1 */
#define PWM_1_INST                                                         TIMG8
#define PWM_1_INST_IRQHandler                                   TIMG8_IRQHandler
#define PWM_1_INST_INT_IRQN                                     (TIMG8_INT_IRQn)
#define PWM_1_INST_CLK_FREQ                                              4000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_1_C0_PORT                                                 GPIOA
#define GPIO_PWM_1_C0_PIN                                          DL_GPIO_PIN_7
#define GPIO_PWM_1_C0_IOMUX                                      (IOMUX_PINCM14)
#define GPIO_PWM_1_C0_IOMUX_FUNC                     IOMUX_PINCM14_PF_TIMG8_CCP0
#define GPIO_PWM_1_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_1_C1_PORT                                                 GPIOA
#define GPIO_PWM_1_C1_PIN                                          DL_GPIO_PIN_6
#define GPIO_PWM_1_C1_IOMUX                                      (IOMUX_PINCM11)
#define GPIO_PWM_1_C1_IOMUX_FUNC                     IOMUX_PINCM11_PF_TIMG8_CCP1
#define GPIO_PWM_1_C1_IDX                                    DL_TIMER_CC_1_INDEX

void tim_g8_pwm_init(void);

#endif