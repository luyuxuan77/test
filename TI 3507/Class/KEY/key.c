#include "key.h"

volatile int Key_Value = 0; // 必须加volatile

void key_init(void)
{
    /* 引脚配置（保持原样） */
    DL_GPIO_initDigitalInputFeatures(KEY_A3_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(KEY_A4_IOMUX,
        DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    /* 配置中断触发边沿（保持原样） */
    DL_GPIO_setLowerPinsPolarity(KEY_PORT, DL_GPIO_PIN_3_EDGE_FALL | DL_GPIO_PIN_4_EDGE_FALL);
    
    /* 清除所有可能的中断标志 */
    DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_A3_PIN | KEY_A4_PIN);
    
    /* 使能中断 */
    DL_GPIO_enableInterrupt(KEY_PORT, KEY_A3_PIN | KEY_A4_PIN);
    
    /* 使能NVIC中断 */
    NVIC_EnableIRQ(GPIO_key1_IRQn);
}

/* 修改后的中断服务函数 */
void GROUP1_IRQHandler(void)
{   
    int i=0;
    /* 获取实际触发中断的引脚 */
    uint32_t triggered_pins = DL_GPIO_getEnabledInterruptStatus(KEY_GPIO, KEY_A3_PIN | KEY_A4_PIN);

    /* 处理PA3中断 */
    if (triggered_pins & KEY_A3_PIN) 
    {
        while(i==800)
        {i++;
        }
        if (!DL_GPIO_readPins(KEY_PORT, KEY_A3_PIN)) // 确认仍是低电平
        Key_Value = 1;
        DL_GPIO_clearPins(GPIOA, DL_GPIO_PIN_0); // 假设低电平点亮LED
        DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_A3_PIN); // 清除标志
        i=0;
    }

    /* 处理PA4中断 */
    if (triggered_pins & KEY_A4_PIN) 
    {
        while(i==800)
        {i++;
        }
        if (!DL_GPIO_readPins(KEY_PORT, KEY_A4_PIN))  // 确认仍是低电平
        Key_Value = 2;
        DL_GPIO_setPins(GPIOA, DL_GPIO_PIN_0);
        DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_A4_PIN); // 清除标志
        i=0;
    }
}