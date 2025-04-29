#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "bsp.h" // 确保包含GPIO相关定义

/* Port definition for Pin Group MAT_KEY */
#define MAT_KEY_PORT                                                     (GPIOA)
/* Defines for ROW0 to ROW3 */
#define MAT_KEY_ROW0_PIN         (DL_GPIO_PIN_0)
#define MAT_KEY_ROW1_PIN         (DL_GPIO_PIN_1)
#define MAT_KEY_ROW2_PIN         (DL_GPIO_PIN_7)
#define MAT_KEY_ROW3_PIN         (DL_GPIO_PIN_12)

/* Defines for COL0 to COL3 */
#define MAT_KEY_COL0_PIN         (DL_GPIO_PIN_13)
#define MAT_KEY_COL1_PIN         (DL_GPIO_PIN_14)
#define MAT_KEY_COL2_PIN         (DL_GPIO_PIN_17)
#define MAT_KEY_COL3_PIN         (DL_GPIO_PIN_18)

/* IOMUX configuration for the pins */
#define MAT_KEY_ROW0_IOMUX       (IOMUX_PINCM1)
#define MAT_KEY_ROW1_IOMUX       (IOMUX_PINCM2)
#define MAT_KEY_ROW2_IOMUX       (IOMUX_PINCM8)
#define MAT_KEY_ROW3_IOMUX       (IOMUX_PINCM13)
#define MAT_KEY_COL0_IOMUX       (IOMUX_PINCM14)
#define MAT_KEY_COL1_IOMUX       (IOMUX_PINCM15)
#define MAT_KEY_COL2_IOMUX       (IOMUX_PINCM18)
#define MAT_KEY_COL3_IOMUX       (IOMUX_PINCM19)

/* Function prototype for initializing the keyboard */
void keyboard_init(void);
uint8_t Key(void);


#endif /* KEYBOARD_CONFIG_H */
