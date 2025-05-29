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

/*******************************************/

/******************用户自定义头文件*********/
#include "uart0.h"
#include "uart1.h"
#include "uart2.h"
#include "uart3.h"
#include "led.h"
#include "tim_g0.h"
#include "keyboard.h"
#include "tim_g8_pwm.h"
#include "tim_g6.h"
#include "key.h"
#include "Emm_V5.h"
#include "fifo.h"
#include "motor.h"
#include "pid.h"
#include "gray.h"
#include "spi0.h"
#include "OLED/oled_spi0.h"
#include "tima0_pwm.h"
#include "delay.h"
#include "control.h"

/*******************************************/





#define GPIO_getEnabledInterruptStatus(GPIOx,Pinx)	 DL_GPIO_getEnabledInterruptStatus(GPIOx,Pinx)



#endif