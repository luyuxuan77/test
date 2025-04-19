#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart7.h"
#include "uart1.h"
unsigned char USART7_RX_BUF[13] = {0};
unsigned char i;
unsigned char Distance;//cm
/**************************************************************************
�������ܣ�����7��ʼ��
��ڲ�����uint32_t��uint32_t
����˵��������ֵΪ���ڲ����ʡ�UART��Դʱ��Ƶ��
**************************************************************************/
void uart7_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // ʹ�� GPIOE ����
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);  // ʹ�� UART7 ����
    GPIOPinConfigure(GPIO_PE0_U7RX);  // ����GPIO���ŵı��ù���
    GPIOPinConfigure(GPIO_PE1_U7TX);  // ����GPIO���ŵı��ù���
	UARTFIFOEnable(UART7_BASE);  // ʹ��FIFO
	MAP_IntMasterEnable();  // �����������ж�
	MAP_IntEnable(INT_UART7);  // ��������7�ж�
	MAP_UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_RT);  // ��������7���ж�Դ
    MAP_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);  // ����UART��Χ�豸ʹ�õ����� PE4|PE5
	UARTIntRegister(UART7_BASE, USART7_IRQHandler);  // ע�ᴮ���жϺ���
	IntPrioritySet(INT_UART7, 0);  // �����ж����ȼ�
    UARTClockSourceSet(UART7_BASE, UART_CLOCK_PIOSC);  // Ϊָ����UART���ò���ʱ��Դ����ʱΪ16MHz
	UARTConfigSetExpClk(UART7_BASE, ui32SrcClock, ui32Baud, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);  // ���ô��ڲ���
//	UARTStdioConfig(7, ui32Baud, ui32SrcClock);  // �����趨�������ֱ��Ǵ��ڱ�š������ʡ�UART��Դʱ��Ƶ��
}

int data_test(unsigned char data[])
{
	if(data[0]!=0xa3) return false;  //Ö¡Í·
	if(data[1]!=0xb3) return false;  //Ö¡Í·
	if(data[9]!=0xc3) return false;  //Ö¡Î²
	
	return true;
}

/**************************************************************************
�������ܣ�����7�жϺ���
��ڲ�����void
����˵������Ϊ����7���жϺ������ṹ��F407��ʱ���ж�����
**************************************************************************/
void USART7_IRQHandler(void)
{
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART7_BASE, true);  // ��ȡ����7���ж�״̬
 	   MAP_UARTIntClear(UART7_BASE, ui32Status);  // �������7���ж�״̬
    while(MAP_UARTCharsAvail(UART7_BASE))  // �жϻ������Ƿ����ַ�
    {
		unsigned char res; 
		res=UARTCharGetNonBlocking(UART7_BASE);
		USART7_RX_BUF[i++]=res;
		if(USART7_RX_BUF[0]!=0xa3) i=0;
		if((i==2)&&(USART7_RX_BUF[1]!=0xb3)) i=0; 
		if(i==10)i = 0;
		if(data_test(USART7_RX_BUF))
		{ 
			Distance = USART7_RX_BUF[2];
		}
//		if(i == 1 && USART7_RX_BUF[0] !=0xAA)//�ж��Ƿ���յ����ݰ�ͷ9
//		{
//			i = 0;
//			return;
//		}
//		if(i == 4 && USART7_RX_BUF[3] != 0x22)
//		{
//			i =0;
//			return;
//		}
//		if(i > 12)
//		{
//			for(int z = 0;z < 12;z++)
//			{
//				sum += USART7_RX_BUF[z];
//			}
//			if(sum == USART7_RX_BUF[12])
//			{
//				laser[0]=USART7_RX_BUF[6];
//				laser[1]=USART7_RX_BUF[7];
//				laser[2]=USART7_RX_BUF[8];
//				laser[3]=USART7_RX_BUF[9];
//				i = 0;
//				for (int j = 0; j < 4; j++) 
//				{
//					result = result << 8 |laser [j];//ʮ�����Ƶ�1λ�൱�ڶ����Ƶ�4λ�����Զ���������8λ�൱��ʮ����������2λ
//				}
//			}
//			else
//			{
//				i = 0;
//			}
//		}
			//printf("%d\n",result);

        //MAP_UARTCharPutNonBlocking(UART7_BASE, MAP_UARTCharGetNonBlocking(UART7_BASE));  // ���ղ����ش�ָ������FIFO���͵�һ���ַ�
	}
}
/**************************************************************************
�������ܣ�����7����ַ�������
��ڲ�����const uint8_t��uint32_t
����˵������Ϊ����7����ַ����ĺ�����ע���ַ����ĳ���
**************************************************************************/
void UART7Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART7_BASE, *pui8Buffer++);
    }
}


