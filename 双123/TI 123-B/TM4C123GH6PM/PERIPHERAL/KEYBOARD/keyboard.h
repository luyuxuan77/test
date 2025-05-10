#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>
// 使用 GPIOD 的 PD1, PD2 作为行输出，PD3, PD4 作为列输入
#define ROW_PORT        GPIOD
#define COL_PORT        GPIOD

#define ROW1_PIN        DL_GPIO_PIN_1
#define ROW2_PIN        DL_GPIO_PIN_2
#define COL1_PIN        DL_GPIO_PIN_3
#define COL2_PIN        DL_GPIO_PIN_4

// 初始化按键GPIO
void keyboard_init(void);

// 按键扫描函数（返回1~4，0表示无按键）
uint8_t Key(void);

#endif // __KEYBOARD_H__
