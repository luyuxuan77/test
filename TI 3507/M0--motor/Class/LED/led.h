#ifndef __LED_
#define __LED_


/*控制LED高低电平*/
#define LED(en)                                        \
    if (en)                                            \
        DL_GPIO_writePinsVal(GPIOA, DL_GPIO_PIN_0, 1); \
    else                                               \
        DL_GPIO_writePinsVal(GPIOA, DL_GPIO_PIN_0, 0);

/*控制LED反转*/
#define LED_toggle DL_GPIO_togglePins(GPIOA, DL_GPIO_PIN_0);


void led_init(void);
void RGB_init(void);
void RGBSet(int	R,int G,int B);
void LED_8_init(void);

void delay_ms(int ms);
void delay_us(int us);

void led_init(void); // 板载LED初始化函数


#define	DeepPink	255,20,147		//深粉红
#define	Crimson		220,20,60			//猩红
#define	Fuchsia		255,0,255		//紫红
#define	SlateBlue	106,90,205		//板岩蓝
#define	DimGray		105,105,105		//灰


#define LED_GPIO	GPIOA
#define LED_PIN	DL_GPIO_PIN_0
#define LED_PINCM	IOMUX_PINCM1
#define led(en)	GPIO_out(LED_GPIO,LED_PIN,en);
#define led_toggle DL_GPIO_togglePins(LED_GPIO,LED_PIN)

#include "bsp.h"

/* Defines for PWM_0 */
#define RGB_RG_INST                                                         TIMG6
#define RGB_RG_INST_IRQHandler                                   TIMG6_IRQHandler
#define RGB_RG_INST_INT_IRQN                                     (TIMG6_INT_IRQn)
#define RGB_RG_INST_CLK_FREQ                                             80000000
/* GPIO defines for channel 0 */
#define GPIO_RGB_R_PORT                                                 GPIOB
#define GPIO_RGB_R_PIN                                         DL_GPIO_PIN_26
#define GPIO_RGB_R_IOMUX                                      (IOMUX_PINCM57)
#define GPIO_RGB_R_IOMUX_FUNC                     IOMUX_PINCM57_PF_TIMG6_CCP0
#define GPIO_RGB_R_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_RGB_G_PORT                                                 GPIOB
#define GPIO_RGB_G_PIN                                          DL_GPIO_PIN_27
#define GPIO_RGB_G_IOMUX                                      (IOMUX_PINCM58)
#define GPIO_RGB_G_IOMUX_FUNC                     IOMUX_PINCM58_PF_TIMG6_CCP1
#define GPIO_RGB_G_IDX                                    DL_TIMER_CC_1_INDEX


/* Defines for PWM_0 */
#define RGB_B_INST                                                         TIMG8
#define RGB_B_INST_IRQHandler                                   TIMG8_IRQHandler
#define RGB_B_INST_INT_IRQN                                     (TIMG8_INT_IRQn)
#define RGB_B_INST_CLK_FREQ                                             40000000
/* GPIO defines for channel 0 */
#define GPIO_RGB_B_PORT                                                 GPIOB
#define GPIO_RGB_B_PIN                                         DL_GPIO_PIN_22
#define GPIO_RGB_B_IOMUX                                      (IOMUX_PINCM50)
#define GPIO_RGB_B_IOMUX_FUNC                     IOMUX_PINCM50_PF_TIMG8_CCP1
#define GPIO_RGB_B_IDX                                    DL_TIMER_CC_1_INDEX








#endif

