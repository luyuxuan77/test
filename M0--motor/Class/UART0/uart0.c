#include "uart0.h"

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

/**************************************************************************
函数功能：UART0初始化函数
函数说明：
        配置波特率的参数计算公式
        BRD = UART Clock / （OVS * Baudrate） = integerDivisor.X
        fractionalDivisor = （X*64）+0.5
入口参数：选择波特率，目前只有115200和9600
返回  值：无
**************************************************************************/
static const DL_UART_Main_ClockConfig gUART_0ClockConfig = {
    .clockSel = DL_UART_MAIN_CLOCK_BUSCLK,
    .divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1};

static const DL_UART_Main_Config gUART_0Config = {
    .mode = DL_UART_MAIN_MODE_NORMAL,
    .direction = DL_UART_MAIN_DIRECTION_TX_RX,
    .flowControl = DL_UART_MAIN_FLOW_CONTROL_NONE,
    .parity = DL_UART_MAIN_PARITY_NONE,
    .wordLength = DL_UART_MAIN_WORD_LENGTH_8_BITS,
    .stopBits = DL_UART_MAIN_STOP_BITS_ONE};

void uart0_init(int baud)
{
    /* 复位启动外设 */
    DL_UART_reset(UART_0_INST);
    /* UART上电使能（UART0） */
    DL_UART_enablePower(UART_0_INST);
    /* 配置UART0外设传输引脚引脚TX */
    DL_GPIO_initPeripheralOutputFunction(
        UART0_TX_PINCM, UART0_TX_FUNC);
    /* 配置UART0外设输入引脚引脚RX */
    DL_GPIO_initPeripheralInputFunction(
        UART0_RX_PINCM, UART0_RX_FUNC);
    /* 设置UART0时钟配置 */
    DL_UART_setClockConfig(UART_0_INST, (DL_UART_Main_ClockConfig *)&gUART_0ClockConfig);
    /* 初始化UART0 */
    DL_UART_init(UART_0_INST, (DL_UART_Main_Config *)&gUART_0Config);
    /* 设置过采样率16倍过采样 */
    DL_UART_setOversampling(UART_0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    /* 计算UART0波特率 */
    if (baud == 9600)
        DL_UART_Main_setBaudRateDivisor(UART_0_INST, (260), (27));
    else if (baud == 115200)
        DL_UART_Main_setBaudRateDivisor(UART_0_INST, (21), (45));
    /* 启用串口接收中断 */
    DL_UART_enableInterrupt(UART_0_INST, DL_UART_MAIN_INTERRUPT_RX);
    /* 启用串口 */
    DL_UART_enable(UART_0_INST);
    /* 清除串口中断 */
    NVIC_ClearPendingIRQ(UART0_INT_IRQn);
    /* 启用中断 */
    NVIC_EnableIRQ(UART0_INT_IRQn);

//	DL_UART_clearInterrupt(UART0, DL_UART_MAIN_IIDX_RX);
}

/**************************************************************************
函数功能：UART0中断服务函数
**函数说明：JY901通讯串口
**波特率：115200
**pitch:俯仰角 精度:1°   范围:-90.0° <---> +90.0°
**roll:横滚角  精度:1°   范围:-180.0°<---> +180.0°
**yaw:航向角   精度:1°   范围:-180.0°<---> +180.0°
**************************************************************************/
u8 rollL,rollH;
u8 pitchL,pitchH;
u8 yawL,yawH;
int  Roll,Pitch,Yaw;
int t=0;
uint8_t g_usart0_rx_buf[20];
uint8_t gEchoData=0;
void UART_0_INST_IRQHandler(void)
{
    u8 res;
    if(DL_UART_Main_getPendingInterrupt(UART0)==DL_UART_MAIN_IIDX_RX)    //接收到数据
    {
        res=DL_UART_receiveData(UART0);        //将接收到的数据赋给变量res
        g_usart0_rx_buf[t]=res;         //将收到的数据提取到数组USART_RX_BUF[]中
        if(g_usart0_rx_buf[0]==0x55)    //判断是否收到帧头
        {
            t++;                        //依次输入数据到数组中
            rollL=g_usart0_rx_buf[2];   //各角度数据的数组存储
            rollH=g_usart0_rx_buf[3];
            pitchL =g_usart0_rx_buf[4];
            pitchH =g_usart0_rx_buf[5];
            yawL =g_usart0_rx_buf[6];
            yawH =g_usart0_rx_buf[7];
            Roll =((rollH <<8)|rollL)/327*180*0.01;	//角度值的16进度数据转十进制运算
            Pitch =((pitchH <<8)|pitchL)/327*180*0.01;
            Yaw =((yawH <<8)|yawL)/327*180*0.01;
            
            if((t>1)&&(g_usart0_rx_buf[1]!=0x53)) t=0;  //判断是否收到角度包头0x53,如果没有收到，则使t为0重新开始等待角度包的到来
        }
        if(t>=11) t=0;                   //当收到数据长度大于10时，使t从0开始重新接受数据到数组
    }
}

/**************************************************************************
函数功能：浮点转字符串
函数说明：无
入口参数：value ->浮点数  *str ->字符串  precision ->保留有效数字位数
返回  值：无
**************************************************************************/
void doubleToStr(double value, char *str, int precision)
{
    long wholePart = (long)value;
    double fractionalPart = fmod(value, 1.0);
    /*使用sprintf函数将整数部分和一个小数点写入字符串*/
    sprintf(str, "%ld.", wholePart);
    str += strlen(str);
    for (int i = 0; i < precision; ++i)
    {
        fractionalPart *= 10;
        long digit = (long)fractionalPart;
        sprintf(str, "%ld", digit);
        str += strlen(str);
        fractionalPart -= digit;
    }
    *str = '\0';
}
