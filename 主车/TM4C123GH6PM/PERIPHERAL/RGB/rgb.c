#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart1.h"
#include "my_timer.h"
#include "control.h"
#include "motor.h"
#include "uart0.h"
#include "ccd.h"
#include "rgb.h"
#include "key.h"
#include "encoder.h"
#include "track.h"
#include "State_Machine.h"
#include "uart7.h"
#include "distance.h"
#include "oled.h"


/**************************************************************************
�������ܣ�����RGB�Ƴ�ʼ��
����˵����RGB�����������Ž��п��ƣ�����Դ������������ѡȡ�������Ž���ʹ��
**************************************************************************/
void RGB_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // ʹ�� GPIOF ����
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);  // ʹ��PF2��PF3����
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0);  // Ĭ�����Ͳ���
}

/******************************************************************************
�������ܣ�RGB���̵�״̬����
��ڲ�����mode��0�̵���1�̵���
����  ֵ����
*******************************************************************************/ 
void Grren_Light_Control(unsigned char mode)
{
	if (mode == 0)	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
	else	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
}

/******************************************************************************
�������ܣ�RGB������״̬����
��ڲ�����mode��0������1������
����  ֵ����
*******************************************************************************/ 
void Blue_Light_Control(unsigned char mode)
{
	if (mode == 0)	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	else	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

void LED_FlashNTimes(int times)
{
    for (int i = 0; i < times; i++)
    {
        Blue_Light_Control(1); // LED ��
        delay_ms(100); 
        Blue_Light_Control(0); // LED ��
		delay_ms(80);
    }
}
