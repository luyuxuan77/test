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
//    /* 发送一个数据 */
//    DL_UART_Main_transmitData(UART0, (uint8_t)ch);
//    /* 等待数据传输完毕 */
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
//        /* 发送一个数据 */
//        DL_UART_Main_transmitData(UART0, (uint8_t)_ptr[i]);
//        /* 等待数据传输完毕 */
//        while (DL_UART_Main_isBusy(UART0))
//            ;
//    }

//    return len;
//}
/**************************************************************************
函数功能：UART3初始化函数
函数说明：
        配置波特率的参数计算公式
        BRD = UART Clock / （OVS * Baudrate） = integerDivisor.X
        fractionalDivisor = （X*64）+0.5
入口参数：选择波特率，目前只有115200和9600
返回  值：无
其    他：在本单片机时钟树当中：串口0 1 2用PD1的40M,但是串口3是PD1的80M
**************************************************************************/
void uart3_init(int baud)
{
    /* 复位启动外设 */
    DL_UART_reset(UART_3_INST);
     /* UART3上电使能（UART0） */
    DL_UART_enablePower(UART_3_INST);
     /* 配置UART3外设传输引脚引脚TX */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_3_IOMUX_TX, GPIO_UART_3_IOMUX_TX_FUNC);
    /* 配置UART3外设输入引脚引脚RX */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_3_IOMUX_RX, GPIO_UART_3_IOMUX_RX_FUNC);
    /* 设置UART3时钟配置 */
    DL_UART_setClockConfig(UART_3_INST, (DL_UART_Main_ClockConfig *)&gUART_3ClockConfig);
    /* 初始化UART3 */
    DL_UART_init(UART_3_INST, (DL_UART_Main_Config *)&gUART_3Config);
     /* 设置过采样率16倍过采样 */
    DL_UART_setOversampling(UART_3_INST, DL_UART_OVERSAMPLING_RATE_16X);
     /* 计算UART3波特率 */
    if (baud == 115200)
        DL_UART_Main_setBaudRateDivisor(UART3, (43), (26));
    if (baud == 9600)
        DL_UART_Main_setBaudRateDivisor(UART3, (520), (53));

    /* 启用串口接收中断 */
    DL_UART_enableInterrupt(UART_3_INST,
                                 DL_UART_MAIN_INTERRUPT_RX);
    /* 启用UART3的FIFO(先进先出缓存) */
    DL_UART_enableFIFOs(UART_3_INST);
    /* 设置FIFO的接收阈值 */
    DL_UART_setRXFIFOThreshold(UART_3_INST, DL_UART_RX_FIFO_LEVEL_1_2_FULL);
    /* 设置FIFO的发送阈值 */
    DL_UART_setTXFIFOThreshold(UART_3_INST, DL_UART_TX_FIFO_LEVEL_1_2_EMPTY);
    /* UART3最终使能 */
    DL_UART_Main_enable(UART_3_INST);
    /* 清楚UART3的中断 */
    NVIC_ClearPendingIRQ(UART3_INT_IRQn);
    /*UART3中断使能 */
    NVIC_EnableIRQ(UART3_INT_IRQn);
}

/**************************************************************************
函数功能：UART1中断服务函数
函数说明：无
入口参数：无
返回  值：无
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
