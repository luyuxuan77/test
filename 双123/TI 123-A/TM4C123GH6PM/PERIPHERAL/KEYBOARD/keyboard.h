#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>

// 初始化键盘引脚
void keyboard_init(void);

// 按键扫描函数，返回 1~4 键值，无按键返回 0
uint8_t keyboard_scan(void);

#endif // __KEYBOARD_H
