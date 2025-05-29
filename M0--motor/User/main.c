#include "ti_msp_dl_config.h"
#include "bsp.h"

int main(void)
{
	SYSCFG_DL_init();
	led_init();
	OLED_Init();
	RGB_init();
	key_init();
	motor_init();
	gray_init();
	tim_g0_init();  //0.5毫秒中断
	tim_g6_init();  //12.5毫中断PID计算
	uart0_init(115200);  //串口打印+陀螺仪
	uart1_init(115200);  //步进电机01,02
	uart2_init(9600);  //从f103获取编码器值
	uart3_init(115200);  //摄像头
	Pid_Init(&L1_Speed_Pid,&L1_PID_Value_Speed);
	Pid_Init(&R1_Speed_Pid,&R1_PID_Value_Speed);
	Pid_Init(&L2_Speed_Pid,&L2_PID_Value_Speed);
	Pid_Init(&R2_Speed_Pid,&R2_PID_Value_Speed);
	Pid_Init(&Grayscale_Direction_Pid,&PID_Value_Grayscale_Direction[0]);
	L1_Speed_Pid.SetPoint = 0;
	R1_Speed_Pid.SetPoint = 0;
	L2_Speed_Pid.SetPoint = 0;
	R2_Speed_Pid.SetPoint = 0;

    while(1)
	{			
		Control(Key_Value);
	}
}

 