//#include "uart0.h"
//#include "State_Machine.h"
//#include <stdint.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include "tiva_sys.h"
//#include "tiva_lib.h"
//#include "udma.h"
//#include "track.h"
//#include "oled.h"
#include "bsp.h"

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


/**************************************************************************
�������ܣ�����0�жϷ�����
��ڲ�������
����˵������
**************************************************************************/
// ���ջ�����
uint8_t uart_rx_buf[4];
uint8_t uart_rx_index = 0;
volatile int response_flag = 0;
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
				response_flag = 1;
//                OLED_Clear();
//                OLED_ShowString(0, 32, "SET SUCCESS",16);
//				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0xF6 &&
                     uart_rx_buf[2] == 0x02 &&
                     uart_rx_buf[3] == 0x6B) 
            {
				response_flag = 2;
//                OLED_Clear();
//                OLED_ShowString(0, 32, "SPEED SUCCESS",16);
//				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0xFD &&
                     uart_rx_buf[2] == 0x02 &&
                     uart_rx_buf[3] == 0x6B) 
            {
				response_flag = 3;
//                OLED_Clear();
//                OLED_ShowString(0, 32, "STATION SUCCESS",16);
//				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0xFE &&
                     uart_rx_buf[2] == 0x02 &&
                     uart_rx_buf[3] == 0x6B) 
            {
				response_flag = 4;
//                OLED_Clear();
//                OLED_ShowString(0, 32, "STOP SUCCESS",16);
//				OLED_Refresh_Gram();

            }
            else if (uart_rx_buf[0] == 0x01 &&
                     uart_rx_buf[1] == 0x00 &&
                     uart_rx_buf[2] == 0xEE &&
                     uart_rx_buf[3] == 0x6B) 
            {
				response_flag = 5;
//                OLED_Clear();
//                OLED_ShowString(0, 32, "ERROR",16);
//				OLED_Refresh_Gram();

            }
            else
            {
//                OLED_Clear();
//                OLED_ShowString(0, 32, "UNKNOWN CMD",16);
//				OLED_Refresh_Gram();

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


/* �������鶨�� */
const uint8_t cmd_send[6] = {0x01, 0xF3, 0xAB, 0x01, 0x00, 0x6B};//const��ʾֻ��
 uint8_t speed_send[8] = {0x01, 0xF6, 0x01, 0x00, 0x05, 0x10, 0x00, 0x6B};
								     /*����****�ٶ�***,���ٶ�*/
 uint8_t station_send[13] = {0x01, 0xFD, 0x01, 0x00, 0x64, 0x0A, 0x00, 0x00, 0x0C, 0x80, 0x00, 0x00, 0x6B};
											/*����****�ٶ�***,���ٶ�,*****������***********/
const uint8_t stop_send[5] = {0x01, 0xFE, 0x98, 0x00, 0x6B};

/**************************************************************************
�������ܣ���������ָ��
��ڲ�������
���ز�������
**************************************************************************/
void send_cmd_set(void)
{
    UART0Send(cmd_send, sizeof(cmd_send));
}

/**************************************************************************
�������ܣ������ٶ�ָ��
��ڲ�����speed - �ٶ�ֵ����λ����Э��Ҫ��
���ز�������
˵�����Զ������ٶ��ֶΣ���˸�ʽ��
**************************************************************************/
void send_speed(uint16_t speed)
{
    /* �����ٶ��ֶΣ�����4-5�� */
    speed_send[4] = (uint8_t)(speed >> 8);   // ���ֽ�
    speed_send[5] = (uint8_t)(speed & 0xFF); // ���ֽ�
    
    UART0Send(speed_send, sizeof(speed_send));
}

/**************************************************************************
�������ܣ�����վ������ָ��
��ڲ�����pulse_count - 32λ�������ֵ
���ز�������
˵�����Զ�������������ֶΣ���˸�ʽ��
**************************************************************************/
void send_station(uint32_t pulse_count)
{
    /* ������������ֶΣ�����6-9�� */
    station_send[6] = (uint8_t)(pulse_count >> 24);  // ����ֽ�
    station_send[7] = (uint8_t)(pulse_count >> 16);
    station_send[8] = (uint8_t)(pulse_count >> 8);
    station_send[9] = (uint8_t)(pulse_count & 0xFF); // ����ֽ�
    
    UART0Send(station_send, sizeof(station_send));
}

/**************************************************************************
�������ܣ����ͼ�ָͣ��
��ڲ�������
���ز�������
**************************************************************************/
void send_emergency_stop(void)
{
    UART0Send(stop_send, sizeof(stop_send));
}
