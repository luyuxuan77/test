#include "keyboard.h"
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart1.h"
#include "my_timer.h"
#include "control.h"
//#include "motor.h"
//#include "uart0.h"
//#include "ccd.h"
//#include "rgb.h"
//#include "key.h"
//#include "encoder.h"
//#include "track.h"
//#include "State_Machine.h"
//#include "uart7.h"
//#include "distance.h"
//#include "oled.h"
//#include <stdio.h>
#include "bsp.h"

#define KEYBOARD_PORT GPIO_PORTD_BASE
#define ROW0_PIN GPIO_PIN_0
#define ROW1_PIN GPIO_PIN_1
#define COL0_PIN GPIO_PIN_2
#define COL1_PIN GPIO_PIN_3


// ��ʼ��2x2������̵�GPIO����
void keyboard_init(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while (!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));

    // ����������Ϊ���
    MAP_GPIOPinTypeGPIOOutput(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN);

    // ����������Ϊ���룬����
    MAP_GPIOPinTypeGPIOInput(KEYBOARD_PORT, COL0_PIN | COL1_PIN);
    MAP_GPIOPadConfigSet(KEYBOARD_PORT, COL0_PIN | COL1_PIN,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // ��ʼ��������������Ϊ�ߵ�ƽ�����ã�
    MAP_GPIOPinWrite(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN, ROW0_PIN | ROW1_PIN);
}

// ����ɨ�躯�������� 1~4��δ���·���0
uint8_t keyboard_scan(void)
{
    uint8_t key = 0;
    static uint8_t key_flag = 0;

//    uint8_t rows[2] = {ROW0_PIN, ROW1_PIN};
    uint8_t cols[2] = {COL0_PIN, COL1_PIN};

    if (key_flag)
    {
        delay_ms(200); // ��ֹ�ظ�����
        key_flag = 0;
    }

    for (int row = 0; row < 2; row++)
    {
        // ���͵�ǰ�У�����Ϊ�ߵ�ƽ
        uint8_t out = (row == 0) ? ROW1_PIN : ROW0_PIN;
        MAP_GPIOPinWrite(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN, out);
        delay_ms(5); // �����ӳ�

        for (int col = 0; col < 2; col++)
        {
            if (!MAP_GPIOPinRead(KEYBOARD_PORT, cols[col])) // ���͵�ƽ
            {
                key = row * 2 + col + 1;
                key_flag = 1;
                break;
            }
        }

        if (key_flag) break;
    }

    // �����лع�ߵ�ƽ
    MAP_GPIOPinWrite(KEYBOARD_PORT, ROW0_PIN | ROW1_PIN, ROW0_PIN | ROW1_PIN);

    return key;
}
