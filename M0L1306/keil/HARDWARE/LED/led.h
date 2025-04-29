#ifndef LED_H_
#define LED_H_

void delay_ms(int x);
void delay_us(int x);

void LED_init(void);
void controlLED(int led_id, int state); 
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOA)

/* Defines for LED0: GPIOA.8 with pinCMx 9 on package pin 12 */
#define LED_LED0_PIN                                             (DL_GPIO_PIN_8)
#define LED_LED0_IOMUX                                            (IOMUX_PINCM9)
/* Defines for LED1: GPIOA.9 with pinCMx 10 on package pin 13 */
#define LED_LED1_PIN                                             (DL_GPIO_PIN_9)
#define LED_LED1_IOMUX                                           (IOMUX_PINCM10)
/* Defines for LED2: GPIOA.10 with pinCMx 11 on package pin 14 */
#define LED_LED2_PIN                                            (DL_GPIO_PIN_10)
#define LED_LED2_IOMUX                                           (IOMUX_PINCM11)
/* Defines for LED3: GPIOA.11 with pinCMx 12 on package pin 15 */
#define LED_LED3_PIN                                            (DL_GPIO_PIN_11)
#define LED_LED3_IOMUX                                           (IOMUX_PINCM12)
/* Defines for LED4: GPIOA.2 with pinCMx 3 on package pin 6 */
#define LED_LED4_PIN                                             (DL_GPIO_PIN_2)
#define LED_LED4_IOMUX                                            (IOMUX_PINCM3)
/* Defines for LED5: GPIOA.3 with pinCMx 4 on package pin 7 */
#define LED_LED5_PIN                                             (DL_GPIO_PIN_3)
#define LED_LED5_IOMUX                                            (IOMUX_PINCM4)
/* Defines for LED6: GPIOA.15 with pinCMx 16 on package pin 19 */
#define LED_LED6_PIN                                            (DL_GPIO_PIN_15)
#define LED_LED6_IOMUX                                           (IOMUX_PINCM16)
/* Defines for LED7: GPIOA.4 with pinCMx 5 on package pin 8 */
#define LED_LED7_PIN                                             (DL_GPIO_PIN_4)
#define LED_LED7_IOMUX                                            (IOMUX_PINCM5)

#endif /* LED_H_ */
