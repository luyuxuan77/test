#ifndef _TIM_G6_H_
#define _TIM_G6_H_
#include "bsp.h"


/* Defines for TIMER_6 */
#define TIMER_6_INST                                                     (TIMG6)
#define TIMER_6_INST_IRQHandler                                 TIMG6_IRQHandler
#define TIMER_6_INST_INT_IRQN                                   (TIMG6_INT_IRQn)
#define TIMER_6_INST_LOAD_VALUE                                          (9999U)
void tim_g6_init(void);

#endif


