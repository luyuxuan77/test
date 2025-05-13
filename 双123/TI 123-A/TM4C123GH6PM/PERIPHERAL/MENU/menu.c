//#inclue "menu.h"
//#include "keyboard.h"
//#include "oled.h"
//#include "uart0.h"

#include "bsp.h"

extern const uint8_t cmd_send[6];
extern uint8_t speed_send[8];
extern uint8_t station_send[13];
extern const uint8_t stop_send[5];

// 菜单选项（保持5个）
const char* menu_items[] = {
    " Enable",
    " Speed",
    " Location",
    " Stop",
    " About"
};

// 菜单状态变量
uint8_t menu_index = 0;       // 当前选中项索引
uint8_t menu_offset = 0;      // 菜单显示偏移量
uint8_t menu_selected = 0xFF; // FF 表示未选中，其他为模式索引
uint8_t in_menu = 1;          // 是否处于菜单界面
static uint16_t current_speed = 45; // 示例速度值

// 使能函数
void manual_operation(void)
{
    // 在这里编写手动操作的代码
    OLED_Clear();
    OLED_ShowString(0, 0, "Enable operation", 16);
//    OLED_Refresh_Gram();
    // 发送设置指令
    UART0Send(cmd_send, sizeof(cmd_send));
	delay_ms(170);
    if(response_flag == 1)
	{
    OLED_ShowString(8, 16, "SET Sent!", 16);
    OLED_Refresh_Gram();
	delay_ms(1000);  // 延迟一段时间显示“SET Sent!”
	menu_init();     // 自动返回菜单界面
	}
}

// 速度模式函数
void auto_operation(void)
{
    OLED_Clear();
    OLED_ShowString(8, 0, "Set Speed:", 16);
    OLED_ShowNum(80, 0, current_speed, 3, 16);
    OLED_Refresh_Gram();

    while (1)
    {
        uint8_t key = keyboard_scan();

        if (key == 1) // ▲ 增加速度
        {
            if (current_speed < 999)
                current_speed += 5;
        }
        else if (key == 2) // ▼ 减少速度
        {
            if (current_speed > 5)
                current_speed -= 5;
        }
        else if (key == 4) // 返回主菜单并发送最终设置速度
        {
            // 更新并发送一次最终的速度
            speed_send[3] = (current_speed >> 8) & 0xFF;
            speed_send[4] = current_speed & 0xFF;
            UART0Send(speed_send, sizeof(speed_send));
            delay_ms(170);

            if (response_flag == 2)
            {
                OLED_ShowString(8, 16, "Speed Sent!", 16);
                OLED_Refresh_Gram();
                delay_ms(800);
            }

            menu_init();
            return;
        }
        else 
		
		continue; // 未按键则跳过刷新

        // 每次变更都发送一次速度指令
        speed_send[3] = (current_speed >> 8) & 0xFF;
        speed_send[4] = current_speed & 0xFF;
        UART0Send(speed_send, sizeof(speed_send));

        // 实时刷新显示
        OLED_Clear();
        OLED_ShowString(8, 0, "Set Speed:", 16);
        OLED_ShowNum(80, 0, current_speed, 3, 16);
        OLED_Refresh_Gram();

        delay_ms(120); // 简易防抖
    }
}


// 位置模式函数
void settings_operation(void)
{
    // 在这里编写设置操作的代码
    OLED_Clear();
    OLED_ShowString(0, 0, "Location operation", 16);
    OLED_Refresh_Gram();
	static uint32_t pulse_count = 3200; // 示例脉冲数
    
    OLED_Clear();
    OLED_ShowString(8, 0, "Set Station:", 16);
    OLED_ShowNum(80, 0, pulse_count, 5, 16);
    OLED_Refresh_Gram();
    
    // 更新并发送站点指令
    station_send[6] = (pulse_count >> 24);
    station_send[7] = (pulse_count >> 16);
    station_send[8] = (pulse_count >> 8);
    station_send[9] = (pulse_count & 0xFF);
    UART0Send(station_send, sizeof(station_send));
	delay_ms(170);
    if(response_flag == 3)
	{
    OLED_ShowString(8, 16, "Station Sent!", 16);
	}
    OLED_Refresh_Gram();
}

// 急停操作函数
void calibration_operation(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Sending STOP...", 16);
    OLED_Refresh_Gram();
    
    // 发送急停指令
    UART0Send(stop_send, sizeof(stop_send));
	delay_ms(170);
    if(response_flag == 4)
	{
    OLED_ShowString(8, 16, "STOP Sent!", 16);
	}
    OLED_Refresh_Gram();

}

// 左右摇摆操作函数
void about_operation(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Swing Operation", 16);
    OLED_Refresh_Gram();

    uint8_t direction = 0x00; // 初始方向为 0x00

    for (int i = 0; i < 10; i++) // 摆动 10 次
    {
        speed_send[2] = direction; // 设置方向
        speed_send[3] = (current_speed >> 8) & 0xFF;
        speed_send[4] = current_speed & 0xFF;

        delay_ms(100);
        UART0Send(stop_send, sizeof(stop_send));
        delay_ms(100);
        UART0Send(speed_send, sizeof(speed_send));

        OLED_ShowString(0, 16, "Swinging...", 16);
        OLED_ShowNum(80, 16, current_speed, 3, 16);
        OLED_Refresh_Gram();

        delay_ms(150); // 摆动延时
        direction = !direction; // 改变方向（0x00 和 0x01 之间切换）
    }

    delay_ms(100);
    UART0Send(stop_send, sizeof(stop_send));

    OLED_Clear();
    OLED_ShowString(0, 0, "Swing Done!", 16);
    OLED_Refresh_Gram();
    delay_ms(1000);
    menu_init(); // 自动返回菜单
}


// 菜单初始化函数
void menu_init(void)
{
    OLED_Clear();
    menu_index = 0;
    menu_offset = 0;
    menu_selected = 0xFF;
    in_menu = 1;
    menu_display();
}

// 显示菜单列表（带滚动功能）
void menu_display(void)
{
    OLED_Clear();

    // 计算可见项范围
    uint8_t visible_items = (MENU_ITEM_COUNT < MAX_VISIBLE_ITEMS)? MENU_ITEM_COUNT : MAX_VISIBLE_ITEMS;

    // 显示可见菜单项
    for (uint8_t i = 0; i < visible_items; i++)
    {
        uint8_t item_index = menu_offset + i;

        if (item_index == menu_index)
            OLED_ShowString(0, i * 16, ">", 16);  // 选中项显示箭头
        else
            OLED_ShowString(0, i * 16, " ", 16);  // 非选中项留空

        OLED_ShowString(8, i * 16, (const uint8_t*)menu_items[item_index], 16);
    }

    OLED_Refresh_Gram();  // 刷新显示
}

// 显示确认模式
void show_selected_mode(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Selected:", 16);
    OLED_ShowString(0, 20, (const uint8_t*)menu_items[menu_index], 16);
    OLED_Refresh_Gram();  // 刷新显示
}

// 菜单运行函数（处理滚动逻辑）
void menu_run(void)
{
    uint8_t key = keyboard_scan();

    if (in_menu)
    {
        if (key == 2) // ▼ 向下
        {
            if (menu_index < MENU_ITEM_COUNT - 1)
            {
                menu_index++;

                // 如果选中项超出可见范围，调整偏移量
                if (menu_index >= menu_offset + MAX_VISIBLE_ITEMS)
                    menu_offset = menu_index - MAX_VISIBLE_ITEMS + 1;

                menu_display();
            }
        }
        else if (key == 1) // ▲ 向上
        {
            if (menu_index > 0)
            {
                menu_index--;

                // 如果选中项超出可见范围，调整偏移量
                if (menu_index < menu_offset)
                    menu_offset = menu_index;

                menu_display();
            }
        }
        else if (key == 3) // 确认
        {
            menu_selected = menu_index;
            in_menu = 0;
//            show_selected_mode();

            // 根据选中的菜单项调用相应的操作函数
            switch (menu_index)
            {
                case 0:
                    manual_operation();
                    break;
                case 1:
                    auto_operation();
                    break;
                case 2:
                    settings_operation();
                    break;
                case 3:
                    calibration_operation();
                    break;
                case 4:
                    about_operation();
                    break;
                default:
                    break;
            }
        }
    }
    else // 已选中模式，等待返回键
    {
        if (key == 4) // 返回
        {
            in_menu = 1;
            menu_display();
        }
    }
}
