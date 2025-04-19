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

void LED_FlashNTimes(int times)
{
    for (int i = 0; i < times; i++)
    {
        Blue_Light_Control(1); // LED 亮
        delay_ms(100); 
        Blue_Light_Control(0); // LED 灭
		delay_ms(80);
    }
}
