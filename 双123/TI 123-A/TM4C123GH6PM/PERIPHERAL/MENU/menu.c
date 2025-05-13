//#inclue "menu.h"
//#include "keyboard.h"
//#include "oled.h"
//#include "uart0.h"

#include "bsp.h"

extern const uint8_t cmd_send[6];
extern uint8_t speed_send[8];
extern uint8_t station_send[13];
extern const uint8_t stop_send[5];

// �˵�ѡ�����5����
const char* menu_items[] = {
    " Enable",
    " Speed",
    " Location",
    " Stop",
    " About"
};

// �˵�״̬����
uint8_t menu_index = 0;       // ��ǰѡ��������
uint8_t menu_offset = 0;      // �˵���ʾƫ����
uint8_t menu_selected = 0xFF; // FF ��ʾδѡ�У�����Ϊģʽ����
uint8_t in_menu = 1;          // �Ƿ��ڲ˵�����
static uint16_t current_speed = 45; // ʾ���ٶ�ֵ

// ʹ�ܺ���
void manual_operation(void)
{
    // �������д�ֶ������Ĵ���
    OLED_Clear();
    OLED_ShowString(0, 0, "Enable operation", 16);
//    OLED_Refresh_Gram();
    // ��������ָ��
    UART0Send(cmd_send, sizeof(cmd_send));
	delay_ms(170);
    if(response_flag == 1)
	{
    OLED_ShowString(8, 16, "SET Sent!", 16);
    OLED_Refresh_Gram();
	delay_ms(1000);  // �ӳ�һ��ʱ����ʾ��SET Sent!��
	menu_init();     // �Զ����ز˵�����
	}
}

// �ٶ�ģʽ����
void auto_operation(void)
{
    OLED_Clear();
    OLED_ShowString(8, 0, "Set Speed:", 16);
    OLED_ShowNum(80, 0, current_speed, 3, 16);
    OLED_Refresh_Gram();

    while (1)
    {
        uint8_t key = keyboard_scan();

        if (key == 1) // �� �����ٶ�
        {
            if (current_speed < 999)
                current_speed += 5;
        }
        else if (key == 2) // �� �����ٶ�
        {
            if (current_speed > 5)
                current_speed -= 5;
        }
        else if (key == 4) // �������˵����������������ٶ�
        {
            // ���²�����һ�����յ��ٶ�
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
		
		continue; // δ����������ˢ��

        // ÿ�α��������һ���ٶ�ָ��
        speed_send[3] = (current_speed >> 8) & 0xFF;
        speed_send[4] = current_speed & 0xFF;
        UART0Send(speed_send, sizeof(speed_send));

        // ʵʱˢ����ʾ
        OLED_Clear();
        OLED_ShowString(8, 0, "Set Speed:", 16);
        OLED_ShowNum(80, 0, current_speed, 3, 16);
        OLED_Refresh_Gram();

        delay_ms(120); // ���׷���
    }
}


// λ��ģʽ����
void settings_operation(void)
{
    // �������д���ò����Ĵ���
    OLED_Clear();
    OLED_ShowString(0, 0, "Location operation", 16);
    OLED_Refresh_Gram();
	static uint32_t pulse_count = 3200; // ʾ��������
    
    OLED_Clear();
    OLED_ShowString(8, 0, "Set Station:", 16);
    OLED_ShowNum(80, 0, pulse_count, 5, 16);
    OLED_Refresh_Gram();
    
    // ���²�����վ��ָ��
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

// ��ͣ��������
void calibration_operation(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Sending STOP...", 16);
    OLED_Refresh_Gram();
    
    // ���ͼ�ָͣ��
    UART0Send(stop_send, sizeof(stop_send));
	delay_ms(170);
    if(response_flag == 4)
	{
    OLED_ShowString(8, 16, "STOP Sent!", 16);
	}
    OLED_Refresh_Gram();

}

// ����ҡ�ڲ�������
void about_operation(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Swing Operation", 16);
    OLED_Refresh_Gram();

    uint8_t direction = 0x00; // ��ʼ����Ϊ 0x00

    for (int i = 0; i < 10; i++) // �ڶ� 10 ��
    {
        speed_send[2] = direction; // ���÷���
        speed_send[3] = (current_speed >> 8) & 0xFF;
        speed_send[4] = current_speed & 0xFF;

        delay_ms(100);
        UART0Send(stop_send, sizeof(stop_send));
        delay_ms(100);
        UART0Send(speed_send, sizeof(speed_send));

        OLED_ShowString(0, 16, "Swinging...", 16);
        OLED_ShowNum(80, 16, current_speed, 3, 16);
        OLED_Refresh_Gram();

        delay_ms(150); // �ڶ���ʱ
        direction = !direction; // �ı䷽��0x00 �� 0x01 ֮���л���
    }

    delay_ms(100);
    UART0Send(stop_send, sizeof(stop_send));

    OLED_Clear();
    OLED_ShowString(0, 0, "Swing Done!", 16);
    OLED_Refresh_Gram();
    delay_ms(1000);
    menu_init(); // �Զ����ز˵�
}


// �˵���ʼ������
void menu_init(void)
{
    OLED_Clear();
    menu_index = 0;
    menu_offset = 0;
    menu_selected = 0xFF;
    in_menu = 1;
    menu_display();
}

// ��ʾ�˵��б����������ܣ�
void menu_display(void)
{
    OLED_Clear();

    // ����ɼ��Χ
    uint8_t visible_items = (MENU_ITEM_COUNT < MAX_VISIBLE_ITEMS)? MENU_ITEM_COUNT : MAX_VISIBLE_ITEMS;

    // ��ʾ�ɼ��˵���
    for (uint8_t i = 0; i < visible_items; i++)
    {
        uint8_t item_index = menu_offset + i;

        if (item_index == menu_index)
            OLED_ShowString(0, i * 16, ">", 16);  // ѡ������ʾ��ͷ
        else
            OLED_ShowString(0, i * 16, " ", 16);  // ��ѡ��������

        OLED_ShowString(8, i * 16, (const uint8_t*)menu_items[item_index], 16);
    }

    OLED_Refresh_Gram();  // ˢ����ʾ
}

// ��ʾȷ��ģʽ
void show_selected_mode(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "Selected:", 16);
    OLED_ShowString(0, 20, (const uint8_t*)menu_items[menu_index], 16);
    OLED_Refresh_Gram();  // ˢ����ʾ
}

// �˵����к�������������߼���
void menu_run(void)
{
    uint8_t key = keyboard_scan();

    if (in_menu)
    {
        if (key == 2) // �� ����
        {
            if (menu_index < MENU_ITEM_COUNT - 1)
            {
                menu_index++;

                // ���ѡ������ɼ���Χ������ƫ����
                if (menu_index >= menu_offset + MAX_VISIBLE_ITEMS)
                    menu_offset = menu_index - MAX_VISIBLE_ITEMS + 1;

                menu_display();
            }
        }
        else if (key == 1) // �� ����
        {
            if (menu_index > 0)
            {
                menu_index--;

                // ���ѡ������ɼ���Χ������ƫ����
                if (menu_index < menu_offset)
                    menu_offset = menu_index;

                menu_display();
            }
        }
        else if (key == 3) // ȷ��
        {
            menu_selected = menu_index;
            in_menu = 0;
//            show_selected_mode();

            // ����ѡ�еĲ˵��������Ӧ�Ĳ�������
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
    else // ��ѡ��ģʽ���ȴ����ؼ�
    {
        if (key == 4) // ����
        {
            in_menu = 1;
            menu_display();
        }
    }
}
