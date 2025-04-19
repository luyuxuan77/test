#include "delay.h"

void delay_ms(int ms)
{
	delay_cycles(80000*ms);
}
void delay_us(int us)
{
	delay_cycles(80*us);
}