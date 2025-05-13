#include "keyboard.h"
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart1.h"
#include "my_timer.h"
#include "control.h"
//#include "motor.h"
//#include "uart0.h"
//#include "ccd.h"
//#include "rgb.h"
//#include "key.h"
//#include "encoder.h"
//#include "track.h"
//#include "State_Machine.h"
//#include "uart7.h"
//#include "distance.h"
//#include "oled.h"
//#include <stdio.h>
#include "bsp.h"

#define KEYBOARD_PORT GPIO_PORTD_BASE
#define ROW0_PIN GPIO_PIN_0
#define ROW1_PIN GPIO_PIN_1
#define COL0_PIN GPIO_PIN_2
#define COL1_PIN GPIO_PIN_3


// 初始化2x2矩阵键盘的GPIO配置
void keyboard_init(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // 设置行引脚为输出
    MAP_GPIOPinTypeGPIOOutput(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN);

    // 设置列引脚为输入，上拉
    MAP_GPIOPinTypeGPIOInput(KEYBOARD_PORT, COL0_PIN | COL1_PIN);
    MAP_GPIOPadConfigSet(KEYBOARD_PORT, COL0_PIN | COL1_PIN,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // 初始设置所有行引脚为高电平（禁用）
    MAP_GPIOPinWrite(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN, ROW0_PIN | ROW1_PIN);
}

// 按键扫描函数，返回 1~4，未按下返回0
uint8_t keyboard_scan(void)
{
    uint8_t key = 0;
    static uint8_t key_flag = 0;

//    uint8_t rows[2] = {ROW0_PIN, ROW1_PIN};
    uint8_t cols[2] = {COL0_PIN, COL1_PIN};

    if (key_flag)
    {
        delay_ms(200); // 防止重复按下
        key_flag = 0;
    }

    for (int row = 0; row < 2; row++)
    {
        // 拉低当前行，其余为高电平
        uint8_t out = (row == 0) ? ROW1_PIN : ROW0_PIN;
        MAP_GPIOPinWrite(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN, out);
        delay_ms(5); // 消抖延迟

        for (int col = 0; col < 2; col++)
        {
            if (!MAP_GPIOPinRead(KEYBOARD_PORT, cols[col])) // 检测低电平
            {
                key = row * 2 + col + 1;
                key_flag = 1;
                break;
            }
        }

        if (key_flag) break;
    }

    // 所有行回归高电平
    MAP_GPIOPinWrite(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN, ROW0_PIN | ROW1_PIN);

    return key;
}
