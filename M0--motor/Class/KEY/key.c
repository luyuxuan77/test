#include "key.h"

void key_init(void)
{
	DL_GPIO_initDigitalInputFeatures(KEY_A3_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		 DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
	DL_GPIO_initDigitalInputFeatures(KEY_A4_IOMUX,
		 DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
		DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

	DL_GPIO_setLowerPinsPolarity(KEY_PORT, DL_GPIO_PIN_3_EDGE_FALL|DL_GPIO_PIN_4_EDGE_FALL);
	DL_GPIO_clearInterruptStatus(KEY_PORT, KEY_A3_PIN|KEY_A4_PIN);
	DL_GPIO_enableInterrupt(KEY_PORT, KEY_A3_PIN|KEY_A4_PIN);
	NVIC_EnableIRQ(GPIO_key1_IRQn);							// �����ж�
}
int Key_Value;
void GROUP1_IRQHandler(void)
{
	/*�ռ����Բ��������жϵ�����*/
	uint32_t IRQn_key = GPIO_getEnabledInterruptStatus(KEY_GPIO,KEY_A3_PIN|KEY_A4_PIN);

	/*���ݲ�ͬ����ִ�ж�Ӧ�жϴ���*/
	if ((IRQn_key & KEY_A3_PIN) ==KEY_A3_PIN)
	{
		Key_Value = 1;
	}
	else if ((IRQn_key & KEY_A4_PIN) ==KEY_A4_PIN)
	{
		Key_Value = 2;
	}
	else Key_Value = 0;
}

