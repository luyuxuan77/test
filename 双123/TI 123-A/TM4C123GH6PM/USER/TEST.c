// Update date��2023/7/30
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
int main(void)
{
	//MAP_��ͷ�Ĳ���ʱ����ʡ�ԡ�MAP_������������������Լ��ݲ�ͬ�汾�ĺ�����װ
	MAP_FPUEnable();  // �������㵥Ԫ
    MAP_FPULazyStackingEnable();  // ʹ��FPU���������㵥Ԫ��
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);  // ��������ʱ�ӣ�2.5��Ƶ��Ϊ80MHz
	uart0_Init(115200, 16000000);//printf�ض��򴮿�
//	uart1_Init(115200, 16000000);//��������Э��
//	uart7_Init(115200, 16000000);
	RGB_Init();
//	KEY_Init();
//	Line7_Init();
	Motor_Init();
//QEI0_Init();
//QEI1_Init();
//Pid_Init(&L1_Speed_Pid, &L1_PID_Value_Speed);
//L1_Speed_Pid.SetPoint = 150;
//Pid_Init(&R1_Speed_Pid, &R1_PID_Value_Speed);
//R1_Speed_Pid.SetPoint = 150;
//	Pid_Init(&Grayscale_Direction_Pid,&PID_Value_Grayscale_Direction[0]);
//	Pid_Init(&UWB_Angle_Pid,&PID_Value_UWB_Angle[0]);
//	Pid_Init(&UWB_Distance_Pid,&PID_Value_UWB_Distance[0]);
//	UWB_Angle_Pid.SetPoint = 60;
//	UWB_Distance_Pid.SetPoint = 100;
	
    while(1)
	{
//		printf("1");
//		Grren_Light_Control(0);
//		Blue_Light_Control(1);
//		delay_ms(500);
//		Grren_Light_Control(1);
//		Blue_Light_Control(0);
//		delay_ms(500);
//		Track_Angle_Distance_Control();
    }
}
