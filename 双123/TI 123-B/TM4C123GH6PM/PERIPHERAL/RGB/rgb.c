#include "rgb.h"

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

