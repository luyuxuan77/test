#include "ti_msp_dl_config.h"
#include "bsp.h"
#include "uart3.h"
//    int Key_Value = 0;
//    int Current_Mode = 0; // ��ǰѡ���ģʽ��0 - ��ʼ���棬1 - ��Ŀһ��2 - ��Ŀ��
//    void Display_Initial_Menu(void)
//    {
//        OLED_Clear();
//        OLED_ShowString(4, 0, "2020TI G");
//        OLED_ShowString(0, 2, "Topic 1:Press 1");
//        OLED_ShowString(0, 4, "Topic 2: Press 2");
//    }

//    void Display_Selected_Mode(void)
//    {
//        OLED_Clear();
//        if (Current_Mode == 1)
//        {
//            OLED_ShowString(12, 0, "Topic 1");
//            // ��ʾ��Ŀһ������ݣ�������������չ��
//        }
//        else if (Current_Mode == 2)
//        {
//            OLED_ShowString(12, 0, "Topic 2");
//            // ��ʾ��Ŀ��������ݣ�������������չ��
//        }
//    }
//    void Menu_System(void)
//{
//    if (Current_Mode == 0)
//    {
//        Display_Initial_Menu();
//    }
//    else
//    {
//        Display_Selected_Mode();
//    }
//}
int main(void)
{
    SYSCFG_DL_init();
    led_init();
    OLED_Init();
    key_init();
    uart0_init(115200);  
    uart3_init(115200);  
    tima0_pwm_init();
//    uint32_t data1, data2, data3, data4;
    OLED_Clear();    
        OLED_ShowString(4, 0, "2020TI G");
        OLED_ShowString(0, 2, "Topic 1:Press 1");
        OLED_ShowString(0, 4, "Topic 2: Press 2");
    uint16_t pwm_value = 1912;  // ��ʼλ�ã�0�ȣ������ʼ�Ƕȣ�
    int direction = 1;          // 1Ϊ˳ʱ�룬-1Ϊ��ʱ��

    while(1)
    {  // ���ƶ��ת���Ƕ�
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, pwm_value, DL_TIMER_CC_0_INDEX);  // ����PWM

        // �ı�Ƕ�ֵ�����ƶ������ת��
        if (direction == 1) {
            pwm_value += 30;  // ˳ʱ�뷽������PWMֵ
            if (pwm_value >= 2400) {  // 90�㼫��ֵ
                direction = -1;  // ����90��ʱ�ı䷽��
            }
        }
        else {
            pwm_value -= 30;  // ��ʱ�뷽�򣬼���PWMֵ
            if (pwm_value <= 1600) {  // -90�㼫��ֵ
                direction = 1;  // ����-90��ʱ�ı䷽��
            }
        }
        
        // ��ʱ��ȷ�����ƽ���ƶ�
        delay_ms(250);


//            if (Key_Value == 1)
//            {
////             Current_Mode = 1;
//             OLED_Clear();    
//             OLED_ShowString(12, 0, "Topic 1");
////                //���ݶ�ȡ
////        data1 = g_usart3_rx_buf[1];  
////        data2 = g_usart3_rx_buf[2];  
////        data3 = g_usart3_rx_buf[3];
////        data4 = g_usart3_rx_buf[4];

//        // ��ɫ
//        OLED_ShowString(0, 0, "Color:");
//        switch(g_usart3_rx_buf[1]) {
//            case 1: OLED_ShowString(48, 0, "Red  "); break;
//            case 3: OLED_ShowString(48, 0, "Blue "); break;
//            case 2: OLED_ShowString(48, 0, "Green"); break;
//            default: OLED_ShowString(48, 0, "     "); break;
//        }

//        // ��״
//        OLED_ShowString(0, 2, "Shape:");
//        switch(g_usart3_rx_buf[2]) {
//            case 1: OLED_ShowString(48, 2, "Tri   "); break;
//            case 2: OLED_ShowString(48, 2, "Cir   "); break; 
//            case 3: OLED_ShowString(48, 2, "Rect  "); break; 
//            default: OLED_ShowString(48, 2, "      "); break;
//        }
////        printf("%c",g_usart3_rx_buf[0]);
////        printf("%c",g_usart3_rx_buf[1]);
////        printf("%c",g_usart3_rx_buf[2]);
////        printf("%c",g_usart3_rx_buf[3]);
//        Key_Value = 0;
//        OLED_ShowNum(4, 4,g_usart3_rx_buf[3],3,16);  // ��ʾ�����ַ���

//            }
//            else if (Key_Value == 2)//����Զ�Ѱ��
//            {
////             Current_Mode = 2;
//             OLED_Clear();    
//             OLED_ShowString(12, 0, "Topic 2");
//             Key_Value = 0;
//            }
    }
}
