#ifndef MY_TIMER_H
#define MY_TIMER_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
extern unsigned char Laser_Command[9];

extern char cheat,times,Flag;
void Timer0_Init(uint16_t time);
void Timer1_Init(uint16_t time);

#endif
