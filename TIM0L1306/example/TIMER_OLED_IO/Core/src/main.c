#include "ti_msp_dl_config.h"
#include "oled.h"

int cnt = 0;

int main(void)
{
    SYSCFG_DL_init(); // Initialize the device
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(10, 0, "Hello");
    
    DL_TimerG_startCounter(TIMER_0_INST);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    /* ʹ�ܣ�������SWITCH��Ӧ��IO�ж� */
    NVIC_EnableIRQ(SWITCH_INT_IRQN);
   
    while (1)
    {
        __WFI();
    }
}

void TIMG0_IRQHandler(void)
{
    OLED_ShowNum(10, 2, (unsigned int)cnt++, 2, 16);
    if(cnt == 100)
        cnt = 0;
}

void GROUP1_IRQHandler(void)
{
    switch(DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
    {
        case SWITCH_INT_IIDX:           // GPIO�ж�
            DL_GPIO_clearPins(LED_PORT, LED_PIN_1_PIN);
            while(DL_GPIO_readPins(SWITCH_PORT, SWITCH_PIN_0_PIN) == 0);
            DL_GPIO_setPins(LED_PORT, LED_PIN_1_PIN);
        break;
        default: break;                 //����GPIO�жϣ���������
    }
}

