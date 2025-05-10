#include "bsp.h"

uint8_t key_num = 0;
uint8_t Key();
extern int NUM;
int sum = 0;
int int_status=0;
int main(void)
{
    SYSCFG_DL_init();
//	LED_init();
//	//keyboard_init();
	OLED_Init();
	OLED_Clear();
	 OLED_ShowString(10, 0, "Hello");
	 OLED_ShowString(2, 14, "WE ARE WINNER!");
	OLED_ShowCHinese(12, 2, 3);
//	pwm_init();
//	uart0_init();
//	opa_init();
//	EXTI_init();
//	motor();
    while(1) 
	{
//	printf("1");
	}
}



//void GROUP1_IRQHandler(void)//Group1的中断服务函数
//{
//    //读取Group1的中断寄存器并清除中断标志位
//    switch( DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1) )
//    {
//        //检查是否是KEY的GPIOA端口中断，注意是PORTB_INT_IIDX，不是PORTB_S1_IIDX
//        case GPIO_MULTIPLE_GPIOA_INT_IIDX:
//            //如果按键按下变为高电平
//            if( DL_GPIO_readPins(GPIOA, GPIO_GRP_1_PIN_1_PIN) < 1 )
//            {
//                //设置LED引脚状态翻转
//                AH_BL(1000);
//            }
//        break;
//    }
//}

//		 key_num = Key(); // 读取按键值1-16，默认为0
//    if (key_num != 0)
//    {
//      // key_num-1使范围变为0-15，可用2进制表示
//      if ((key_num - 1) & 1) // 转为2进制后的第0位，如11=b1011，b1011&b0001=b0001，因此LED3亮，同理LED2和LED0亮，而LED1不亮
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED3_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED3_PIN);
//      }
//      if ((key_num - 1) & 2) // 转为2进制后的第1位
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED2_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED2_PIN);
//      }
//      if ((key_num - 1) & 4) // 转为2进制后的第2位
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);
//      }
//      if ((key_num - 1) & 8) // 转为2进制后的第3位
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
//      }

//      key_num = 0; // key_num恢复默认
//    }


//				controlLED(0,1);    
//				delay_ms(200);
//				controlLED(1,1);    
//				delay_ms(200);
//				controlLED(2,1);    
//				delay_ms(200);
//				controlLED(3,1);    
//				delay_ms(200);
//				controlLED(4,1);    
//				delay_ms(200);
//				controlLED(5,1);    
//				delay_ms(200);
//				controlLED(6,1);    
//				delay_ms(200);
//				controlLED(7,1);    
//				delay_ms(200);