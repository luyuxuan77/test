#include "uart0.h"

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

/**************************************************************************
�������ܣ�UART0��ʼ������
����˵����
        ���ò����ʵĲ������㹫ʽ
        BRD = UART Clock / ��OVS * Baudrate�� = integerDivisor.X
        fractionalDivisor = ��X*64��+0.5
��ڲ�����ѡ�����ʣ�Ŀǰֻ��115200��9600
����  ֵ����
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
    /* ��λ�������� */
    DL_UART_reset(UART_0_INST);
    /* UART�ϵ�ʹ�ܣ�UART0�� */
    DL_UART_enablePower(UART_0_INST);
    /* ����UART0���贫����������TX */
    DL_GPIO_initPeripheralOutputFunction(
        UART0_TX_PINCM, UART0_TX_FUNC);
    /* ����UART0����������������RX */
    DL_GPIO_initPeripheralInputFunction(
        UART0_RX_PINCM, UART0_RX_FUNC);
    /* ����UART0ʱ������ */
    DL_UART_setClockConfig(UART_0_INST, (DL_UART_Main_ClockConfig *)&gUART_0ClockConfig);
    /* ��ʼ��UART0 */
    DL_UART_init(UART_0_INST, (DL_UART_Main_Config *)&gUART_0Config);
    /* ���ù�������16�������� */
    DL_UART_setOversampling(UART_0_INST, DL_UART_OVERSAMPLING_RATE_16X);
    /* ����UART0������ */
    if (baud == 9600)
        DL_UART_Main_setBaudRateDivisor(UART_0_INST, (260), (27));
    else if (baud == 115200)
        DL_UART_Main_setBaudRateDivisor(UART_0_INST, (21), (45));
    /* ���ô��ڽ����ж� */
    DL_UART_enableInterrupt(UART_0_INST, DL_UART_MAIN_INTERRUPT_RX);
    /* ���ô��� */
    DL_UART_enable(UART_0_INST);
    /* ��������ж� */
    NVIC_ClearPendingIRQ(UART0_INT_IRQn);
    /* �����ж� */
    NVIC_EnableIRQ(UART0_INT_IRQn);

//	DL_UART_clearInterrupt(UART0, DL_UART_MAIN_IIDX_RX);
}

/**************************************************************************
�������ܣ�UART0�жϷ�����
**����˵����JY901ͨѶ����
**�����ʣ�115200
**pitch:������ ����:1��   ��Χ:-90.0�� <---> +90.0��
**roll:�����  ����:1��   ��Χ:-180.0��<---> +180.0��
**yaw:�����   ����:1��   ��Χ:-180.0��<---> +180.0��
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
    if(DL_UART_Main_getPendingInterrupt(UART0)==DL_UART_MAIN_IIDX_RX)    //���յ�����
    {
        res=DL_UART_receiveData(UART0);        //�����յ������ݸ�������res
        g_usart0_rx_buf[t]=res;         //���յ���������ȡ������USART_RX_BUF[]��
        if(g_usart0_rx_buf[0]==0x55)    //�ж��Ƿ��յ�֡ͷ
        {
            t++;                        //�����������ݵ�������
            rollL=g_usart0_rx_buf[2];   //���Ƕ����ݵ�����洢
            rollH=g_usart0_rx_buf[3];
            pitchL =g_usart0_rx_buf[4];
            pitchH =g_usart0_rx_buf[5];
            yawL =g_usart0_rx_buf[6];
            yawH =g_usart0_rx_buf[7];
            Roll =((rollH <<8)|rollL)/327*180*0.01;	//�Ƕ�ֵ��16��������תʮ��������
            Pitch =((pitchH <<8)|pitchL)/327*180*0.01;
            Yaw =((yawH <<8)|yawL)/327*180*0.01;
            
            if((t>1)&&(g_usart0_rx_buf[1]!=0x53)) t=0;  //�ж��Ƿ��յ��ǶȰ�ͷ0x53,���û���յ�����ʹtΪ0���¿�ʼ�ȴ��ǶȰ��ĵ���
        }
        if(t>=11) t=0;                   //���յ����ݳ��ȴ���10ʱ��ʹt��0��ʼ���½������ݵ�����
    }
}

/**************************************************************************
�������ܣ�����ת�ַ���
����˵������
��ڲ�����value ->������  *str ->�ַ���  precision ->������Ч����λ��
����  ֵ����
**************************************************************************/
void doubleToStr(double value, char *str, int precision)
{
    long wholePart = (long)value;
    double fractionalPart = fmod(value, 1.0);
    /*ʹ��sprintf�������������ֺ�һ��С����д���ַ���*/
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
