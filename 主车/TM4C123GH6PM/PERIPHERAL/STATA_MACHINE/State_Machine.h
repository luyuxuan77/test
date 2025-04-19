#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "track.h"
#include "motor.h"
#include "uart.h"
#include "pid.h"
#include "stdbool.h"
#include "my_timer.h"
#include "distance.h"
#define delay_ms(n); SysCtlDelay(n*(SysCtlClockGet()/3000));
#define delay_us(n); SysCtlDelay(n*(SysCtlClockGet()/3000000));

extern uint8_t step;
void State_Machine_Main(void);

#endif

