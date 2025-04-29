#include "bsp.h"


#define CLK_HZ 32e+06 // ϵͳʱ��


void LED_init(void)
{
    DL_GPIO_initDigitalOutput(LED_LED0_IOMUX);

    DL_GPIO_initDigitalOutput(LED_LED1_IOMUX);

    DL_GPIO_initDigitalOutput(LED_LED2_IOMUX);

    DL_GPIO_initDigitalOutput(LED_LED3_IOMUX);

    DL_GPIO_initDigitalOutput(LED_LED4_IOMUX);

    DL_GPIO_initDigitalOutput(LED_LED5_IOMUX);

    DL_GPIO_initDigitalOutput(LED_LED6_IOMUX);

    DL_GPIO_initDigitalOutput(LED_LED7_IOMUX);

    DL_GPIO_setPins(LED_PORT, LED_LED0_PIN |
		LED_LED1_PIN |
		LED_LED2_PIN |
		LED_LED3_PIN |
		LED_LED4_PIN |
		LED_LED5_PIN |
		LED_LED6_PIN |
		LED_LED7_PIN);
    DL_GPIO_enableOutput(LED_PORT, LED_LED0_PIN |
		LED_LED1_PIN |
		LED_LED2_PIN |
		LED_LED3_PIN |
		LED_LED4_PIN |
		LED_LED5_PIN |
		LED_LED6_PIN |
		LED_LED7_PIN);
}


void controlLED(int led_id, int state) 
{
	DL_GPIO_setPins(LED_PORT, LED_LED0_PIN|LED_LED1_PIN|LED_LED2_PIN|LED_LED3_PIN|LED_LED4_PIN|LED_LED5_PIN|LED_LED6_PIN|LED_LED7_PIN);
    switch (led_id) 
    {
        case 0:
            if (state == 1) 
            {
                // ʹ LED 0 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 0 ��
                DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
            }
            break;

        case 1:
            if (state == 1) 
            {
                // ʹ LED 1 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 1 ��
                DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);
            }
            break;

        case 2:
            if (state == 1) 
            {
                // ʹ LED 2 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED2_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 2 ��
                DL_GPIO_setPins(LED_PORT, LED_LED2_PIN);
            }
            break;

        case 3:
            if (state == 1) 
            {
                // ʹ LED 3 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED3_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 3 ��
                DL_GPIO_setPins(LED_PORT, LED_LED3_PIN);
            }
            break;

        case 4:
            if (state == 1) 
            {
                // ʹ LED 4 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED4_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 4 ��
                DL_GPIO_setPins(LED_PORT, LED_LED4_PIN);
            }
            break;

        case 5:
            if (state == 1) 
            {
                // ʹ LED 5 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED5_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 5 ��
                DL_GPIO_setPins(LED_PORT, LED_LED5_PIN);
            }
            break;

        case 6:
            if (state == 1) 
            {
                // ʹ LED 6 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED6_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 6 ��
                DL_GPIO_setPins(LED_PORT, LED_LED6_PIN);
            }
            break;

        case 7:
            if (state == 1) 
            {
                // ʹ LED 7 ��
                DL_GPIO_clearPins(LED_PORT, LED_LED7_PIN);
            } 
            else if (state == 0) 
            {
                // ʹ LED 7 ��
                DL_GPIO_setPins(LED_PORT, LED_LED7_PIN);
            }
            break;

        default:
            // ����ṩ�� led_id ��Ч�������ڴ˴����ػ������
            break;
    }
}

void delay_ms(int x)
{
  delay_cycles(CLK_HZ / 1000 * x);
}

void delay_us(int x)
{
  delay_cycles(CLK_HZ / 1000000 * x);
}