#ifndef __PWM_H_
#define __PWM_H_

#include "bsp.h"


/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMG0
#define PWM_0_INST_IRQHandler                                   TIMG0_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMG0_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                          DL_GPIO_PIN_5
#define GPIO_PWM_0_C0_IOMUX                                       (IOMUX_PINCM6)
#define GPIO_PWM_0_C0_IOMUX_FUNC                      IOMUX_PINCM6_PF_TIMG0_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                          DL_GPIO_PIN_6
#define GPIO_PWM_0_C1_IOMUX                                       (IOMUX_PINCM7)
#define GPIO_PWM_0_C1_IOMUX_FUNC                      IOMUX_PINCM7_PF_TIMG0_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for PWM_3 */
#define PWM_1_INST                                                         TIMG4
#define PWM_1_INST_IRQHandler                                   TIMG4_IRQHandler
#define PWM_1_INST_INT_IRQN                                     (TIMG4_INT_IRQn)
#define PWM_1_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_1_C0_PORT                                                 GPIOA
#define GPIO_PWM_1_C0_PIN                                         DL_GPIO_PIN_10
#define GPIO_PWM_1_C0_IOMUX                                      (IOMUX_PINCM11)
#define GPIO_PWM_1_C0_IOMUX_FUNC                     IOMUX_PINCM11_PF_TIMG4_CCP0
#define GPIO_PWM_1_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_1_C1_PORT                                                 GPIOA
#define GPIO_PWM_1_C1_PIN                                         DL_GPIO_PIN_11
#define GPIO_PWM_1_C1_IOMUX                                      (IOMUX_PINCM12)
#define GPIO_PWM_1_C1_IOMUX_FUNC                     IOMUX_PINCM12_PF_TIMG4_CCP1
#define GPIO_PWM_1_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for PWM_2 */
#define PWM_2_INST                                                         TIMG2
#define PWM_2_INST_IRQHandler                                   TIMG2_IRQHandler
#define PWM_2_INST_INT_IRQN                                     (TIMG2_INT_IRQn)
#define PWM_2_INST_CLK_FREQ                                             32000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_2_C0_PORT                                                 GPIOA
#define GPIO_PWM_2_C0_PIN                                          DL_GPIO_PIN_3
#define GPIO_PWM_2_C0_IOMUX                                       (IOMUX_PINCM4)
#define GPIO_PWM_2_C0_IOMUX_FUNC                      IOMUX_PINCM4_PF_TIMG2_CCP0
#define GPIO_PWM_2_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_2_C1_PORT                                                 GPIOA
#define GPIO_PWM_2_C1_PIN                                          DL_GPIO_PIN_4
#define GPIO_PWM_2_C1_IOMUX                                       (IOMUX_PINCM5)
#define GPIO_PWM_2_C1_IOMUX_FUNC                      IOMUX_PINCM5_PF_TIMG2_CCP1
#define GPIO_PWM_2_C1_IDX                                    DL_TIMER_CC_1_INDEX

void pwm_init(void);
void motor(void);
void AH_BL (int speed);
void AH_CL (int speed);
void BH_AL (int speed);
void BH_CL (int speed);
void CH_AL (int speed);
void CH_BL (int speed);
#endif