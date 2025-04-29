#include "exti.h"
#include "bsp.h"

//void EXTI_init(void)
//{
////	DL_GPIO_reset(GPIOA);

////    DL_GPIO_enablePower(GPIOA);
////    delay_cycles(POWER_STARTUP_DELAY);
//	
//	DL_GPIO_initDigitalInputFeatures(GPIO_GRP_0_PIN_0_IOMUX,
//		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
//		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

//    DL_GPIO_initDigitalInputFeatures(GPIO_GRP_1_PIN_1_IOMUX,
//		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
//		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

//    DL_GPIO_initDigitalInputFeatures(GPIO_GRP_2_PIN_2_IOMUX,
//		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_NONE,
//		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

//    DL_GPIO_setLowerPinsPolarity(GPIOA, DL_GPIO_PIN_0_EDGE_RISE_FALL |
//		DL_GPIO_PIN_1_EDGE_RISE_FALL |
//		DL_GPIO_PIN_2_EDGE_RISE_FALL);
//	
//    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_GRP_0_PIN_0_PIN |
//		GPIO_GRP_1_PIN_1_PIN |
//		GPIO_GRP_2_PIN_2_PIN);
//		
//    DL_GPIO_enableInterrupt(GPIOA, GPIO_GRP_0_PIN_0_PIN |
//		GPIO_GRP_1_PIN_1_PIN |
//		GPIO_GRP_2_PIN_2_PIN);
//		
//    /* INT_GROUP1 Priority */
//    NVIC_SetPriority(GPIOA_INT_IRQn, 0);
//	
//	NVIC_EnableIRQ(GPIOA_INT_IRQn);
//}


void EXTI_init(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_enablePower(GPIOA);
//	DL_GPIO_initDigitalInput(GPIO_PA0_PIN_0_IOMUX);
//	DL_GPIO_initDigitalInput(GPIO_PA1_PIN_1_IOMUX);
//	DL_GPIO_initDigitalInput(GPIO_PA2_PIN_2_IOMUX);

	DL_GPIO_initDigitalInputFeatures(GPIO_PA0_PIN_0_IOMUX,
	DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
	DL_GPIO_HYSTERESIS_ENABLE, DL_GPIO_WAKEUP_DISABLE);

	DL_GPIO_initDigitalInputFeatures(GPIO_PA1_PIN_1_IOMUX,
	DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
	DL_GPIO_HYSTERESIS_ENABLE, DL_GPIO_WAKEUP_DISABLE);

	DL_GPIO_initDigitalInputFeatures(GPIO_PA2_PIN_2_IOMUX,
	DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
	DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

	DL_GPIO_setLowerPinsPolarity(GPIOA, DL_GPIO_PIN_0_EDGE_RISE_FALL |
										DL_GPIO_PIN_1_EDGE_RISE_FALL |
										DL_GPIO_PIN_2_EDGE_RISE_FALL);
	DL_GPIO_clearInterruptStatus(GPIOA, GPIO_PA0_PIN_0_PIN |
										GPIO_PA1_PIN_1_PIN |
										GPIO_PA2_PIN_2_PIN);
	DL_GPIO_enableInterrupt(GPIOA, GPIO_PA0_PIN_0_PIN |
									GPIO_PA1_PIN_1_PIN |
									GPIO_PA2_PIN_2_PIN);
//    DL_GPIO_setLowerPinsPolarity(GPIO_PA_PORT, DL_GPIO_PIN_0_EDGE_RISE_FALL);
//    DL_GPIO_setLowerPinsPolarity(GPIO_PA_PORT, DL_GPIO_PIN_1_EDGE_RISE_FALL);
//	DL_GPIO_setLowerPinsPolarity(GPIO_PA_PORT, DL_GPIO_PIN_2_EDGE_RISE_FALL);
//	
//    DL_GPIO_clearInterruptStatus(GPIO_PA_PORT, GPIO_PA0_PIN_0_PIN);
//    DL_GPIO_clearInterruptStatus(GPIO_PA_PORT, GPIO_PA1_PIN_1_PIN);
//    DL_GPIO_clearInterruptStatus(GPIO_PA_PORT, GPIO_PA2_PIN_2_PIN);

//    DL_GPIO_enableInterrupt(GPIO_PA_PORT, GPIO_PA0_PIN_0_PIN);
//	DL_GPIO_enableInterrupt(GPIO_PA_PORT, GPIO_PA1_PIN_1_PIN);
//    DL_GPIO_enableInterrupt(GPIO_PA_PORT, GPIO_PA2_PIN_2_PIN);

	//	* INT_GROUP1 Priority */
	//	NVIC_SetPriority(GPIOA_INT_IRQn, 0);
	NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

//void GROUP1_IRQHandler(void)
//{
//  switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
//  {
//  case KEY_INT_IIDX:
//    delay_ms(10); // 按键消抖延迟，一般为10ms

//    if (!DL_GPIO_readPins(KEY_PORT, KEY_KEY0_PIN))
//    {
//      clk_ch = (clk_ch + 1) % 4; // 切换时钟选择
//      clk_flag = 1;
//    }
//    break;
//  default:
//    break;
//  }
//  	DL_UART_Main_clearInterruptStatus(UART0,DL_UART_MAIN_IIDX_RX);

//}
int num;
void GROUP1_IRQHandler(void)//Group1的中断服务函数
{
	uint32_t gpioA = DL_GPIO_getEnabledInterruptStatus(GPIOA,GPIO_PA0_PIN_0_PIN|GPIO_PA1_PIN_1_PIN|GPIO_PA2_PIN_2_PIN);
	
	if((gpioA & GPIO_PA0_PIN_0_PIN)==GPIO_PA0_PIN_0_PIN)
	{
//		num=(DL_GPIO_readPins(GPIOA,GPIO_PA0_PIN_0_PIN)<<2)|(DL_GPIO_readPins(GPIOA,GPIO_PA1_PIN_1_PIN)<<1)|(DL_GPIO_readPins(GPIOA,GPIO_PA2_PIN_2_PIN)<<0);
//		printf("%d",num);
		if(DL_GPIO_readPins(GPIOA,GPIO_PA0_PIN_0_PIN))
		{
			BH_AL(1400);
		}
		else if(DL_GPIO_readPins(GPIOA,GPIO_PA0_PIN_0_PIN)==0)
		{
			AH_BL(1400);
		}
		DL_GPIO_clearInterruptStatus(GPIOA,GPIO_PA0_PIN_0_PIN);
	}
	if((gpioA & GPIO_PA1_PIN_1_PIN)==GPIO_PA1_PIN_1_PIN)
	{
//		num=(DL_GPIO_readPins(GPIOA,GPIO_PA0_PIN_0_PIN)<<2)|(DL_GPIO_readPins(GPIOA,GPIO_PA1_PIN_1_PIN)<<1)|(DL_GPIO_readPins(GPIOA,GPIO_PA2_PIN_2_PIN)<<0);
//		printf("2");
		if(DL_GPIO_readPins(GPIOA,GPIO_PA1_PIN_1_PIN))
		{
			BH_CL(1400);
		}
		else if(DL_GPIO_readPins(GPIOA,GPIO_PA1_PIN_1_PIN)==0)
		{
			CH_BL(1400);
		}
		DL_GPIO_clearInterruptStatus(GPIOA,GPIO_PA1_PIN_1_PIN);
	}
		if((gpioA & GPIO_PA2_PIN_2_PIN)==GPIO_PA2_PIN_2_PIN)
	{
//		num=(DL_GPIO_readPins(GPIOA,GPIO_PA0_PIN_0_PIN)<<2)|(DL_GPIO_readPins(GPIOA,GPIO_PA1_PIN_1_PIN)<<1)|(DL_GPIO_readPins(GPIOA,GPIO_PA2_PIN_2_PIN)<<0);
//		printf("3");
		if(DL_GPIO_readPins(GPIOA,GPIO_PA2_PIN_2_PIN))
		{
			AH_CL(1400);
		}
		else if(DL_GPIO_readPins(GPIOA,GPIO_PA2_PIN_2_PIN)==0)
		{
			CH_AL(1400);
		}
		DL_GPIO_clearInterruptStatus(GPIOA,GPIO_PA2_PIN_2_PIN);
	}
//	DL_GPIO_clearInterruptStatus(GPIOA,GPIO_PA2_PIN_2_PIN|GPIO_PA2_PIN_1_PIN|GPIO_PA2_PIN_2_PIN);

}