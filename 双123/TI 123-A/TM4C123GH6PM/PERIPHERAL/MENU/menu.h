#ifndef MENU_H
#define MENU_H

#include "oled.h"

// 菜单选项
extern const char* menu_items[];
#define MENU_ITEM_COUNT (sizeof(menu_items)/sizeof(menu_items[0]))
#define MAX_VISIBLE_ITEMS 4  // 屏幕最多显示4个选项

// 菜单状态变量
extern uint8_t menu_index;       // 当前选中项索引
extern uint8_t menu_offset;      // 菜单显示偏移量
extern uint8_t menu_selected;    // 选中的菜单项，0xFF表示未选中
extern uint8_t in_menu;          // 是否在菜单界面

// 操作函数声明
void manual_operation(void);
void auto_operation(void);
void settings_operation(void);
void calibration_operation(void);
void about_operation(void);

// 菜单初始化函数
void menu_init(void);
// 显示菜单列表
void menu_display(void);
// 显示确认模式
void show_selected_mode(void);
// 菜单运行函数
void menu_run(void);

#endif
