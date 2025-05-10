#include "uart0.h"
#include "State_Machine.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "udma.h"
#include "track.h"
#include "oled.h"


/**************************************************************************
�������ܣ�����0�жϷ�����
��ڲ�������
����˵������
**************************************************************************/
// ���ջ�����
uint8_t uart_rx_buf[4];
uint8_t uart_rx_index = 0;

void UART0_IRQHandler(void)
{
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);  // ��ȡ�ж�״̬
    MAP_UARTIntClear(UART0_BASE, ui32Status);          // ����жϱ�־

    while (MAP_UARTCharsAvail(UART0_BASE))  // �����ݿɶ�
    {
        uint8_t byte = UARTCharGetNonBlocking(UART0_BASE);
        uart_rx_buf[uart_rx_index++] = byte;

        if (uart_rx_index >= 4)  // ���յ�4�ֽ�
        {
            uart_rx_index = 0;  // ���ü�����

            // ƥ�䲻ͬ����
            if (uart_rx_buf[0] == 0x01 &&
                uart_rx_buf[1] == 0xF3 &&
                uart_rx_buf[2] == 0x02 &&
                uart_rx_buf[3] == 0x6B) 
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "SET SUCCESS",16);
				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0xF6 &&
                     uart_rx_buf[2] == 0x02 &&
                     uart_rx_buf[3] == 0x6B) 
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "SPEED SUCCESS",16);
				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0xFD &&
                     uart_rx_buf[2] == 0x02 &&
                     uart_rx_buf[3] == 0x6B) 
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "STATION SUCCESS",16);
				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0xFE &&
                     uart_rx_buf[2] == 0x02 &&
                     uart_rx_buf[3] == 0x6B) 
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "STOP SUCCESS",16);
				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0x00 &&
                     uart_rx_buf[2] == 0xEE &&
                     uart_rx_buf[3] == 0x6B) 
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "ERROR",16);
				OLED_Refresh_Gram();

            }
            else
            {
                OLED_Clear();
                OLED_ShowString(0, 0, "UNKNOWN CMD",16);
				OLED_Refresh_Gram();

            }
        }
    }
}


/**************************************************************************
�������ܣ�����0��ʼ��
��ڲ�����ui32Baud�����ڲ����ʡ�ui32SrcClock������Դʱ��Ƶ��
���ز�������
**************************************************************************/
void uart0_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);	//ʹ��UART0����
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//ʹ��GPIOA����
	MAP_GPIOPinConfigure(GPIO_PA0_U0RX);	//����GPIO���ű��ù���
	MAP_GPIOPinConfigure(GPIO_PA1_U0TX);	//����GPIO���ű��ù���
	
	MAP_UARTFIFOEnable(UART0_BASE);	//ʹ��FIFO
	MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	//����UART��Χ�豸ʹ�õ�����PA0|PA1
	MAP_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);	//Ϊָ����UART���ò�����ʱ��Դ����ʱΪ16MHz
	UARTStdioConfig(0 , ui32Baud, ui32SrcClock);			//�����趨�������ֱ��Ǵ��ڱ�š������ʡ�UART��ʱ��Ƶ��
	
	UARTIntRegister(UART0_BASE, UART0_IRQHandler);	//ע�ᴮ���жϺ���
	MAP_IntPrioritySet(INT_UART0, 3);				//�����ж����ȼ�
	
	MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);	//��������0�ж�Դ
	MAP_IntEnable(INT_UART0);	//��������0�ж�
	MAP_IntMasterEnable();		//�����������ж�
}

/**************************************************************************
�������ܣ�����0����ַ�������
��ڲ�����pui8Buffer���ַ�����ui32Count���ַ�������
���ز�������
**************************************************************************/
void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}


#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{
	UARTCharPut(UART0_BASE,(unsigned char)ch);//������������ڣ�?����޸Ļ��?UART0_BASE)���ɡ�
	return ch;
}
#endif
