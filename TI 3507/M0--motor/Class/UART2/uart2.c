#include "uart2.h"


/**************************************************************************
�������ܣ�UART2��ʼ������
����˵����
        ���ò����ʵĲ������㹫ʽ
        BRD = UART Clock / ��OVS * Baudrate�� = integerDivisor.X
        fractionalDivisor = ��X*64��+0.5
��ڲ�����ѡ�����ʣ�Ŀǰֻ��115200��9600
����  ֵ����
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
    /* ��λ����UART2 */
    DL_UART_reset(UART_2_INST);
    /* UART2�ϵ�ʹ�� */
    DL_UART_enablePower(UART_2_INST);
    /* ����UART2�Ĵ������� */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_2_IOMUX_TX, GPIO_UART_2_IOMUX_TX_FUNC);
    /* ����UART2���������� */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_2_IOMUX_RX, GPIO_UART_2_IOMUX_RX_FUNC);
    /* ����UART2��ʱ������ */
    DL_UART_setClockConfig(UART_2_INST, (DL_UART_Main_ClockConfig *)&gUART_2ClockConfig);
    /* ��ʼ��UART2 */
    DL_UART_init(UART_2_INST, (DL_UART_Main_Config *)&gUART_2Config);
    /* ���ù�������16�������� */
    DL_UART_setOversampling(UART_2_INST, DL_UART_OVERSAMPLING_RATE_16X);

    /* ����UART0������ */
    if (baud == 9600)
        DL_UART_Main_setBaudRateDivisor(UART2, (260), (27));
    else if (baud == 115200)
        DL_UART_Main_setBaudRateDivisor(UART2, (21), (45));
    /* UART2�ж�ʹ�� */
    DL_UART_enableInterrupt(UART_2_INST,
                                 DL_UART_MAIN_INTERRUPT_RX);
    /* UART2����ʹ�� */
    DL_UART_enable(UART_2_INST);

    /* UART2�ж���� */
    NVIC_ClearPendingIRQ(UART2_INT_IRQn);
    /* UART2�ж�ʹ�� */
    NVIC_EnableIRQ(UART2_INT_IRQn);
}

/**************************************************************************
�������ܣ�UART2�жϷ�����
����˵�������ڽ���STM32F103�������ı�������ֵ
��ڲ�������
����  ֵ����
**************************************************************************/
uint8_t g_usart2_rx_buf[20];
uint8_t g_usart2_rx_buffff[20];
uint16_t g_usart2_rx_sta = 0;

int get_high_low_eight(uint8_t high,uint8_t low)
{
	uint16_t data=(high << 8) | low;
	int sum=0;
	if (data&0x8000)//Ϊ����
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
    if ((g_usart2_rx_sta & 0x8000) == 0) /* ����δ���? */
    {
        if (g_usart2_rx_sta & 0x4000) /* ���յ���0x0d? */
        {
            if (rxdata != 0x0a) /* ���յ���0x0a? (�����Ƚ��յ���0x0d,�ż��0x0a) */
            {
                g_usart2_rx_sta = 0; /* ���մ���, ���¿�ʼ */
                g_usart2_rx_buf[0] = 0;
            }
            else
            {
                g_usart2_rx_sta |= 0x8000; /* �յ���0x0a,��ǽ�������� */
                if (g_usart2_rx_buf[0] == 0xff)
                {
                    for (int i = 1; i < (g_usart2_rx_sta & 0X3FFF); i++)
                        g_usart2_rx_buffff[i] = g_usart2_rx_buf[i];
                }
                g_usart2_rx_sta = 0;
                g_usart2_rx_buf[0] = 0;
            }
        }
        else /* ��û�յ�0x0d */
        {
            if (rxdata == 0x0d)
            {
                g_usart2_rx_sta |= 0x4000; /* ��ǽ��յ��� 0x0d */
            }
            else
            {
                g_usart2_rx_buf[g_usart2_rx_sta & 0X3FFF] = rxdata; /* �洢���ݵ� g_usart_rx_buf */
                g_usart2_rx_sta++;
                if (g_usart2_rx_sta > (20 - 1))
                {
                    g_usart2_rx_sta = 0; /* �����������, ���¿�ʼ���� */
                }
            }
        }
    }
}