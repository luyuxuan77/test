#include "rgb.h"

/**************************************************************************
函数功能：板载RGB灯初始化
函数说明：RGB采用三个引脚进行控制，若资源不足可以则可以选取部分引脚进行使能
**************************************************************************/
void RGB_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  // 使能 GPIOF 外设
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);  // 使能PF2、PF3引脚
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0);  // 默认拉低不亮
}

/******************************************************************************
函数功能：RGB灯绿灯状态控制
入口参数：mode：0绿灯灭，1绿灯亮
返回  值：无
*******************************************************************************/ 
void Grren_Light_Control(unsigned char mode)
{
	if (mode == 0)	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
	else	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
}

/******************************************************************************
函数功能：RGB灯蓝灯状态控制
入口参数：mode：0蓝灯灭，1蓝灯亮
返回  值：无
*******************************************************************************/ 
void Blue_Light_Control(unsigned char mode)
{
	if (mode == 0)	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
	else	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

