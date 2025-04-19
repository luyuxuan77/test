#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "bsp.h"

/* Port definition for Pin Group R1 */
#define R1_PORT                                                          (GPIOB)
/* Defines for PIN_3: GPIOB.19 with pinCMx 45 on package pin 16 */
#define R1__PIN                                            (DL_GPIO_PIN_19)
#define R1__IOMUX                                           (IOMUX_PINCM45)

/* Port definition for Pin Group R2 */
#define R2_PORT                                                          (GPIOA)
/* Defines for PIN_4: GPIOA.22 with pinCMx 47 on package pin 18 */
#define R2__PIN                                            (DL_GPIO_PIN_22)
#define R2__IOMUX                                           (IOMUX_PINCM47)

/* Port definition for Pin Group R3 */
#define R3_PORT                                                          (GPIOA)
/* Defines for PIN_5: GPIOA.24 with pinCMx 54 on package pin 25 */
#define R3__PIN                                            (DL_GPIO_PIN_24)
#define R3__IOMUX                                           (IOMUX_PINCM54)

/* Port definition for Pin Group R4 */
#define R4_PORT                                                          (GPIOB)
/* Defines for PIN_6: GPIOB.20 with pinCMx 48 on package pin 19 */
#define R4__PIN                                            (DL_GPIO_PIN_20)
#define R4__IOMUX                                           (IOMUX_PINCM48)

/* Port definition for Pin Group C1 */
#define C1_PORT                                                          (GPIOB)
/* Defines for PIN_7: GPIOB.21 with pinCMx 49 on package pin 20 */
#define C1__PIN                                            (DL_GPIO_PIN_21)
#define C1__IOMUX                                           (IOMUX_PINCM49)

/* Port definition for Pin Group C2 */
#define C2_PORT                                                          (GPIOB)
/* Defines for PIN_8: GPIOB.22 with pinCMx 50 on package pin 21 */
#define C2__PIN                                            (DL_GPIO_PIN_22)
#define C2__IOMUX                                           (IOMUX_PINCM50)

/* Port definition for Pin Group C3 */
#define C3_PORT                                                          (GPIOB)
/* Defines for PIN_9: GPIOB.23 with pinCMx 51 on package pin 22 */
#define C3__PIN                                            (DL_GPIO_PIN_23)
#define C3__IOMUX                                           (IOMUX_PINCM51)

/* Port definition for Pin Group C4 */
#define C4_PORT                                                          (GPIOB)
/* Defines for PIN_10: GPIOB.24 with pinCMx 52 on package pin 23 */
#define C4__PIN                                           (DL_GPIO_PIN_24)
#define C4__IOMUX                                          (IOMUX_PINCM52)


void keyboard_init(void);
int keyboard_scan(void);
void scan_keyR_init(void);
void scan_keyC_init(void);

#endif
