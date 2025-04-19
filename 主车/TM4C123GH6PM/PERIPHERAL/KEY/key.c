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
#include "beep.h"




/* 自定义解锁函数（根据实际需要） */
void GPIO_Unlock(uint32_t ui32Port, uint8_t ui8Pins)
{
    HWREG(ui32Port + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
    HWREG(ui32Port + GPIO_O_CR) |= ui8Pins;
    HWREG(ui32Port + GPIO_O_LOCK) = 0;
}

/* 全局变量 */
uint8_t itemFlag = 0;
volatile int lap_count = 0;         // 记录已完成的圈数
volatile int checkpoint_count = 0;  // 记录当前圈内经过的监测点数量
volatile int prev_flag = 0;         // 记录是否刚经过监测点，防止重复触发
volatile int key_flag = 0;  // 按键标志位
volatile int flag = 1;  // 按键标志位

volatile int key1 = 0;  // 按键标志位
volatile int key2 = 0;  // 按键标志位


/* 中断服务函数 */
void PortF_Interrupt(void)
{
    uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE, true);

    if ((s & GPIO_PIN_4) && key_flag == 0)  // PF4触发且无其他按键活动
    {
        key_flag = 1;  // 置位，防止多个按键干扰
        SysCtlDelay(SysCtlClockGet() / 1000);    // 消抖延时
        while (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)) {} // 等待释放
        SysCtlDelay(SysCtlClockGet() / 1000);
//        printf("PF4 Pressed\n");
		key1 = 1;
        key_flag = 0;  // 释放标志
    }

    // 清除PF4中断
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
}

void PortE_Interrupt(void)
{
    uint32_t s = GPIOIntStatus(GPIO_PORTE_BASE, true);

    if ((s & GPIO_PIN_3) && key_flag == 0)  // PE3触发且无其他按键活动
    {
        key_flag = 1;  // 置位，防止多个按键干扰
        SysCtlDelay(SysCtlClockGet() / 2000);    // 消抖延时
        while (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3)) {} // 等待释放
        SysCtlDelay(SysCtlClockGet() / 2000);
//        printf("PE3 Pressed\n");
		key2 = 1;
        key_flag = 0;  // 释放标志
    }

    // 清除PE3中断
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_3);
}

/* 按键初始化 */
void KEY_Init(void)
{
    // 使能GPIOF和GPIOE时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // 配置PF4
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, 
                    GPIO_PIN_4,
                    GPIO_STRENGTH_2MA, 
                    GPIO_PIN_TYPE_STD_WPU);

    // 配置PE3
    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTE_BASE, 
                    GPIO_PIN_3,
                    GPIO_STRENGTH_2MA, 
                    GPIO_PIN_TYPE_STD_WPU);

    // 配置PF4中断
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, PortF_Interrupt);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);

    // 配置PE3中断
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTE_BASE, PortE_Interrupt);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_3);

    // 使能全局中断
    IntEnable(INT_GPIOF);
    IntEnable(INT_GPIOE);
    IntMasterEnable();
}

void Menu_Process(void)
{
    if (key1 == 1)
    {
        key1 = 0;
        itemFlag++;
        delay_ms(5);
    }

    if (key2 == 1)
    {  	
		printf("%d",itemFlag); 
        delay_ms(200); 
		Grren_Light_Control(1);
        key2 = 0;
        BEEP_BeepNTimes(itemFlag);
		Grren_Light_Control(0);
        SysCtlDelay(SysCtlClockGet() / 200); // 消抖延时
//        if (itemFlag > 0 && itemFlag <= 9)  // 确认题目选择
//        {
//            itemFlag = 0;
//        }
		
		switch (itemFlag)
		{
			case 1:
				task1();
				break;

			case 2:
				task2();
				break;

			case 3:
				task3();
				break;

			case 4:
				task4();
				break;

	//        case 5:
	//            task5();
	//            break;

	//        case 6:
	//            task6();
	//            break;

	//        case 7:
	//            task7();
	//            break;

	//        case 8:
	//            task8();
	//            break;

	//        case 9:
	//            task9();
	//            break;
		}
    }
    else // 如果没有按下 key2，则 LED 继续闪烁
	{
	LED_FlashNTimes(itemFlag);
	delay_ms(800);
	}

}

int task1(void)
{
	checkpoint_count=0;
	while (checkpoint_count < 1) 
	{  // 只跑 1圈
		Track_Direction_Control(85);  // 0.3m/s
		// 检测是否到达监测点（保持你的判断逻辑）
	// 检测是否到达监测点（根据新条件：所有7个引脚都为1时触发蜂鸣器）
		if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
		{
		Buzzer_On();        // 触发蜂鸣器
			delay_ms(40);
		prev_flag = 1;      // 记录触发状态，避免重复触发
			prev_flag = 0;
		Buzzer_Off(); // 关闭蜂鸣器
		checkpoint_count++; // 记录当前圈的监测点数
		}
    }
    // 行驶 1 圈后停止小车
			motor_4_speed(0,0,0,0);
	itemFlag = 0;
	printf("9");
	return 0;
}

int task2(void)
{
	checkpoint_count=0;
	while (checkpoint_count < 2) 
	{  // 只跑 1圈
	Track_Direction_Control1(130);  // 0.5m/s
	// 检测是否到达监测点（保持你的判断逻辑）
// 检测是否到达监测点（根据新条件：所有7个引脚都为1时触发蜂鸣器）
	if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
	{
    Buzzer_On();        // 触发蜂鸣器
		delay_ms(40);
    prev_flag = 1;      // 记录触发状态，避免重复触发
	    prev_flag = 0;
    Buzzer_Off(); // 关闭蜂鸣器
    checkpoint_count++; // 记录当前圈的监测点数
		delay_ms(5);
	} 
    }
    // 行驶 1 圈后停止小车
    motor_4_speed(0, 0, 0, 0);
	itemFlag = 0;
		printf("9");
	return 0;
}
int task3(void)
{
	checkpoint_count=0;
	while (checkpoint_count < 3) 
	{
		if(checkpoint_count==0)
		{
			Track_Direction_Control2(140);
		}
		if(checkpoint_count==1)
		{
			Track_Direction_Control2(137);
		}
		else if (checkpoint_count==2)
		{
			Track_Direction_Control3(155);
		}
	// 检测是否到达监测点（保持你的判断逻辑）
// 检测是否到达监测点（根据新条件：所有7个引脚都为1时触发蜂鸣器）
	if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
	{
    Buzzer_On();        // 触发蜂鸣器
		delay_ms(40);
		prev_flag = 1;      // 记录触发状态，避免重复触发
	    prev_flag = 0;
    Buzzer_Off(); // 关闭蜂鸣器
    checkpoint_count++; // 记录当前圈的监测点数
	}
    }
    // 行驶 1 圈后停止小车
	itemFlag = 0;
	printf("9");
	motor_4_speed(0,0,0,0);
	return 0;
}
//int task3(void)
//{
//	checkpoint_count=0;
//	while (checkpoint_count < 3) 
//	{  // 只跑 1圈
//	Track_Direction_Control1(140);  // 0.5m/s
//	// 检测是否到达监测点（保持你的判断逻辑）
//// 检测是否到达监测点（根据新条件：所有7个引脚都为1时触发蜂鸣器）
//	if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
//	{
//    Buzzer_On();        // 触发蜂鸣器
//		delay_ms(30);
//    prev_flag = 1;      // 记录触发状态，避免重复触发
//	    prev_flag = 0;
//    Buzzer_Off(); // 关闭蜂鸣器
//    checkpoint_count++; // 记录当前圈的监测点数
//	}
//	if(checkpoint_count==1)
//	{
//		printf("7");
//	}
//	if((PD0 && PB3) &&(flag == 1)&&(checkpoint_count==2))
//	{       
//		motor_4_speed(100, 100, 220, 220);
//		delay_ms(250);
//			flag=0;
//		printf("8");
//	}
//	
//    }
//    // 行驶 1 圈后停止小车
//	itemFlag = 0;
//	printf("9");
//	motor_4_speed(0,0,0,0);
//	return 0;
//}
int task4(void)
{
	while(!(PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2))
	{
		Track_Direction_Control2(210);
	}
	printf("5");
	motor_4_speed(0,0,0,0);
		Buzzer_On();  // 响
		delay_ms(500);
		Buzzer_Off(); // 停
		delay_ms(500);
		Buzzer_On();  // 响
		delay_ms(500);
		Buzzer_Off(); // 停
		delay_ms(500);
		Buzzer_On();  // 响
		delay_ms(500);
		Buzzer_Off(); // 停
		delay_ms(500);
		Buzzer_On();  // 响
		delay_ms(500);
		Buzzer_Off(); // 停
		delay_ms(500); 
		Buzzer_On();  // 响
		delay_ms(500);
		Buzzer_Off(); // 停
		delay_ms(500);
	while(!(PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2))
	{
		Track_Direction_Control2(210);
	}
	printf("9");
	motor_4_speed(0,0,0,0);
	return 0;
}
