#include "keyboard.h"

void keyboard_init(void)
{
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM45,												// ��ʼ��IOMUX_PINCM45Ϊ��������
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,		// ��ֹ�����źŷ�ת,������������
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);		// �����������,���û��ѹ���
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM47,												//����ͬ��
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM54,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM48,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM49,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM50,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM51,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(IOMUX_PINCM52,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
}

void scan_keyR_init(void)
{
    DL_GPIO_initDigitalInputFeatures(R1__IOMUX,													// ��ʼ��R1__IOMUXΪ��������
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,		// ��ֹ�����źŷ�ת��������������
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);		// ����������ͣ����û��ѹ���
    DL_GPIO_initDigitalInputFeatures(R2__IOMUX,													//����ͬ��
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(R3__IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(R4__IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutputFeatures(C4__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initDigitalOutputFeatures(C3__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initDigitalOutputFeatures(C2__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initDigitalOutputFeatures(C1__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
}

void scan_keyC_init(void)
{
    DL_GPIO_initDigitalInputFeatures(C4__IOMUX,													// ��ʼ��C4__IOMUXΪ��������
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,		// ��ֹ�����źŷ�ת��������������
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);		//����������ͣ����û��ѹ���
    DL_GPIO_initDigitalInputFeatures(C3__IOMUX,													//����ͬ��
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(C2__IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(C1__IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    DL_GPIO_initDigitalOutputFeatures(R4__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initDigitalOutputFeatures(R3__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initDigitalOutputFeatures(R2__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_initDigitalOutputFeatures(R1__IOMUX,
                                      DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
                                      DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
}

int keyboard_scan(void)
{
    uint32_t Cpins = 0;							// ��ʼ����ȡ��������״̬����
    scan_keyR_init();							// ��ʼ����ɨ��
    delay_cycles(100000);						// ��ʱ��ȷ���ź��ȶ�
	
	// ���ÿһ���Ƿ��а�������
    if (DL_GPIO_readPins(GPIOB, R1__PIN))		// ���R1���а������£�������
    {
        scan_keyC_init();
        delay_cycles(100000);
        Cpins = DL_GPIO_readPins(GPIOB, C1__PIN | C2__PIN | C3__PIN | C4__PIN);
        if (Cpins & C1__PIN)
            return 1;
        else if (Cpins & C2__PIN)
            return 2;
        else if (Cpins & C3__PIN)
            return 3;
        else if (Cpins & C4__PIN)
            return 4;
    }
    else if (DL_GPIO_readPins(GPIOA, R2__PIN))	// ���R2���а������£�������
    {
        scan_keyC_init();
        delay_cycles(100000);
        Cpins = DL_GPIO_readPins(GPIOB, C1__PIN | C2__PIN | C3__PIN | C4__PIN);
        if (Cpins & C1__PIN)
            return 5;
        else if (Cpins & C2__PIN)
            return 6;
        else if (Cpins & C3__PIN)
            return 7;
        else if (Cpins & C4__PIN)
            return 8;
    }

    else if (DL_GPIO_readPins(GPIOA, R3__PIN))	// ���R3���а������£�������
    {
        scan_keyC_init();
        delay_cycles(100000);
        Cpins = DL_GPIO_readPins(GPIOB, C1__PIN | C2__PIN | C3__PIN | C4__PIN);
        if (Cpins & C1__PIN)
            return 9;
        else if (Cpins & C2__PIN)
            return 10;
        else if (Cpins & C3__PIN)
            return 11;
        else if (Cpins & C4__PIN)
            return 12;
    }

    else if (DL_GPIO_readPins(GPIOB, R4__PIN))	// ���R4���а������£�������
    {
        scan_keyC_init();
        delay_cycles(100000);
        Cpins = DL_GPIO_readPins(GPIOB, C1__PIN | C2__PIN | C3__PIN | C4__PIN);
        if (Cpins & C1__PIN)
            return 13;
        else if (Cpins & C2__PIN)
            return 14;
        else if (Cpins & C3__PIN)
            return 15;
        else if (Cpins & C4__PIN)
            return 16;
    }
    return 0;								// ���R2���а������£�������
}
