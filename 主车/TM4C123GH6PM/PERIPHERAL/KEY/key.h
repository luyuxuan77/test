#ifndef KEY_H
#define KEY_H

#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

// 硬件连接定义
#define KEY1_PIN        GPIO_PIN_3  // PE3 作为按键1
#define KEY2_PIN        GPIO_PIN_4  // PF4 作为按键2
#define KEY_PORTF       GPIO_PORTF_BASE
#define KEY_PORTE       GPIO_PORTE_BASE

// 按键状态读取宏
#define KEY1_STATE      GPIOPinRead(KEY_PORTE, KEY1_PIN)
#define KEY2_STATE      GPIOPinRead(KEY_PORTF, KEY2_PIN)

// 按键按下标识
#define KEY1_PRESS      1   // PE3 按下标识
#define KEY2_PRESS      2   // PF4 按下标识

// 函数声明
void GPIO_Unlock(uint32_t ui32Port, uint8_t ui8Pins);  // 解锁函数声明
void KEY_Init(void);  // 按键初始化函数声明
void Menu_Process(void);
int task1(void);
int task2(void);
int task3(void);
int task4(void);


// 中断处理函数声明
void PortF_Interrupt(void); // PF4 中断服务函数
void PortE_Interrupt(void); // PE3 中断服务函数

// 全局变量（用于记录按键按下时间）
extern volatile uint32_t key1_press_time;
extern volatile uint32_t key2_press_time;

// 全局变量（用于记录按键是否按下）
extern volatile int key1;
extern volatile int key2;
extern volatile int lap_count;         // 记录已完成的圈数
extern volatile int checkpoint_count ;  // 记录当前圈内经过的监测点数量
extern volatile int prev_flag ;         // 记录是否刚经过监测点，防止重复触发
extern uint8_t itemFlag;


#endif // KEY_H
