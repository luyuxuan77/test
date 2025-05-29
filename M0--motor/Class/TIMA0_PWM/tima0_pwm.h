#ifndef tima0_pwm_h
#define tima0_pwm_h

#include "bsp.h"


/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMA0
#define PWM_0_INST_IRQHandler                                   TIMA0_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOB
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_14
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM31)
#define GPIO_PWM_0_C0_IOMUX_FUNC                     IOMUX_PINCM31_PF_TIMA0_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                          DL_GPIO_PIN_9
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM20)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM20_PF_TIMA0_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX
/* GPIO defines for channel 2 */
#define GPIO_PWM_0_C2_PORT                                                 GPIOA
#define GPIO_PWM_0_C2_PIN                                          DL_GPIO_PIN_7
#define GPIO_PWM_0_C2_IOMUX                                      (IOMUX_PINCM14)
#define GPIO_PWM_0_C2_IOMUX_FUNC                     IOMUX_PINCM14_PF_TIMA0_CCP2
#define GPIO_PWM_0_C2_IDX                                    DL_TIMER_CC_2_INDEX
/* GPIO defines for channel 3 */
#define GPIO_PWM_0_C3_PORT                                                 GPIOB
#define GPIO_PWM_0_C3_PIN                                         DL_GPIO_PIN_24
#define GPIO_PWM_0_C3_IOMUX                                      (IOMUX_PINCM52)
#define GPIO_PWM_0_C3_IOMUX_FUNC                     IOMUX_PINCM52_PF_TIMA0_CCP3
#define GPIO_PWM_0_C3_IDX                                    DL_TIMER_CC_3_INDEX

void tima0_pwm_init(void);

#endif