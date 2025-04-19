#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart1.h"
#include "my_timer.h"
#include "control.h"
#include "motor.h"
#include "uart0.h"
#include "ccd.h"
#include "rgb.h"
#include "key.h"
#include "encoder.h"
#include "track.h"
#include "State_Machine.h"
#include "uart7.h"
#include "distance.h"
#include "oled.h"
#include "beep.h"

// 初始化蜂鸣器
void Buzzer_Init(void)
{
    // 使能端口A的时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // 使能GPIOA时钟

    // 确保PA7是一个可用的引脚
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
        // 等待端口A时钟稳定
    }

    // 设置PA7为输出模式
    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT); // 设置PA7为输出模式

    // 配置PA7为标准数字输出
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);  // 配置为标准输出模式

    // 默认关闭蜂鸣器
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);  // 将PA7置低，关闭蜂鸣器
}

// 打开蜂鸣器
void Buzzer_On(void)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 1);  // 将PA7置高，打开蜂鸣器
}

// 关闭蜂鸣器
void Buzzer_Off(void)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);  // 将PA7置低，关闭蜂鸣器
}

void BEEP_BeepNTimes(uint8_t times)
{
    for(uint8_t i=0; i<times; i++){
        Buzzer_On();  // 响
		delay_ms(100);
        Buzzer_Off();        // 停
        delay_ms(50);;          // 间隔约50ms
    }
}
