#ifndef __BEEP_H__
#define __BEEP_H__

#include <stdint.h>


// ��������
void Buzzer_Init(void);    // ��ʼ��������
void Buzzer_On(void);      // �򿪷�����
void Buzzer_Off(void);     // �رշ�����
void BEEP_BeepNTimes(uint8_t times);

#endif // __BUZZER_H__
