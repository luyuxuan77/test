#ifndef MY_ADC_H
#define MY_ADC_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

void ADC0_Init(void);
uint16_t ADC0_Get(void);

#endif
