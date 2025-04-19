#include "uart1.h"

/**************************************************************************
�������ܣ�UART1��ʼ������
����˵����
        ���ò����ʵĲ������㹫ʽ
        BRD = UART Clock / ��OVS * Baudrate�� = integerDivisor.X
        fractionalDivisor = ��X*64��+0.5
��ڲ�����ѡ�����ʣ�Ŀǰֻ��115200��9600
����  ֵ����
**************************************************************************/
static const DL_UART_Main_ClockConfig UART1_ClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Config UART1_Config = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,			// ����ģʽ����
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,		// ���� UART �������ͽ�����
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,      // ������������  ������ ����������/�����������/���������ͺ��������
    .parity      = DL_UART_MAIN_PARITY_NONE,			// ������żУ�������
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,		// 8����λ
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE			// 1ֹͣλ
};
void uart1_init(int baud)
{
	DL_UART_reset(UART1);									// ��λ����0
	DL_UART_enablePower(UART1);							// ��������0�����Դ
	DL_UART_enable(UART1);									// ʹ�ܴ���0
	DL_GPIO_initPeripheralInputFunction(UART1_RX_PINCM,UART1_RX_FUNC);		// ʹ��rx�ܽ����븴�ù���
	DL_GPIO_initPeripheralOutputFunction(UART1_TX_PINCM,UART1_TX_FUNC);
	DL_UART_setClockConfig(UART1,\
		(DL_UART_ClockConfig *) &UART1_ClockConfig);	// ���ô���0ʱ��
	DL_UART_init(UART1,(DL_UART_Config *) &UART1_Config);
	
	/* ͨ�����ó������Ͳ����ʳ������ò����� */
	/* 4000000/16/9600=208.333 ��������Ϊ208��С������Ϊ33.3��33.3%64=21 */
	/* 40000000Ϊʱ��Ƶ�ʣ�16Ϊ�����ʣ�9600Ϊ�����ʣ�64Ϊ�̶�������208��21ΪҪ��������� */
	/* ����֪�����㷽��������Ҫʹ�����������ʵĻ�����Ȼ�����ù����� */
	DL_UART_setOversampling(UART1,DL_UART_OVERSAMPLING_RATE_16X);
	if(baud==9600)
	{
		DL_UART_setBaudRateDivisor(UART1,260,27);
	}
	else if(baud==115200)
	{
		DL_UART_setBaudRateDivisor(UART1,21,45);
	}
		
	DL_UART_enableInterrupt(UART1,DL_UART_INTERRUPT_RX);	// ������ʱ�ж�
	DL_UART_enable(UART1);
    NVIC_EnableIRQ(UART1_INT_IRQn);						// �����ж�
}

/**************************************************************************
�������ܣ�UART1�жϷ�����
����˵������
��ڲ�������
����  ֵ����
**************************************************************************/
uint8_t g_usart1_rx_buf[20];
uint16_t g_usart1_rx_sta = 0;

void UART_1_INST_IRQHandler(void)
{
	if(DL_UART_Main_getPendingInterrupt(UART1)==DL_UART_MAIN_IIDX_RX) 
	{
		uint8_t rxdata;
		rxdata = DL_UART_receiveData(UART1);
		if ((g_usart1_rx_sta & 0x8000) == 0) /* ����δ��� */
		{
			if (g_usart1_rx_sta & 0x4000) /* ���յ���0x6B */
			{
					if (g_usart1_rx_buf[0] == 0x01&&g_usart1_rx_buf[1]==0x36)
					{
						g_usart1_rx_sta |= 0x8000; /* �յ���0x01,0x36,��ǽ�������� */
						g_usart1_rx_sta = 0;
					}
					else
					{
						g_usart1_rx_sta = 0; /* ���մ���, ���¿�ʼ */
						g_usart1_rx_buf[0]=0;
					}
			}
			else /* ��û�յ�0x06B*/
			{
				if (rxdata == 0x6B)
				{
					g_usart1_rx_sta |= 0x4000; /* ��ǽ��յ��� 0x6B */
				}
				else
				{
					g_usart1_rx_buf[g_usart1_rx_sta & 0X3FFF] = rxdata; /* �洢���ݵ� g_usart_rx_buf */
					g_usart1_rx_sta++;
					if (g_usart1_rx_sta > (12 - 1))
					{
						g_usart1_rx_sta = 0; /* �����������, ���¿�ʼ���� */
						g_usart1_rx_buf[0]=0;
					}
				}
			}
		}
	}
	DL_UART_Main_clearInterruptStatus(UART1,DL_UART_MAIN_IIDX_RX);
}


/* �������UART1ָ��ͺ��� */
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
	__IO uint8_t i = 0;
	
	for(i=0; i < len; i++) { DL_UART_transmitData(UART1,cmd[i]); delay_us(100);}
}
