#ifndef KEY_H
#define KEY_H
#include "sys.h"

#define KEY0 	PAin(4)  // KEY0
#define KEY1 	PAin(5)  // KEY1
#define KEY_UP 	PAin(0)  // KEY0

#define KEY0_PRES	1  // key0按下
#define KEY1_PRES	2  // ley1按下

void Key_Init(void);  // 按键初始化函数
u8 KEY_Scan(u8);  // 按键扫描函数
#endif
