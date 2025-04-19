// Update date：2023/7/30
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

//int lap_count = 0;         // 记录已完成的圈数
//int checkpoint_count = 0;  // 记录当前圈内经过的监测点数量
//int prev_flag = 0;         // 记录是否刚经过监测点，防止重复触发
float speedL ;  // 获取左轮速度
float speedR ;  // 获取右轮速度
int main(void)
{
	MAP_FPUEnable();  // 启动浮点单元
    MAP_FPULazyStackingEnable();  // 使能FPU（浮点运算单元）
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);  // 设置外设时钟，2.5分频后为80MHz
	uart0_Init(115200, 16000000);//printf重定向串口
//	uart1_Init(115200, 16000000);//蓝牙滑条协议
//	uart7_Init(115200, 16000000);
	RGB_Init();
	Timer0_Init(1);
	Buzzer_Init();
	Line7_Init();
//	OLED_Init();
	Motor_Init();
	QEI0_Init();
	QEI1_Init();
	Pid_Init(&L1_Speed_Pid, &L1_PID_Value_Speed);
	L1_Speed_Pid.SetPoint = 0;
	Pid_Init(&R1_Speed_Pid, &R1_PID_Value_Speed);
	R1_Speed_Pid.SetPoint = 0;
//	OLED_ShowString(0, 0, "TM4C123G", 16);  // 显示 16 号字体
//	OLED_Refresh_Gram(); // 刷新 OLED
	Pid_Init(&Grayscale_Direction_Pid,&PID_Value_Grayscale_Direction[1]);
//	Pid_Init(&UWB_Angle_Pid,&PID_Value_UWB_Angle[0]);
//	Pid_Init(&UWB_Distance_Pid,&PID_Value_UWB_Distance[0]);
//	UWB_Angle_Pid.SetPoint = 60;
//	UWB_Distance_Pid.SetPoint = 100;
	KEY_Init();
    while(1)
	{ 
//Get_Grayscale_Offset();
//		Track_Direction_Control(85);  // 0.3m/s
//		Track_Direction_Control3(140);
		Menu_Process();
//		while(!(PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2))
//		{
//		Buzzer_Off(); // 关闭蜂鸣器
//		}
//		Buzzer_On();        // 触发蜂鸣器
    }
}
