#include "my_adc.h"

void ADC0_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);		//使能 ADC0 外设
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);	//使能 GPIOE 外设
	MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);	//配置 ADC0 外围设备使用的引脚PE3
	
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);	//配置触发源和优先级
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);	//配置样本测序仪
	ADCSequenceEnable(ADC0_BASE, 3);	//使能采样序列
	ADCIntClear(ADC0_BASE, 3);			//清除采样序列中断源
}

uint16_t ADC0_Get(void)
{
	uint32_t ADC0_Value[1];  //该数组必须和FIFO深度一样大
	
	ADCProcessorTrigger(ADC0_BASE, 3);  //触发ADC转换
	while(!ADCIntStatus(ADC0_BASE, 3, false));  //等待转换完成
	ADCIntClear(ADC0_BASE, 3);  //清除采样序列中断源
	ADCSequenceDataGet(ADC0_BASE, 3, ADC0_Value);  //数据读取
	//UARTprintf("ADC0_Value[0] = %d\r\n",ADC0_Value[0]);
	return ADC0_Value[0];
}

