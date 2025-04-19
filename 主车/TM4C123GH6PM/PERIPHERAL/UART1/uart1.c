#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "udma.h"
#include "uart0.h"
#include "uart1.h"
#include "State_Machine.h"
#include "track.h"

// uDMA���������Ʊ�
#if defined(ewarm)
#pragma data_alignment=1024
uint8_t ui8ControlTable[1024];
#elif defined(ccs)
#pragma DATA_ALIGN(ui8ControlTable, 1024)
uint8_t ui8ControlTable[1024];
#else
uint8_t ui8ControlTable[1024] __attribute__ ((aligned(1024)));
#endif

uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];
uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];
uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];
uint8_t g_ui8RxBufA[UART_RXBUF_SIZE];
uint8_t g_ui8RxBufB[UART_RXBUF_SIZE];
uint32_t g_ui32RxBufACount = 0;
uint32_t g_ui32RxBufBCount = 0;

/**************************************************************************
�������ܣ�����1+uDMA�жϴ�����
��ڲ�����void
����˵����UART1��uDMAͨ�����DMA����ʱ�����ж�
**************************************************************************/
void UART1IntHandler(void)
{
    uint32_t ui32Status;
    uint32_t ui32Mode;
    ui32Status = MAP_UARTIntStatus(UART1_BASE, 1);  // ��ȡ�����ж�״̬
    MAP_UARTIntClear(UART1_BASE, ui32Status);  // ������й���״̬
    ui32Mode = MAP_uDMAChannelModeGet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT);  // ��鴫���Ƿ����
    if(ui32Mode == UDMA_MODE_STOP)
    {
        g_ui32RxBufACount++;
		// ����uDMAͨ�����ƽṹ�Ĵ������
        MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufA, sizeof(g_ui8RxBufA));
    }
    ui32Mode = MAP_uDMAChannelModeGet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT);  // ��鴫���Ƿ����
    if(ui32Mode == UDMA_MODE_STOP)
    {
        g_ui32RxBufBCount++;
		// ����uDMAͨ�����ƽṹ�Ĵ������
        MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufB, sizeof(g_ui8RxBufB));
    }
    if(!MAP_uDMAChannelIsEnabled(UDMA_CHANNEL_UART1TX))  // ��UART1DMAͨ���޷�ʹ�ã���DMA�������
    {
		// ����uDMAͨ�����ƽṹ�Ĵ������
        MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC, g_ui8TxBuf, (void *)(UART1_BASE + UART_O_DR), sizeof(g_ui8TxBuf));
        MAP_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);  // ���� uDMA UART1TX ͨ��
    }
}

/**************************************************************************
�������ܣ�����1+uDMA��ʼ������
��ڲ�����void
����˵����
**************************************************************************/
//void InitUART1Transfer(uint32_t ui32Baud, uint32_t ui32SrcClock)
//{
//	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);  // ʹ�� uDMA ����
//	MAP_IntEnable(INT_UDMAERR);  // ʹ��uDMA�����ж�
//	MAP_uDMAEnable();  // ʹ��uDMA������
//	MAP_uDMAControlBaseSet(ui8ControlTable);  // ����ͨ�����Ʊ�Ļ���ַ
//	InitSWTransfer();
//	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  // ʹ�� GPIOB ����
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);  // ʹ�� UART1 ����
//	GPIOPinConfigure(GPIO_PB0_U1RX);  // ����GPIO���ŵı��ù���
//	GPIOPinConfigure(GPIO_PB1_U1TX);  // ����GPIO���ŵı��ù���
//	MAP_IntEnable(INT_UART1);  // ��������1�ж�
//	MAP_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);  // ����UART��Χ�豸ʹ�õ����� PB0|PB1
//    MAP_UARTConfigSetExpClk(UART1_BASE, ui32SrcClock, ui32Baud, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);  // ���ô��ڲ���
//    MAP_UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);  // ���ô�����ֵ
//    MAP_UARTEnable(UART1_BASE);  // ʹ�� UART1 �ķ��ͺͽ���
//    MAP_UARTDMAEnable(UART1_BASE, UART_DMA_RX | UART_DMA_TX);  // ʹ�� UART1DMA ��TX��RXͨ��
//    MAP_IntEnable(INT_UART1);  // ��������1�ж�
//    MAP_uDMAChannelAttributeDisable(UDMA_CHANNEL_UART1RX, UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);  // ��ʼ��uDMA����
//    // ����uDMAͨ�����ƽṹ�Ŀ��Ʋ���
//	MAP_uDMAChannelControlSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4);
//    MAP_uDMAChannelControlSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4);
//    // ����uDMAͨ�����ƽṹ�Ĵ������
//	MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufA, sizeof(g_ui8RxBufA));
//    MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufB, sizeof(g_ui8RxBufB));
//    MAP_uDMAChannelAttributeDisable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_ALTSELECT | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);  // ��ʼ��uDMA����
//    MAP_uDMAChannelAttributeEnable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_USEBURST);  // ����uDMA ADC0 ͨ��������
//	// ����uDMAͨ�����ƽṹ�Ŀ��Ʋ���
//    MAP_uDMAChannelControlSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_4);
//	// ����uDMAͨ�����ƽṹ�Ĵ������
//    MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC, g_ui8TxBuf, (void *)(UART1_BASE + UART_O_DR), sizeof(g_ui8TxBuf));
//    MAP_uDMAChannelEnable(UDMA_CHANNEL_UART1RX);  // ʹ�� uDMA ͨ��
//    MAP_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);
//}

/**************************************************************************
�������ܣ�����1��ʼ��
��ڲ�����uint32_t��uint32_t
����˵��������ֵΪ���ڲ����ʡ�UART��Դʱ��Ƶ��
**************************************************************************/
void uart1_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  // ʹ�� GPIOB ����
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);  // ʹ�� UART1 ����
	GPIOPinConfigure(GPIO_PB0_U1RX);  // ����GPIO���ŵı��ù���
	GPIOPinConfigure(GPIO_PB1_U1TX);  // ����GPIO���ŵı��ù���
	UARTFIFOEnable(UART1_BASE);  // ʹ��FIFO
	MAP_IntMasterEnable();  // �����������ж�
	MAP_IntEnable(INT_UART1);  // ��������1�ж�
	MAP_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);  // ��������1���ж�Դ
	MAP_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);  // ����UART��Χ�豸ʹ�õ����� PB0|PB1
	UARTIntRegister(UART1_BASE, USART1_IRQHandler);  // ע�ᴮ���жϺ���
	IntPrioritySet(INT_UART1, 1);  // �����ж����ȼ�
	UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);  // Ϊָ����UART���ò���ʱ��Դ����ʱΪ16MHz
	UARTStdioConfig(1, ui32Baud, ui32SrcClock);  // �����趨�������ֱ��Ǵ��ڱ�š������ʡ�UART��Դʱ��Ƶ��
}


void processSliderData(u8 frameIdentifier, u8 sliderValue) {
    switch (frameIdentifier) {
        case 0xF1:
            printf("������1������: %d\n", sliderValue);
			UWB_Current_Angle = sliderValue;
            // �ڴ˴���Ӵ����߼�...
            break;
        case 0xF2:
            printf("������2������: %d\n", sliderValue);
			UWB_Current_Distance = sliderValue;
            // �ڴ˴���Ӵ����߼�...
            break;
        case 0xF3:
            printf("������3������: %d\n", sliderValue);
            // �ڴ˴���Ӵ����߼�...
            break;
        default:
            printf("δ֪�Ļ�����ʶ��: %d\n", frameIdentifier);
    }
}

#define USART_REC_LEN 100 // ���������ճ���Ϊ100�ֽ�
u8 USART_RX_BUF[USART_REC_LEN];
u16 USART_RX_STA = 0;
/**************************************************************************
�������ܣ�����1�жϺ���
��ڲ�����void
����˵������Ϊ����1���жϺ������ṹ��F407��ʱ���ж�����
**************************************************************************/
void USART1_IRQHandler(void)
{
	u8 res;
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART1_BASE, true);  // ��ȡ����1���ж�״̬
//	Blue_Light_Control(1);
    while(MAP_UARTCharsAvail(UART1_BASE))  // �жϻ������Ƿ����ַ�
    {
        res = UARTCharGetNonBlocking(UART1_BASE);
//		printf("%X ",res);
		if ((USART_RX_STA & 0x8000) == 0) 
		{ // ����δ���
			if (res == 0xA5 && USART_RX_STA == 0) 
			{ // ���֡��ʼ�ֽ�
				USART_RX_BUF[0] = res;
				USART_RX_STA = 1; // ��ʼ������֡
			}
			else if (USART_RX_STA >= 1 && USART_RX_STA < 7) 
			{ // ���ս��������ֽ�
				USART_RX_BUF[USART_RX_STA++] = res;
				if (USART_RX_STA == 7) 
				{
					// ����֡�������
					if (USART_RX_BUF[1] == 0x5A && USART_RX_BUF[6] == 0xAA) 
					{ // У��֡�����ֽں͵ڶ�����ʼ�ֽ�
						// ����У�������֤����������
						u8 checksum = (USART_RX_BUF[2] + USART_RX_BUF[3] + USART_RX_BUF[4]) % 256;
						if (checksum == USART_RX_BUF[5])
						{
							// ������Ч�������3��4��5λ��Ŀ��ֵ
							processSliderData(USART_RX_BUF[3], USART_RX_BUF[4]);
						}
					}
					USART_RX_STA = 0; // ���ý���״̬���
				}
			}
			else 
			{
				USART_RX_STA = 0; // ���մ������¿�ʼ
			}
		}
	}
	MAP_UARTIntClear(UART1_BASE, ui32Status);  // �������1���ж�״̬
}

/**************************************************************************
�������ܣ�����1����ַ�������
��ڲ�����const uint8_t��uint32_t
����˵������Ϊ����1����ַ����ĺ�����ע���ַ����ĳ���
**************************************************************************/
void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART1_BASE, *pui8Buffer++);
    }
}
