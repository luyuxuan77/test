
#include "bsp.h" // ȷ�����ͷ�ļ�������GPIO��صĺ�������

void keyboard_init(void)
{
    // ��ʼ��������Ϊ���
    DL_GPIO_initDigitalOutput(MAT_KEY_ROW0_IOMUX);
    DL_GPIO_initDigitalOutput(MAT_KEY_ROW1_IOMUX);
    DL_GPIO_initDigitalOutput(MAT_KEY_ROW2_IOMUX);
    DL_GPIO_initDigitalOutput(MAT_KEY_ROW3_IOMUX);

    // ��ʼ��������Ϊ���룬������������
    DL_GPIO_initDigitalInputFeatures(MAT_KEY_COL0_IOMUX,
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
         DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(MAT_KEY_COL1_IOMUX,
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
         DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(MAT_KEY_COL2_IOMUX,
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
         DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(MAT_KEY_COL3_IOMUX,
         DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_UP,
         DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);

    // �������������
    DL_GPIO_enableOutput(GPIOA, MAT_KEY_ROW0_PIN |
        MAT_KEY_ROW1_PIN |
        MAT_KEY_ROW2_PIN |
        MAT_KEY_ROW3_PIN);

    // ����������Ϊ�ߵ�ƽ�����������У�
    DL_GPIO_setPins(GPIOA, MAT_KEY_ROW0_PIN |
        MAT_KEY_ROW1_PIN |
        MAT_KEY_ROW2_PIN |
        MAT_KEY_ROW3_PIN);
}

// ����ɨ�躯��
uint8_t Key()
{
    uint8_t key_num = 0;         // ����ֵ1-16��Ĭ��Ϊ0
    static uint8_t key_flag = 0; // ���°�����־

    // �������������ΰ���ʶ��
    if (key_flag)
    {
        delay_ms(300); // 300ms�ӳ٣���ֹ����һ�ΰ���ȴ����Ϊ�����˶�ΰ���
        key_flag = 0;  // ������°�����־
    }

    // ��ɨ�����ж�ȡ
    // ROW 0111
    DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN);
    DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW1_PIN | MAT_KEY_ROW2_PIN | MAT_KEY_ROW3_PIN);
    delay_ms(10); // ���������ӳ�

    if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN)) { key_num = 1; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN)) { key_num = 2; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN)) { key_num = 3; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN)) { key_num = 4; key_flag = 1; }

    // ROW 1011
    DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW1_PIN);
    DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN | MAT_KEY_ROW2_PIN | MAT_KEY_ROW3_PIN);
    delay_ms(10);

    if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN)) { key_num = 5; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN)) { key_num = 6; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN)) { key_num = 7; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN)) { key_num = 8; key_flag = 1; }

    // ROW 1101
    DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW2_PIN);
    DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN | MAT_KEY_ROW1_PIN | MAT_KEY_ROW3_PIN);
    delay_ms(10);

    if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN)) { key_num = 9; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN)) { key_num = 10; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN)) { key_num = 11; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN)) { key_num = 12; key_flag = 1; }

    // ROW 1110
    DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW3_PIN);
    DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN | MAT_KEY_ROW1_PIN | MAT_KEY_ROW2_PIN);
    delay_ms(10);

    if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN)) { key_num = 13; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN)) { key_num = 14; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN)) { key_num = 15; key_flag = 1; }
    else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN)) { key_num = 16; key_flag = 1; }

    return key_num;
}