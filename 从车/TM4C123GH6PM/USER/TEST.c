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
#include "oled.h"
#include "beep.h"

float speedL ;  // ��ȡ�����ٶ�
float speedR ;  // ��ȡ�����ٶ�
//int lap_count = 0;         // ��¼����ɵ�Ȧ��
//int checkpoint_count = 0;  // ��¼��ǰȦ�ھ����ļ�������
//int prev_flag = 0;         // ��¼�Ƿ�վ������㣬��ֹ�ظ�����
float speedL ;  // ��ȡ�����ٶ�
float speedR ;  // ��ȡ�����ٶ�
int main(void)
{
	MAP_FPUEnable();  // �������㵥Ԫ
    MAP_FPULazyStackingEnable();  // ʹ��FPU���������㵥Ԫ��
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);  // ��������ʱ�ӣ�2.5��Ƶ��Ϊ80MHz
	uart0_Init(115200, 16000000);//printf�ض��򴮿�
	uart1_Init(115200, 16000000);//��������Э��
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
//	OLED_ShowString(0, 0, "TM4C123G", 16);  // ��ʾ 16 ������
//	OLED_Refresh_Gram(); // ˢ�� OLED
	Pid_Init(&Grayscale_Direction_Pid,&PID_Value_Grayscale_Direction[1]);
	Pid_Init(&Distance_Pid,&PID_Value_Distance[0]);
//	Pid_Init(&UWB_Angle_Pid,&PID_Value_UWB_Angle[0]);
//	Pid_Init(&UWB_Distance_Pid,&PID_Value_UWB_Distance[0]);
//	UWB_Angle_Pid.SetPoint = 60;
//	UWB_Distance_Pid.SetPoint = 100;
	KEY_Init();
    while(1)
	{ 

//Get_Grayscale_Offset();
//		Track_Direction_Control(85);  // 0.3m/s
//		printf("%d\r\n",g_u32Distance);
//		delay_ms(300);
//		Track_Direction_Control1(140);
//		Track_Direction_Distance_Control(85);
//		Menu_Process();
//	Track_Direction_Distance_Control2(215);
//			Track_Direction_Control2(140);
//	while (!(PB2 && PB3 && PE4 && PE5 && PD2 && PD3 && PE2)) 
//	{
//		Track_Direction_Distance_Control1(140);
//	}
//	while (!(PB2 && PB3 && PE4 && PE5 && PD2 && PD3 && PE2)) 
//	{
//		Track_Direction_Control2(140);
//	}
//	while (!(PB2 && PB3 && PE4 && PE5 && PD2 && PD3 && PE2)) 
//	{
//		Track_Direction_Distance_Control1(130);
//	}
//    // ��ʻ 1 Ȧ��ֹͣС��
//	itemFlag = 0;
//	L1_Speed_Pid.SetPoint = 0;
//	R1_Speed_Pid.SetPoint = 0;
		if(c == '1')
        {
		   task1();

        }
        else if(c == '2')
        {
			task2();
        }
        else if(c == '3')
        {
			task3();
        }
		else if(c == '4')
        {
			task4();
        }

    }
}
