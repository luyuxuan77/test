#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart2.h"
/**************************************************************************
�������ܣ�����2��ʼ��
��ڲ�����uint32_t��uint32_t
����˵��������ֵΪ���ڲ����ʡ�UART��Դʱ��Ƶ��
**************************************************************************/
void uart2_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);  // ʹ�� GPIOD ����
	//����PD7���ţ�Ҫ����UART2����ʹ��ǰ�������޷�ʹ��
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  |= GPIO_PIN_7;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x0;
	
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);  // ʹ�� UART2 ����
    GPIOPinConfigure(GPIO_PD6_U2RX);  // ����GPIO���ŵı��ù���
    GPIOPinConfigure(GPIO_PD7_U2TX);  // ����GPIO���ŵı��ù���
	UARTFIFOEnable(UART2_BASE);  // ʹ��FIFO
	MAP_IntMasterEnable();  // �����������ж�
	MAP_IntEnable(INT_UART2);  // ��������2�ж�
	MAP_UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);  // ��������2���ж�Դ
    MAP_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);  // ����UART��Χ�豸ʹ�õ����� PD6|PD7
	UARTIntRegister(UART2_BASE, USART2_IRQHandler);  // ע�ᴮ���жϺ���
	IntPrioritySet(INT_UART2, 0);  // �����ж����ȼ�
    UARTClockSourceSet(UART2_BASE, UART_CLOCK_PIOSC);  // Ϊָ����UART���ò���ʱ��Դ����ʱΪ16MHz
    UARTStdioConfig(2, ui32Baud, ui32SrcClock);  // �����趨�������ֱ��Ǵ��ڱ�š������ʡ�UART��Դʱ��Ƶ��
}

/**************************************************************************
�������ܣ�����2�жϺ���
��ڲ�����void
����˵������Ϊ����2���жϺ������ṹ��F407��ʱ���ж�����
**************************************************************************/
void USART2_IRQHandler(void)
{
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART2_BASE, true);  // ��ȡ����2���ж�״̬
    MAP_UARTIntClear(UART2_BASE, ui32Status);  // �������2���ж�״̬
    while(MAP_UARTCharsAvail(UART2_BASE))  // �жϻ������Ƿ����ַ�
    {
	} 
        MAP_UARTCharPutNonBlocking(UART2_BASE, MAP_UARTCharGetNonBlocking(UART2_BASE));  // ���ղ����ش�ָ������FIFO���͵�һ���ַ�
}

/**************************************************************************
�������ܣ�����2����ַ�������
��ڲ�����const uint8_t��uint32_t
����˵������Ϊ����2����ַ����ĺ�����ע���ַ����ĳ���
**************************************************************************/
void UART2Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART2_BASE, *pui8Buffer++);
    }
}


