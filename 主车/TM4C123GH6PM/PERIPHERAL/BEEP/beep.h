#ifndef __BEEP_H__
#define __BEEP_H__

#include <stdint.h>


// 函数声明
void Buzzer_Init(void);    // 初始化蜂鸣器
void Buzzer_On(void);      // 打开蜂鸣器
void Buzzer_Off(void);     // 关闭蜂鸣器
void BEEP_BeepNTimes(uint8_t times);

#endif // __BUZZER_H__
