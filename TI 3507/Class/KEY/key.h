#ifndef key_h
#define key_h
#include "bsp.h"
//#include "dl_gpio.h" // 必须包含GPIO库头文件

extern volatile int Key_Value; // 增加volatile

void key_init(void);

/* 确保以下宏定义与芯片手册一致 */
#define KEY_GPIO                GPIOA
#define GPIO_key1_IRQn          (GPIOA_INT_IRQn)
#define KEY_PORT                GPIOA

/* PA3配置 */
#define KEY_A3_PIN              DL_GPIO_PIN_3
#define KEY_A3_IOMUX            IOMUX_PINCM8

/* PA4配置 */
#define KEY_A4_PIN              DL_GPIO_PIN_4
#define KEY_A4_IOMUX            IOMUX_PINCM9

#endif