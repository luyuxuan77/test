#include "uart2.h"


/**************************************************************************
函数功能：UART2初始化函数
函数说明：
        配置波特率的参数计算公式
        BRD = UART Clock / （OVS * Baudrate） = integerDivisor.X
        fractionalDivisor = （X*64）+0.5
入口参数：选择波特率，目前只有115200和9600
返回  值：无
**************************************************************************/
static const DL_UART_Main_ClockConfig gUART_2ClockConfig = {
    .clockSel = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1};

static const DL_UART_Main_Config gUART_2Config = {
    .mode = DL_UART_MAIN_MODE_NORMAL,
    .direction = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity = DL_UART_MAIN_PARITY_NONE,
    .wordLength = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits = DL_UART_MAIN_STOP_BITS_ONE};
void uart2_init(int baud)
{
    /* 复位启用UART2 */
    DL_UART_reset(UART_2_INST);
    /* UART2上电使能 */
    DL_UART_enablePower(UART_2_INST);
    /* 配置UART2的传输引脚 */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_2_IOMUX_TX, GPIO_UART_2_IOMUX_TX_FUNC);
    /* 配置UART2的输入引脚 */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_2_IOMUX_RX, GPIO_UART_2_IOMUX_RX_FUNC);
    /* 设置UART2的时钟配置 */
    DL_UART_setClockConfig(UART_2_INST, (DL_UART_Main_ClockConfig *)&gUART_2ClockConfig);
    /* 初始化UART2 */
    DL_UART_init(UART_2_INST, (DL_UART_Main_Config *)&gUART_2Config);
    /* 设置过采样率16倍过采样 */
    DL_UART_setOversampling(UART_2_INST, DL_UART_OVERSAMPLING_RATE_16X);

    /* 计算UART0波特率 */
    if (baud == 9600)
        DL_UART_Main_setBaudRateDivisor(UART2, (260), (27));
    else if (baud == 115200)
        DL_UART_Main_setBaudRateDivisor(UART2, (21), (45));
    /* UART2中断使能 */
    DL_UART_enableInterrupt(UART_2_INST,
                                 DL_UART_MAIN_INTERRUPT_RX);
    /* UART2最终使能 */
    DL_UART_enable(UART_2_INST);

    /* UART2中断清除 */
    NVIC_ClearPendingIRQ(UART2_INT_IRQn);
    /* UART2中断使能 */
    NVIC_EnableIRQ(UART2_INT_IRQn);
}

/**************************************************************************
函数功能：UART2中断服务函数
函数说明：用于解析STM32F103传过来的编码器的值
入口参数：无
返回  值：无
**************************************************************************/
uint8_t g_usart2_rx_buf[20];
uint8_t g_usart2_rx_buffff[20];
uint16_t g_usart2_rx_sta = 0;

int get_high_low_eight(uint8_t high,uint8_t low)
{
	uint16_t data=(high << 8) | low;
	int sum=0;
	if (data&0x8000)//为负数
	{
		sum= data-65536;
	}
	else sum=data;
return sum;
}
int get_encoder(int num)
{
    int a = 0;
    if (num == 1)
        a = get_high_low_eight(g_usart2_rx_buffff[1], g_usart2_rx_buffff[2]);
    else if(num == 2)
        a = get_high_low_eight(g_usart2_rx_buffff[3], g_usart2_rx_buffff[4]);
    else if(num == 3)
        a = get_high_low_eight(g_usart2_rx_buffff[5], g_usart2_rx_buffff[6]);
    else if(num == 4)
        a = get_high_low_eight(g_usart2_rx_buffff[7], g_usart2_rx_buffff[8]);
    return a;
}
void UART_2_INST_IRQHandler(void)
{
    uint8_t rxdata;
    rxdata = DL_UART_Main_receiveData(UART2);
    if ((g_usart2_rx_sta & 0x8000) == 0) /* 接收未完成? */
    {
        if (g_usart2_rx_sta & 0x4000) /* 接收到了0x0d? */
        {
            if (rxdata != 0x0a) /* 接收到了0x0a? (必须先接收到到0x0d,才检查0x0a) */
            {
                g_usart2_rx_sta = 0; /* 接收错误, 重新开始 */
                g_usart2_rx_buf[0] = 0;
            }
            else
            {
                g_usart2_rx_sta |= 0x8000; /* 收到了0x0a,标记接收完成了 */
                if (g_usart2_rx_buf[0] == 0xff)
                {
                    for (int i = 1; i < (g_usart2_rx_sta & 0X3FFF); i++)
                        g_usart2_rx_buffff[i] = g_usart2_rx_buf[i];
                }
                g_usart2_rx_sta = 0;
                g_usart2_rx_buf[0] = 0;
            }
        }
        else /* 还没收到0x0d */
        {
            if (rxdata == 0x0d)
            {
                g_usart2_rx_sta |= 0x4000; /* 标记接收到了 0x0d */
            }
            else
            {
                g_usart2_rx_buf[g_usart2_rx_sta & 0X3FFF] = rxdata; /* 存储数据到 g_usart_rx_buf */
                g_usart2_rx_sta++;
                if (g_usart2_rx_sta > (20 - 1))
                {
                    g_usart2_rx_sta = 0; /* 接收数据溢出, 重新开始接收 */
                }
            }
        }
    }
}