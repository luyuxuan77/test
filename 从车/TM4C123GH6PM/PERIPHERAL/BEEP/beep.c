#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart1.h"
#include "my_timer.h"
#include "control.h"
#include "motor.h"
#include "uart0.h"
#include "ccd.h"
#include "rgb.h"
#include "key.h"
#include "encoder.h"
#include "track.h"
#include "State_Machine.h"
#include "uart7.h"
#include "distance.h"
#include "oled.h"
#include "beep.h"

// ��ʼ��������
void Buzzer_Init(void)
{
    // ʹ�ܶ˿�A��ʱ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // ʹ��GPIOAʱ��

    // ȷ��PA7��һ�����õ�����
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
        // �ȴ��˿�Aʱ���ȶ�
    }

    // ����PA7Ϊ���ģʽ
    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT); // ����PA7Ϊ���ģʽ

    // ����PA7Ϊ��׼�������
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);  // ����Ϊ��׼���ģʽ

    // Ĭ�Ϲرշ�����
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);  // ��PA7�õͣ��رշ�����
}

// �򿪷�����
void Buzzer_On(void)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 1);  // ��PA7�øߣ��򿪷�����
}

// �رշ�����
void Buzzer_Off(void)
{
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);  // ��PA7�õͣ��رշ�����
}

void BEEP_BeepNTimes(uint8_t times)
{
    for(uint8_t i=0; i<times; i++){
        Buzzer_On();  // ��
		delay_ms(100);
        Buzzer_Off();        // ͣ
        delay_ms(50);;          // ���Լ50ms
    }
}
