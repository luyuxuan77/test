#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>

// ��ʼ����������
void keyboard_init(void);

// ����ɨ�躯�������� 1~4 ��ֵ���ް������� 0
uint8_t keyboard_scan(void);

#endif // __KEYBOARD_H
