#ifndef MENU_H
#define MENU_H

#include "oled.h"

// �˵�ѡ��
extern const char* menu_items[];
#define MENU_ITEM_COUNT (sizeof(menu_items)/sizeof(menu_items[0]))
#define MAX_VISIBLE_ITEMS 4  // ��Ļ�����ʾ4��ѡ��

// �˵�״̬����
extern uint8_t menu_index;       // ��ǰѡ��������
extern uint8_t menu_offset;      // �˵���ʾƫ����
extern uint8_t menu_selected;    // ѡ�еĲ˵��0xFF��ʾδѡ��
extern uint8_t in_menu;          // �Ƿ��ڲ˵�����

// ������������
void manual_operation(void);
void auto_operation(void);
void settings_operation(void);
void calibration_operation(void);
void about_operation(void);

// �˵���ʼ������
void menu_init(void);
// ��ʾ�˵��б�
void menu_display(void);
// ��ʾȷ��ģʽ
void show_selected_mode(void);
// �˵����к���
void menu_run(void);

#endif
