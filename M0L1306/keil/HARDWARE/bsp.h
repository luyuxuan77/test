#ifndef bsp_h
#define bsp_h

/******************系统头文件***************/
#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "math.h"
#include "ti_msp_dl_config.h"


/*******************************************/

/******************用户自定义头文件*********/

#include "led.h"
#include "keyboard.h"
#include "oled.h" 
#include "pwm.h"
#include "opa.h"
#include "exti.h"
#include "uart0.h"
#include "exti.h"



/*******************************************/





//#define GPIO_getEnabledInterruptStatus(GPIOx,Pinx)	 DL_GPIO_getEnabledInterruptStatus(GPIOx,Pinx)



#endif