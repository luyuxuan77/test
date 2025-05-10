#ifndef RGB_H
#define RGB_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

void RGB_Init(void);
void Grren_Light_Control(unsigned char mode);
void Blue_Light_Control(unsigned char mode);
#endif

