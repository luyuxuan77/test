#include "uart0.h"
#include "bsp.h"

/**************************************************************************
函数功能：重定义fputc/fputs函数
函数说明：用于重定义fputc/fputs函数，由于fputc/fputs函数仅有一个，所以无法
实现多个串口均用printf打印，所以本工程仅USART1使用printf，若有需要自行更改。
入口参数：略
返回  值：略
其    他：如果发现printf可以输出一般字符，而无法输出参数，
需要重定向fputs的同时，再重定向puts
**************************************************************************/
#pragma(__use_no_semihosting)
struct FILE
{
    int handle;
};
FILE __stdout;
void _sys_exit(int x)
{
    x = x;
}
int fputc(int ch, FILE *f)
{
    /* 发送一个数据 */
    DL_UART_Main_transmitData(UART0, (uint8_t)ch);
    /* 等待数据传输完毕 */
    while (DL_UART_Main_isBusy(UART0))
        ;

    return ch;
}
int fputs(const char *_ptr, register FILE *_fp)
{
    uint16_t i, len;
    len = strlen(_ptr);
    for (i = 0; i < len; i++)
    {
        /* 发送一个数据 */
        DL_UART_Main_transmitData(UART0, (uint8_t)_ptr[i]);
        /* 等待数据传输完毕 */
        while (DL_UART_Main_isBusy(UART0))
            ;
    }

    return len;
}

static const DL_UART_Main_ClockConfig gUART_0ClockConfig = {
    .clockSel    = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1
};

static const DL_UART_Main_Config gUART_0Config = {
    .mode        = DL_UART_MAIN_MODE_NORMAL,
    .direction   = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity      = DL_UART_MAIN_PARITY_NONE,
    .wordLength  = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits    = DL_UART_MAIN_STOP_BITS_ONE
};

void uart0_init(void)
{
    /* 复位启动外设 */
    DL_UART_Main_reset(UART_0_INST);
    /* UART上电使能（UART0） */
    DL_UART_Main_enablePower(UART_0_INST);
    /* 配置UART0外设传输引脚引脚TX */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_0_IOMUX_TX, GPIO_UART_0_IOMUX_TX_FUNC);
    /* 配置UART0外设输入引脚引脚RX */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_0_IOMUX_RX, GPIO_UART_0_IOMUX_RX_FUNC);
    /* 设置UART0时钟配置 */
    DL_UART_Main_setClockConfig(UART_0_INST, (DL_UART_Main_ClockConfig *) &gUART_0ClockConfig);
    /* 初始化UART0 */
    DL_UART_Main_init(UART_0_INST, (DL_UART_Main_Config *) &gUART_0Config);
    /* 设置过采样率16倍过采样 */
    DL_UART_Main_setOversampling(UART_0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    /* 计算UART0波特率 */
    DL_UART_Main_setBaudRateDivisor(UART_0_INST, UART_0_IBRD_32_MHZ_115200_BAUD, UART_0_FBRD_32_MHZ_115200_BAUD);
    /* 启用串口接收中断 */
    DL_UART_Main_enableInterrupt(UART_0_INST,DL_UART_MAIN_INTERRUPT_RX);
    /* 启用串口 */
    DL_UART_Main_enable(UART_0_INST);
    /* 清除串口中断 */
    NVIC_ClearPendingIRQ(UART0_INT_IRQn);
    /* 启用中断 */
    NVIC_EnableIRQ(UART0_INT_IRQn);

}

volatile unsigned int delay_times = 0;
volatile unsigned char uart_data = 0;

//串口发送单个字符
void uart0_send_char(char ch)
{
    //当串口0忙的时候等待，不忙的时候再发送传进来的字符
    while( DL_UART_isBusy(UART_0_INST) == true );
    //发送单个字符
    DL_UART_Main_transmitData(UART_0_INST, ch);
}

//串口的中断服务函数
void UART_0_INST_IRQHandler(void)
{
    //如果产生了串口中断
    switch( DL_UART_getPendingInterrupt(UART_0_INST) )
    {
        case DL_UART_IIDX_RX://如果是接收中断
            //接发送过来的数据保存在变量中
            uart_data = DL_UART_Main_receiveData(UART_0_INST);
            //将保存的数据再发送出去
            uart0_send_char(uart_data);
            break;
        
        default://其他的串口中断
            break;
    }
}

