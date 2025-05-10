#include "keyboard.h"
#include "bsp.h"       // 包含 GPIO 端口定义和 delay_ms
#include "ti/devices/msp/msp.h" // 或者其他头文件，确保包含 DL_GPIO_* 函数定义



void keyboard_init(void)
{
    // 初始化行引脚为输出（默认高）
    DL_GPIO_initDigitalOutputFeatures(ROW_PORT,
        DL_GPIO_OUTPUT_STRENGTH_HIGH,
        false,
        DL_GPIO_DRIVE_MODE_PUSH_PULL,
        ROW1_PIN | ROW2_PIN);
    DL_GPIO_enableOutput(ROW_PORT, ROW1_PIN | ROW2_PIN);
    DL_GPIO_setPins(ROW_PORT, ROW1_PIN | ROW2_PIN);  // 初始高电平

    // 初始化列引脚为输入，带上拉
    DL_GPIO_initDigitalInputFeatures(COL_PORT,
        DL_GPIO_INVERSION_DISABLE,
        DL_GPIO_RESISTOR_PULL_UP,
        DL_GPIO_HYSTERESIS_DISABLE,
        DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_enableInput(COL_PORT, COL1_PIN | COL2_PIN);
}

uint8_t Key(void)
{
    uint8_t key_num = 0;
    static uint8_t key_flag = 0;

    if (key_flag) {
        delay_ms(200);
        key_flag = 0;
    }

    // 扫描行1（PD1）
    DL_GPIO_clearPins(ROW_PORT, ROW1_PIN);
    DL_GPIO_setPins(ROW_PORT, ROW2_PIN);
    delay_ms(10);

    if (!DL_GPIO_readPins(COL_PORT, COL1_PIN)) { key_num = 1; key_flag = 1; }
    else if (!DL_GPIO_readPins(COL_PORT, COL2_PIN)) { key_num = 2; key_flag = 1; }

    // 扫描行2（PD2）
    DL_GPIO_clearPins(ROW_PORT, ROW2_PIN);
    DL_GPIO_setPins(ROW_PORT, ROW1_PIN);
    delay_ms(10);

    if (!DL_GPIO_readPins(COL_PORT, COL1_PIN)) { key_num = 3; key_flag = 1; }
    else if (!DL_GPIO_readPins(COL_PORT, COL2_PIN)) { key_num = 4; key_flag = 1; }

    return key_num;
}
