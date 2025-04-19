#include "uart3.h"

static const DL_UART_Main_ClockConfig gUART_3ClockConfig = {
    .clockSel = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1};

static const DL_UART_Main_Config gUART_3Config = {
    .mode = DL_UART_MAIN_MODE_NORMAL,
    .direction = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity = DL_UART_MAIN_PARITY_NONE,
    .wordLength = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits = DL_UART_MAIN_STOP_BITS_ONE};
//#pragma(__use_no_semihosting)
//struct FILE
//{
//    int handle;
//};
//FILE __stdout;
//void _sys_exit(int x)
//{
//    x = x;
//}
//int fputc(int ch, FILE *f)
//{
//    /* ����һ������ */
//    DL_UART_Main_transmitData(UART0, (uint8_t)ch);
//    /* �ȴ����ݴ������ */
//    while (DL_UART_Main_isBusy(UART0))
//        ;

//    return ch;
//}
//int fputs(const char *_ptr, register FILE *_fp)
//{
//    uint16_t i, len;
//    len = strlen(_ptr);
//    for (i = 0; i < len; i++)
//    {
//        /* ����һ������ */
//        DL_UART_Main_transmitData(UART0, (uint8_t)_ptr[i]);
//        /* �ȴ����ݴ������ */
//        while (DL_UART_Main_isBusy(UART0))
//            ;
//    }

//    return len;
//}
/**************************************************************************
�������ܣ�UART3��ʼ������
����˵����
        ���ò����ʵĲ������㹫ʽ
        BRD = UART Clock / ��OVS * Baudrate�� = integerDivisor.X
        fractionalDivisor = ��X*64��+0.5
��ڲ�����ѡ�����ʣ�Ŀǰֻ��115200��9600
����  ֵ����
��    �����ڱ���Ƭ��ʱ�������У�����0 1 2��PD1��40M,���Ǵ���3��PD1��80M
**************************************************************************/
void uart3_init(int baud)
{
    /* ��λ�������� */
    DL_UART_reset(UART_3_INST);
     /* UART3�ϵ�ʹ�ܣ�UART0�� */
    DL_UART_enablePower(UART_3_INST);
     /* ����UART3���贫����������TX */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_3_IOMUX_TX, GPIO_UART_3_IOMUX_TX_FUNC);
    /* ����UART3����������������RX */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_3_IOMUX_RX, GPIO_UART_3_IOMUX_RX_FUNC);
    /* ����UART3ʱ������ */
    DL_UART_setClockConfig(UART_3_INST, (DL_UART_Main_ClockConfig *)&gUART_3ClockConfig);
    /* ��ʼ��UART3 */
    DL_UART_init(UART_3_INST, (DL_UART_Main_Config *)&gUART_3Config);
     /* ���ù�������16�������� */
    DL_UART_setOversampling(UART_3_INST, DL_UART_OVERSAMPLING_RATE_16X);
     /* ����UART3������ */
    if (baud == 115200)
        DL_UART_Main_setBaudRateDivisor(UART3, (43), (26));
    if (baud == 9600)
        DL_UART_Main_setBaudRateDivisor(UART3, (520), (53));

    /* ���ô��ڽ����ж� */
    DL_UART_enableInterrupt(UART_3_INST,
                                 DL_UART_MAIN_INTERRUPT_RX);
    /* ����UART3��FIFO(�Ƚ��ȳ�����) */
    DL_UART_enableFIFOs(UART_3_INST);
    /* ����FIFO�Ľ�����ֵ */
    DL_UART_setRXFIFOThreshold(UART_3_INST, DL_UART_RX_FIFO_LEVEL_1_2_FULL);
    /* ����FIFO�ķ�����ֵ */
    DL_UART_setTXFIFOThreshold(UART_3_INST, DL_UART_TX_FIFO_LEVEL_1_2_EMPTY);
    /* UART3����ʹ�� */
    DL_UART_Main_enable(UART_3_INST);
    /* ���UART3���ж� */
    NVIC_ClearPendingIRQ(UART3_INT_IRQn);
    /*UART3�ж�ʹ�� */
    NVIC_EnableIRQ(UART3_INT_IRQn);
}

/**************************************************************************
�������ܣ�UART1�жϷ�����
����˵������
��ڲ�������
����  ֵ����
**************************************************************************/
uint16_t g_usart3_rx_buf[20];           // ?????
uint8_t g_usart3_rx_buffff[20];        // ???????,???????
uint16_t g_usart3_rx_sta = 0;          // ????

void UART_3_INST_IRQHandler(void)
{
    if(DL_UART_Main_getPendingInterrupt(UART3)==DL_UART_MAIN_IIDX_RX) 
    {
		uint16_t rxdata=0;
		rxdata = DL_UART_receiveData(UART3);
		if ((g_usart3_rx_sta & 0x8000) == 0) 
		{
			if (g_usart3_rx_sta & 0x4000) 
			{
				if (rxdata != 0x0a) 
				{
					g_usart3_rx_sta = 0; 
					g_usart3_rx_buf[0] = 0;
				}
				else
				{
					g_usart3_rx_sta |= 0x8000; 
					if (g_usart3_rx_buf[0] == 0xff)
					{
						
						
//						g_usart3_rx_buffff[1] = g_usart3_rx_buf[1];
//						g_usart3_rx_buffff[2] = g_usart3_rx_buf[2];
//						g_usart3_rx_buffff[3] = g_usart3_rx_buf[3];
//						g_usart3_rx_buffff[4] = g_usart3_rx_buf[4];
//						g_usart3_rx_buffff[5] = g_usart3_rx_buf[5];
////						printf("2");
					}
					g_usart3_rx_sta = 0;
				}
			}
			else 
			{
				if (rxdata == 0x0d)
				{
					g_usart3_rx_sta |= 0x4000; 
				}
				else
				{
					g_usart3_rx_buf[g_usart3_rx_sta & 0X3FFF] = rxdata; 
					g_usart3_rx_sta++;
					
					if (g_usart3_rx_sta > (20 - 1))
					{
						g_usart3_rx_sta= 0; 
					}
				}
			}
		}
	}
	DL_UART_Main_clearInterruptStatus(UART3,DL_UART_MAIN_IIDX_RX);
}
