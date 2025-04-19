#ifndef KEY_H
#define KEY_H

#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

// Ӳ�����Ӷ���
#define KEY1_PIN        GPIO_PIN_3  // PE3 ��Ϊ����1
#define KEY2_PIN        GPIO_PIN_4  // PF4 ��Ϊ����2
#define KEY_PORTF       GPIO_PORTF_BASE
#define KEY_PORTE       GPIO_PORTE_BASE

// ����״̬��ȡ��
#define KEY1_STATE      GPIOPinRead(KEY_PORTE, KEY1_PIN)
#define KEY2_STATE      GPIOPinRead(KEY_PORTF, KEY2_PIN)

// �������±�ʶ
#define KEY1_PRESS      1   // PE3 ���±�ʶ
#define KEY2_PRESS      2   // PF4 ���±�ʶ

// ��������
void GPIO_Unlock(uint32_t ui32Port, uint8_t ui8Pins);  // ������������
void KEY_Init(void);  // ������ʼ����������
void Menu_Process(void);
int task1(void);
int task2(void);
int task3(void);
int task4(void);


// �жϴ���������
void PortF_Interrupt(void); // PF4 �жϷ�����
void PortE_Interrupt(void); // PE3 �жϷ�����

// ȫ�ֱ��������ڼ�¼��������ʱ�䣩
extern volatile uint32_t key1_press_time;
extern volatile uint32_t key2_press_time;

// ȫ�ֱ��������ڼ�¼�����Ƿ��£�
extern volatile int key1;
extern volatile int key2;
extern volatile int lap_count;         // ��¼����ɵ�Ȧ��
extern volatile int checkpoint_count ;  // ��¼��ǰȦ�ھ����ļ�������
extern volatile int prev_flag ;         // ��¼�Ƿ�վ������㣬��ֹ�ظ�����
extern uint8_t itemFlag;


#endif // KEY_H
