#include "my_adc.h"

void ADC0_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);		//ʹ�� ADC0 ����
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);	//ʹ�� GPIOE ����
	MAP_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);	//���� ADC0 ��Χ�豸ʹ�õ�����PE3
	
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);	//���ô���Դ�����ȼ�
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);	//��������������
	ADCSequenceEnable(ADC0_BASE, 3);	//ʹ�ܲ�������
	ADCIntClear(ADC0_BASE, 3);			//������������ж�Դ
}

uint16_t ADC0_Get(void)
{
	uint32_t ADC0_Value[1];  //����������FIFO���һ����
	
	ADCProcessorTrigger(ADC0_BASE, 3);  //����ADCת��
	while(!ADCIntStatus(ADC0_BASE, 3, false));  //�ȴ�ת�����
	ADCIntClear(ADC0_BASE, 3);  //������������ж�Դ
	ADCSequenceDataGet(ADC0_BASE, 3, ADC0_Value);  //���ݶ�ȡ
	//UARTprintf("ADC0_Value[0] = %d\r\n",ADC0_Value[0]);
	return ADC0_Value[0];
}

