#ifndef LED_H
#define LED_H
#include "sys.h"

#define LED1 		PFout(9)  // LED1
#define LED1_ON 	PFout(9) = 0  // LED1µÆÁÁ
#define LED1_OFF	PFout(9) = 1  // LED1µÆÃð

#define LED2		PFout(10)  // LED2
#define LED2_ON 	PFout(10)=0  // LED2µÆÁÁ
#define LED2_OFF	PFout(10)=1  // LED2µÆÃð

void Led_Init(void);
void Led_Blink(u8 num);
void Led_Warning(void);
#endif
