#include "sys.h"
#include "led.h"
#include "key.h"
#include "pid.h"
#include "ccd.h"
#include "adc.h"
#include "oled.h"
#include "fifo.h"
#include "usart.h"
#include "delay.h"
#include "motor.h"
#include "timer.h"
#include "track.h"
#include "debug.h"
#include "mymath.h"
#include "usart3.h"
#include "usart6.h"
#include "encoder.h"
#include "control.h"
#include "keyboard.h"
#include "uart5_pc.h"
#include "vision_u4.h"
#include "bluetooch_u2.h"
#include "stepper_motor.h"
//#include "ZDT_X42_V2.h"
#include "Emm_V5.h"

int SPEED = 2000;

int main(void)
{
	uint8_t key;
//	int Key_Value = 0;
	// 定义实时位置变量
	// 定义接收数据数组、接收数据长度
//	uint8_t rxCmd[128] = {0}; uint8_t rxCount = 0;
  
	Stm32_Clock_Init(336, 8, 2, 7);
	delay_init(168);
	uart_init(84, 115200);
//	usart2_init(42, 115200);  // 蓝牙串口
//	usart3_init(42, 115200);	
//	uart4_init(42, 115200);  // 视觉部分
//	uart5_init(42, 115200);  // 串口5初始化,用作上位机通信
//	usart6_init(84, 115200);
	
	Led_Init();
	Key_Init();
//	CCD_Init();
//	OLED_Init();
//	Line7_Init();
//	Motor_Init();
//	KeyBoard_Init();
//	TIM7_Int_Init(65535,0); //arr:1000->1ms
//	TIM6_Int_Init(65535,0);
//	TIM9_Int_Init(65535,0);
	Stepper_Motor_Init();
//	Encoder_Init_TIM2();
//	Pid_Init( &Encoder1_Pid, &Encoder1_PID_Value_Encoder ); // 初始化PID参数，步进电机编码器1控制
//	Pid_Init( &Encoder2_Pid, &Encoder2_PID_Value_Encoder ); // 初始化PID参数，摆杆编码器2控制
//	
	delay_ms(100);
	while(1)
	{
		delay_ms (100);
		LED1 =!LED1 ;
        if(KEY_UP)                        /* 按下KEY0开始插补 */
        {
			delay_ms (35);
			printf ("456\r\n");
			LED1 = 0;
			LED2 = 0;
//			arc_incmove(800 * 5, 0, 0, -800 * 5, 2000, CW);     /* 第四象限圆弧 */
//            while(g_motor_sta);
//            arc_incmove(0, -800 * 5, -800 * 5, 0, 2000, CW);    /* 第三象限圆弧 */   
//            while(g_motor_sta);
//            arc_incmove(-800 * 5, 0, 0, 800 * 5, 2000, CW);     /* 第二象限圆弧 */
//            while(g_motor_sta);
//            arc_incmove(0, 800 * 5, 800 * 5, 0, 2000, CW);      /* 第一象限圆弧 */
//            while(g_motor_sta);     
            line_inpolation(1*10,1*1, SPEED);   /* 第一象限直线类似图形'/'  */
            while(g_motor_sta);
//            line_inpolation(2*1600, -2*1600, 2000); /* 第四象限直线类似图形'\'  */
//            while(g_motor_sta);
//            line_inpolation(-2*1600,-2*1600, 500); /* 第三象限直线类似图形'/'  */
//            while(g_motor_sta);
//            line_inpolation(-2*1600, 2*1600,500);  /* 第二象限直线类似图形'\'  */
////            while(g_motor_sta);  
			printf ("123\r\n");			
			key =0;
			delay_ms (35);
        }
		else if(KEY0)
		{
			delay_ms (15);
			if(KEY0)
			{
				SPEED += 10;
				printf ("SPEED  %d\r\n",SPEED );			
			}
			delay_ms (15);
		}
		else if(KEY1)
		{
			delay_ms (15);
			if(KEY1)
			{
				SPEED -= 10;
				printf ("SPEED  %d\r\n",SPEED );			
			}
			delay_ms (15);
		}
//		LED2 = 1;
//		LED1 = 1;
	}	
		
}

