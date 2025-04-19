#include "uart1.h"

/**************************************************************************
函数功能：UART1初始化函数
函数说明：
        配置波特率的参数计算公式
        BRD = UART Clock / （OVS * Baudrate） = integerDivisor.X
        fractionalDivisor = （X*64）+0.5
入口参数：选择波特率，目前只有115200和9600
返回  值：无
**************************************************************************/
static const DL_UART_Main_ClockConfig UART1_ClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Config UART1_Config = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,			// 正常模式运行
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,		// 启用 UART 发送器和接收器
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,      // 禁用流量控制  其他： 启用请求发送/启用清除发送/启用请求发送和清除发送
    .parity      = DL_UART_MAIN_PARITY_NONE,			// 禁用奇偶校验和生成
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,		// 8数据位
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE			// 1停止位
};
void uart1_init(int baud)
{
	DL_UART_reset(UART1);									// 复位串口0
	DL_UART_enablePower(UART1);							// 开启串口0外设电源
	DL_UART_enable(UART1);									// 使能串口0
	DL_GPIO_initPeripheralInputFunction(UART1_RX_PINCM,UART1_RX_FUNC);		// 使能rx管脚输入复用功能
	DL_GPIO_initPeripheralOutputFunction(UART1_TX_PINCM,UART1_TX_FUNC);
	DL_UART_setClockConfig(UART1,\
		(DL_UART_ClockConfig *) &UART1_ClockConfig);	// 设置串口0时钟
	DL_UART_init(UART1,(DL_UART_Config *) &UART1_Config);
	
	/* 通过设置超采样和波特率除数配置波特率 */
	/* 4000000/16/9600=208.333 整数部分为208，小数部分为33.3，33.3%64=21 */
	/* 40000000为时钟频率，16为采样率，9600为波特率，64为固定常数，208与21为要填入的数据 */
	/* 尽管知道计算方法，但是要使用其他波特率的话，依然建议用工具配 */
	DL_UART_setOversampling(UART1,DL_UART_OVERSAMPLING_RATE_16X);
	if(baud==9600)
	{
		DL_UART_setBaudRateDivisor(UART1,260,27);
	}
	else if(baud==115200)
	{
		DL_UART_setBaudRateDivisor(UART1,21,45);
	}
		
	DL_UART_enableInterrupt(UART1,DL_UART_INTERRUPT_RX);	// 接收延时中断
	DL_UART_enable(UART1);
    NVIC_EnableIRQ(UART1_INT_IRQn);						// 开启中断
}

/**************************************************************************
函数功能：UART1中断服务函数
函数说明：无
入口参数：无
返回  值：无
**************************************************************************/
uint8_t g_usart1_rx_buf[20];
uint16_t g_usart1_rx_sta = 0;

void UART_1_INST_IRQHandler(void)
{
	if(DL_UART_Main_getPendingInterrupt(UART1)==DL_UART_MAIN_IIDX_RX) 
	{
		uint8_t rxdata;
		rxdata = DL_UART_receiveData(UART1);
		if ((g_usart1_rx_sta & 0x8000) == 0) /* 接收未完成 */
		{
			if (g_usart1_rx_sta & 0x4000) /* 接收到了0x6B */
			{
					if (g_usart1_rx_buf[0] == 0x01&&g_usart1_rx_buf[1]==0x36)
					{
						g_usart1_rx_sta |= 0x8000; /* 收到了0x01,0x36,标记接收完成了 */
						g_usart1_rx_sta = 0;
					}
					else
					{
						g_usart1_rx_sta = 0; /* 接收错误, 重新开始 */
						g_usart1_rx_buf[0]=0;
					}
			}
			else /* 还没收到0x06B*/
			{
				if (rxdata == 0x6B)
				{
					g_usart1_rx_sta |= 0x4000; /* 标记接收到了 0x6B */
				}
				else
				{
					g_usart1_rx_buf[g_usart1_rx_sta & 0X3FFF] = rxdata; /* 存储数据到 g_usart_rx_buf */
					g_usart1_rx_sta++;
					if (g_usart1_rx_sta > (12 - 1))
					{
						g_usart1_rx_sta = 0; /* 接收数据溢出, 重新开始接收 */
						g_usart1_rx_buf[0]=0;
					}
				}
			}
		}
	}
	DL_UART_Main_clearInterruptStatus(UART1,DL_UART_MAIN_IIDX_RX);
}


/* 步进电机UART1指令发送函数 */
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
	__IO uint8_t i = 0;
	
	for(i=0; i < len; i++) { DL_UART_transmitData(UART1,cmd[i]); delay_us(100);}
}
