#include "key.h"

volatile int Key_Value = 0; // �����volatile

void key_init(void)
{
    /* �������ã�����ԭ���� */
    DL_GPIO_initDigitalInputFeatures(KEY_A3_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(KEY_A4_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    /* �����жϴ������أ�����ԭ���� */
    DL_GPIO_setLowerPinsPolarity(KEY_PORT, DL_GPIO_PIN_3_EDGE_FALL | DL_GPIO_PIN_4_EDGE_FALL);
    
    /* ������п��ܵ��жϱ�־ */
    DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_A3_PIN | KEY_A4_PIN);
    
    /* ʹ���ж� */
    DL_GPIO_enableInterrupt(KEY_PORT, KEY_A3_PIN | KEY_A4_PIN);
    
    /* ʹ��NVIC�ж� */
    NVIC_EnableIRQ(GPIO_key1_IRQn);
}

/* �޸ĺ���жϷ����� */
void GROUP1_IRQHandler(void)
{   
    int i=0;
    /* ��ȡʵ�ʴ����жϵ����� */
    uint32_t triggered_pins = DL_GPIO_getEnabledInterruptStatus(KEY_GPIO, KEY_A3_PIN | KEY_A4_PIN);

    /* ����PA3�ж� */
    if (triggered_pins & KEY_A3_PIN) 
    {
        while(i==800)
        {i++;
        }
        if (!DL_GPIO_readPins(KEY_PORT, KEY_A3_PIN)) // ȷ�����ǵ͵�ƽ
        Key_Value = 1;
        DL_GPIO_clearPins(GPIOA, DL_GPIO_PIN_0); // ����͵�ƽ����LED
        DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_A3_PIN); // �����־
        i=0;
    }

    /* ����PA4�ж� */
    if (triggered_pins & KEY_A4_PIN) 
    {
        while(i==800)
        {i++;
        }
        if (!DL_GPIO_readPins(KEY_PORT, KEY_A4_PIN))  // ȷ�����ǵ͵�ƽ
        Key_Value = 2;
        DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0);
        DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_A4_PIN); // �����־
        i=0;
    }
}