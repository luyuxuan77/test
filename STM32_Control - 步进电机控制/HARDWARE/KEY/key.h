#ifndef KEY_H
#define KEY_H
#include "sys.h"

#define KEY0 	PAin(4)  // KEY0
#define KEY1 	PAin(5)  // KEY1
#define KEY_UP 	PAin(0)  // KEY0

#define KEY0_PRES	1  // key0����
#define KEY1_PRES	2  // ley1����

void Key_Init(void);  // ������ʼ������
u8 KEY_Scan(u8);  // ����ɨ�躯��
#endif
