#ifndef KEY_H
#define KEY_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

#define	SW1_STATE	GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)	//sw1����״̬
#define	SW2_STATE	GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)	//sw2����״̬

#define SW1_PRESS	1	//SW1����
#define SW2_PRESS	2	//SW2����

void KEY_Init(void);
unsigned char KEY_Scan(unsigned char mode);
#endif
