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



//void GROUP1_IRQHandler(void)//Group1���жϷ�����
//{
//    //��ȡGroup1���жϼĴ���������жϱ�־λ
//    switch( DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1) )
//    {
//        //����Ƿ���KEY��GPIOA�˿��жϣ�ע����PORTB_INT_IIDX������PORTB_S1_IIDX
//        case GPIO_MULTIPLE_GPIOA_INT_IIDX:
//            //����������±�Ϊ�ߵ�ƽ
//            if( DL_GPIO_readPins(GPIOA, GPIO_GRP_1_PIN_1_PIN) < 1 )
//            {
//                //����LED����״̬��ת
//                AH_BL(1000);
//            }
//        break;
//    }
//}

//		 key_num = Key(); // ��ȡ����ֵ1-16��Ĭ��Ϊ0
//    if (key_num != 0)
//    {
//      // key_num-1ʹ��Χ��Ϊ0-15������2���Ʊ�ʾ
//      if ((key_num - 1) & 1) // תΪ2���ƺ�ĵ�0λ����11=b1011��b1011&b0001=b0001�����LED3����ͬ��LED2��LED0������LED1����
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED3_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED3_PIN);
//      }
//      if ((key_num - 1) & 2) // תΪ2���ƺ�ĵ�1λ
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED2_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED2_PIN);
//      }
//      if ((key_num - 1) & 4) // תΪ2���ƺ�ĵ�2λ
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);
//      }
//      if ((key_num - 1) & 8) // תΪ2���ƺ�ĵ�3λ
//      {
//        DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
//      }
//      else
//      {
//        DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
//      }

//      key_num = 0; // key_num�ָ�Ĭ��
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