#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <stdint.h>
// ʹ�� GPIOD �� PD1, PD2 ��Ϊ�������PD3, PD4 ��Ϊ������
#define ROW_PORT        GPIOD
#define COL_PORT        GPIOD

#define ROW1_PIN        DL_GPIO_PIN_1
#define ROW2_PIN        DL_GPIO_PIN_2
#define COL1_PIN        DL_GPIO_PIN_3
#define COL2_PIN        DL_GPIO_PIN_4

// ��ʼ������GPIO
void keyboard_init(void);

// ����ɨ�躯��������1~4��0��ʾ�ް�����
uint8_t Key(void);

#endif // __KEYBOARD_H__
