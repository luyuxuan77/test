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
volatile int checkpoint_count = 0;  // 记录当前圈内经过的监测点数量
volatile int flag = 1;  // 按键标志位

Distance_TypeDef Distance_Data;

unsigned int Get_Distance(void)
{
	return g_u32Distance;
}

int Ger_Distance_Offset(unsigned int Distance)
{
	Distance_Data.Distance_length = Get_Distance();
//	printf("%d                              %d\r\n",g_u32Distance,Distance_Data.Distance_length);
	return Distance_Data.Distance_offset = Distance_Data.Distance_length - Distance ;
}

PID_TypeDef Distance_Pid;/*!pid算法!中用到的参数集合*/
p_i_d_Value_TypeDef PID_Value_Distance[3] = {{ 4, 0, 0 },	//预存三组pid参数，用于不同的巡线速度
														{ 0.0, 0.0, 0.0 },/*比例，积分，微分3值大小结构体*/
														{ 0.0, 0.0, 0.0 }};

void Track_Direction_Distance_Control(float speed)
{
	Get_Grayscale_Offset();
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 40);
	
	Ger_Distance_Offset(20);
//	printf("%d\r\n",Distance_Data.Distance_offset);
	Pid_control(&Distance_Pid, Distance_Data.Distance_offset, 0);
	Pid_OutLimit(&Distance_Pid, 15);
//	printf("%f\r\n",Distance_Pid.ActualValue);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue - Distance_Pid.ActualValue*1.2;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue - Distance_Pid.ActualValue*1.2;
	
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}
void Track_Direction_Distance_Control1(float speed)
{
	Get_Grayscale_Offset();
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 55);
	
	Ger_Distance_Offset(20);
//	printf("%d\r\n",Distance_Data.Distance_offset);
	Pid_control(&Distance_Pid, Distance_Data.Distance_offset, 0);
	Pid_OutLimit(&Distance_Pid, 30);
//	printf("%f\r\n",Distance_Pid.ActualValue);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue*1.45 - Distance_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue*1.45 - Distance_Pid.ActualValue;
	
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}
void Track_Direction_Distance_Control2(float speed)
{
	Get_Grayscale_Offset();
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 60);
	
	Ger_Distance_Offset(20);
//	printf("%d\r\n",Distance_Data.Distance_offset);
	Pid_control(&Distance_Pid, Distance_Data.Distance_offset, 0);
	Pid_OutLimit(&Distance_Pid, 30);
//	printf("%f\r\n",Distance_Pid.ActualValue);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue*1.48 - Distance_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue*1.48 - Distance_Pid.ActualValue;
	
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}
void Track_Direction_Distance_Control3(float speed)
{
	Get_Grayscale_Offset1();
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 60);
	
	Ger_Distance_Offset(20);
//	printf("%d\r\n",Distance_Data.Distance_offset);
	Pid_control(&Distance_Pid, Distance_Data.Distance_offset, 0);
	Pid_OutLimit(&Distance_Pid, 30);
//	printf("%f\r\n",Distance_Pid.ActualValue);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue*1.48 - Distance_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue*1.48 - Distance_Pid.ActualValue;
	
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}

int task1(void)
{
	LED_FlashNTimes(1);
	BEEP_BeepNTimes(1);
	while(c == '1')
	{
	Track_Direction_Distance_Control(85);
	}
	L1_Speed_Pid.SetPoint = 0;
	R1_Speed_Pid.SetPoint = 0;
	return 0;
}
int task2(void)
{
	LED_FlashNTimes(2);
	BEEP_BeepNTimes(2);
	while(c == '2')
	{
	Track_Direction_Distance_Control1(140);
	}
	L1_Speed_Pid.SetPoint = 0;
	R1_Speed_Pid.SetPoint = 0;
	return 0;
}
int task3(void)
{ 
	checkpoint_count=0;
	LED_FlashNTimes(3);
	delay_ms(150);

	while (c == '3') 
	{
			// 检测是否到达监测点（保持你的判断逻辑）
// 检测是否到达监测点（根据新条件：所有7个引脚都为1时触发蜂鸣器）
	if (PB2 && PB3 && PE4 && PE5 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
	{
    Buzzer_On();        // 触发蜂鸣器
		delay_ms(40);
		prev_flag = 1;      // 记录触发状态，避免重复触发
	    prev_flag = 0;
    Buzzer_Off(); // 关闭蜂鸣器
    checkpoint_count++; // 记录当前圈的监测点数
		delay_ms(70);
	}
		if(checkpoint_count==3)
		{
		Track_Direction_Control2(137);
		}
		else
		{
 		Track_Direction_Distance_Control1(142);
		}

    }
    // 行驶 1 圈后停止小车
	itemFlag = 0;
	L1_Speed_Pid.SetPoint = 0;
	R1_Speed_Pid.SetPoint = 0;
	return 0;
}
//int task3(void)
//{
//	LED_FlashNTimes(3);
//	BEEP_BeepNTimes(3);
//	while (c == '3') 
//	{  // 只跑 1圈
//	Track_Direction_Distance_Control1(140);  // 0.5m/s
//    }
//	while(c == '7')
//	{
//	Track_Direction_Control2(140);  
//	}
//	while (c == '8') 
//	{  // 只跑 1圈
//	Track_Direction_Distance_Control1(140);  // 0.5m/s
//    }
//	motor_4_speed(0,0,0,0);
//	return 0;
//}
int task4(void)
{
	LED_FlashNTimes(4);
		delay_ms(250);
	while(c == '4')
	{
	Track_Direction_Distance_Control2(205);
	}
	L1_Speed_Pid.SetPoint = 0;
	R1_Speed_Pid.SetPoint = 0;
	delay_ms(5000);
	while(c == '5')
	{
	Track_Direction_Distance_Control2(210);
	}
	L1_Speed_Pid.SetPoint = 0;
	R1_Speed_Pid.SetPoint = 0;
	return 0;
}