#ifndef key_h
#define key_h
#include "bsp.h"
//#include "dl_gpio.h" // �������GPIO��ͷ�ļ�

extern volatile int Key_Value; // ����volatile

void key_init(void);

/* ȷ�����º궨����оƬ�ֲ�һ�� */
#define KEY_GPIO                GPIOA
#define GPIO_key1_IRQn          (GPIOA_INT_IRQn)
#define KEY_PORT                GPIOA

/* PA3���� */
#define KEY_A3_PIN              DL_GPIO_PIN_3
#define KEY_A3_IOMUX            IOMUX_PINCM8

/* PA4���� */
#define KEY_A4_PIN              DL_GPIO_PIN_4
#define KEY_A4_IOMUX            IOMUX_PINCM9

#endif