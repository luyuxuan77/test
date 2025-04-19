#ifndef __TIM_G0_
#define __TIM_G0_
#include <ti/driverlib/driverlib.h>

/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (49U)

extern int Flag,Count,Time;
void tim_g0_init(void);
#endif
