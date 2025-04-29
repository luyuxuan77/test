#include "uart0.h"
#include "bsp.h"

/**************************************************************************
�������ܣ��ض���fputc/fputs����
����˵���������ض���fputc/fputs����������fputc/fputs��������һ���������޷�
ʵ�ֶ�����ھ���printf��ӡ�����Ա����̽�USART1ʹ��printf��������Ҫ���и��ġ�
��ڲ�������
����  ֵ����
��    �����������printf�������һ���ַ������޷����������
��Ҫ�ض���fputs��ͬʱ�����ض���puts
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
    /* ����һ������ */
    DL_UART_Main_transmitData(UART0, (uint8_t)ch);
    /* �ȴ����ݴ������ */
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
        /* ����һ������ */
        DL_UART_Main_transmitData(UART0, (uint8_t)_ptr[i]);
        /* �ȴ����ݴ������ */
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
    /* ��λ�������� */
    DL_UART_Main_reset(UART_0_INST);
    /* UART�ϵ�ʹ�ܣ�UART0�� */
    DL_UART_Main_enablePower(UART_0_INST);
    /* ����UART0���贫����������TX */
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_UART_0_IOMUX_TX, GPIO_UART_0_IOMUX_TX_FUNC);
    /* ����UART0����������������RX */
    DL_GPIO_initPeripheralInputFunction(
        GPIO_UART_0_IOMUX_RX, GPIO_UART_0_IOMUX_RX_FUNC);
    /* ����UART0ʱ������ */
    DL_UART_Main_setClockConfig(UART_0_INST, (DL_UART_Main_ClockConfig *) &gUART_0ClockConfig);
    /* ��ʼ��UART0 */
    DL_UART_Main_init(UART_0_INST, (DL_UART_Main_Config *) &gUART_0Config);
    /* ���ù�������16�������� */
    DL_UART_Main_setOversampling(UART_0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    /* ����UART0������ */
    DL_UART_Main_setBaudRateDivisor(UART_0_INST, UART_0_IBRD_32_MHZ_115200_BAUD, UART_0_FBRD_32_MHZ_115200_BAUD);
    /* ���ô��ڽ����ж� */
    DL_UART_Main_enableInterrupt(UART_0_INST,DL_UART_MAIN_INTERRUPT_RX);
    /* ���ô��� */
    DL_UART_Main_enable(UART_0_INST);
    /* ��������ж� */
    NVIC_ClearPendingIRQ(UART0_INT_IRQn);
    /* �����ж� */
    NVIC_EnableIRQ(UART0_INT_IRQn);

}

volatile unsigned int delay_times = 0;
volatile unsigned char uart_data = 0;

//���ڷ��͵����ַ�
void uart0_send_char(char ch)
{
    //������0æ��ʱ��ȴ�����æ��ʱ���ٷ��ʹ��������ַ�
    while( DL_UART_isBusy(UART_0_INST) == true );
    //���͵����ַ�
    DL_UART_Main_transmitData(UART_0_INST, ch);
}

//���ڵ��жϷ�����
void UART_0_INST_IRQHandler(void)
{
    //��������˴����ж�
    switch( DL_UART_getPendingInterrupt(UART_0_INST) )
    {
        case DL_UART_IIDX_RX://����ǽ����ж�
            //�ӷ��͹��������ݱ����ڱ�����
            uart_data = DL_UART_Main_receiveData(UART_0_INST);
            //������������ٷ��ͳ�ȥ
            uart0_send_char(uart_data);
            break;
        
        default://�����Ĵ����ж�
            break;
    }
}

