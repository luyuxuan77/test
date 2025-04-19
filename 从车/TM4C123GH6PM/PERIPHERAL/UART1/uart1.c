#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "udma.h"
#include "uart0.h"
#include "uart1.h"
#include "State_Machine.h"
#include "track.h"

// uDMA控制器控制表
#if defined(ewarm)
#pragma data_alignment=1024
uint8_t ui8ControlTable[1024];
#elif defined(ccs)
#pragma DATA_ALIGN(ui8ControlTable, 1024)
uint8_t ui8ControlTable[1024];
#else
uint8_t ui8ControlTable[1024] __attribute__ ((aligned(1024)));
#endif

uint32_t g_ui32SrcBuf[MEM_BUFFER_SIZE];
uint32_t g_ui32DstBuf[MEM_BUFFER_SIZE];
uint8_t g_ui8TxBuf[UART_TXBUF_SIZE];
uint8_t g_ui8RxBufA[UART_RXBUF_SIZE];
uint8_t g_ui8RxBufB[UART_RXBUF_SIZE];
uint32_t g_ui32RxBufACount = 0;
uint32_t g_ui32RxBufBCount = 0;

/**************************************************************************
函数功能：串口1+uDMA中断处理函数
入口参数：void
函数说明：UART1的uDMA通道完成DMA传输时发生中断
**************************************************************************/
void UART1IntHandler(void)
{
    uint32_t ui32Status;
    uint32_t ui32Mode;
    ui32Status = MAP_UARTIntStatus(UART1_BASE, 1);  // 读取串口中断状态
    MAP_UARTIntClear(UART1_BASE, ui32Status);  // 清除所有挂起状态
    ui32Mode = MAP_uDMAChannelModeGet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT);  // 检查传输是否完成
    if(ui32Mode == UDMA_MODE_STOP)
    {
        g_ui32RxBufACount++;
		// 设置uDMA通道控制结构的传输参数
        MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufA, sizeof(g_ui8RxBufA));
    }
    ui32Mode = MAP_uDMAChannelModeGet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT);  // 检查传输是否完成
    if(ui32Mode == UDMA_MODE_STOP)
    {
        g_ui32RxBufBCount++;
		// 设置uDMA通道控制结构的传输参数
        MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufB, sizeof(g_ui8RxBufB));
    }
    if(!MAP_uDMAChannelIsEnabled(UDMA_CHANNEL_UART1TX))  // 若UART1DMA通道无法使用，则DMA传输完成
    {
		// 设置uDMA通道控制结构的传输参数
        MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC, g_ui8TxBuf, (void *)(UART1_BASE + UART_O_DR), sizeof(g_ui8TxBuf));
        MAP_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);  // 启用 uDMA UART1TX 通道
    }
}

/**************************************************************************
函数功能：串口1+uDMA初始化函数
入口参数：void
函数说明：
**************************************************************************/
//void InitUART1Transfer(uint32_t ui32Baud, uint32_t ui32SrcClock)
//{
//	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);  // 使能 uDMA 外设
//	MAP_IntEnable(INT_UDMAERR);  // 使能uDMA错误中断
//	MAP_uDMAEnable();  // 使能uDMA控制器
//	MAP_uDMAControlBaseSet(ui8ControlTable);  // 设置通道控制表的基地址
//	InitSWTransfer();
//	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  // 使能 GPIOB 外设
//    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);  // 使能 UART1 外设
//	GPIOPinConfigure(GPIO_PB0_U1RX);  // 配置GPIO引脚的备用功能
//	GPIOPinConfigure(GPIO_PB1_U1TX);  // 配置GPIO引脚的备用功能
//	MAP_IntEnable(INT_UART1);  // 启动串口1中断
//	MAP_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);  // 配置UART外围设备使用的引脚 PB0|PB1
//    MAP_UARTConfigSetExpClk(UART1_BASE, ui32SrcClock, ui32Baud, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);  // 配置串口参数
//    MAP_UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);  // 设置触发阈值
//    MAP_UARTEnable(UART1_BASE);  // 使能 UART1 的发送和接收
//    MAP_UARTDMAEnable(UART1_BASE, UART_DMA_RX | UART_DMA_TX);  // 使能 UART1DMA 的TX和RX通道
//    MAP_IntEnable(INT_UART1);  // 启动串口1中断
//    MAP_uDMAChannelAttributeDisable(UDMA_CHANNEL_UART1RX, UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);  // 初始化uDMA配置
//    // 设置uDMA通道控制结构的控制参数
//	MAP_uDMAChannelControlSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4);
//    MAP_uDMAChannelControlSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4);
//    // 设置uDMA通道控制结构的传输参数
//	MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_PRI_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufA, sizeof(g_ui8RxBufA));
//    MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1RX | UDMA_ALT_SELECT, UDMA_MODE_PINGPONG, (void *)(UART1_BASE + UART_O_DR), g_ui8RxBufB, sizeof(g_ui8RxBufB));
//    MAP_uDMAChannelAttributeDisable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_ALTSELECT | UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);  // 初始化uDMA配置
//    MAP_uDMAChannelAttributeEnable(UDMA_CHANNEL_UART1TX, UDMA_ATTR_USEBURST);  // 设置uDMA ADC0 通道的属性
//	// 设置uDMA通道控制结构的控制参数
//    MAP_uDMAChannelControlSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_4);
//	// 设置uDMA通道控制结构的传输参数
//    MAP_uDMAChannelTransferSet(UDMA_CHANNEL_UART1TX | UDMA_PRI_SELECT, UDMA_MODE_BASIC, g_ui8TxBuf, (void *)(UART1_BASE + UART_O_DR), sizeof(g_ui8TxBuf));
//    MAP_uDMAChannelEnable(UDMA_CHANNEL_UART1RX);  // 使能 uDMA 通道
//    MAP_uDMAChannelEnable(UDMA_CHANNEL_UART1TX);
//}

/**************************************************************************
函数功能：串口1初始化
入口参数：uint32_t、uint32_t
函数说明：输入值为串口波特率、UART的源时钟频率
**************************************************************************/
void uart1_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  // 使能 GPIOB 外设
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);  // 使能 UART1 外设
	GPIOPinConfigure(GPIO_PB0_U1RX);  // 配置GPIO引脚的备用功能
	GPIOPinConfigure(GPIO_PB1_U1TX);  // 配置GPIO引脚的备用功能
	UARTFIFOEnable(UART1_BASE);  // 使能FIFO
	MAP_IntMasterEnable();  // 启动处理器中断
	MAP_IntEnable(INT_UART1);  // 启动串口1中断
	MAP_UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);  // 启动串口1的中断源
	MAP_GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);  // 配置UART外围设备使用的引脚 PB0|PB1
	UARTIntRegister(UART1_BASE, USART1_IRQHandler);  // 注册串口中断函数
	IntPrioritySet(INT_UART1, 1);  // 设置中断优先级
	UARTClockSourceSet(UART1_BASE, UART_CLOCK_PIOSC);  // 为指定的UART设置波特时钟源，此时为16MHz
	UARTStdioConfig(1, ui32Baud, ui32SrcClock);  // 参数设定函数，分别是串口编号、波特率、UART的源时钟频率
}


void processSliderData(u8 frameIdentifier, u8 sliderValue) {
    switch (frameIdentifier) {
        case 0xF1:
            printf("处理滑条1的数据: %d\n", sliderValue);
			UWB_Current_Angle = sliderValue;
            // 在此处添加处理逻辑...
            break;
        case 0xF2:
            printf("处理滑条2的数据: %d\n", sliderValue);
			UWB_Current_Distance = sliderValue;
            // 在此处添加处理逻辑...
            break;
        case 0xF3:
            printf("处理滑条3的数据: %d\n", sliderValue);
            // 在此处添加处理逻辑...
            break;
        default:
            printf("未知的滑条标识符: %d\n", frameIdentifier);
    }
}

#define USART_REC_LEN 100 // 假设最大接收长度为100字节
u8 USART_RX_BUF[USART_REC_LEN];
u16 USART_RX_STA = 0;
/**************************************************************************
函数功能：串口1中断函数
入口参数：void
函数说明：作为串口1的中断函数，结构与F407定时器中断类似
**************************************************************************/
// 新增全局变量声明
volatile uint32_t g_u32Distance = 0; // 存储距离值的全局变量

void USART1_IRQHandler(void)
{
    u8 res;
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART1_BASE, true);
    Blue_Light_Control(1);

    static u8 USART_RX_BUF[100];
    static u8 USART_RX_STA = 0;

    while (MAP_UARTCharsAvail(UART1_BASE))
    {
        res = UARTCharGetNonBlocking(UART1_BASE);

        if ((USART_RX_STA & 0x8000) == 0)
        {
            if (USART_RX_STA == 0 && res == 0x44)  // 帧头检测
            {
                USART_RX_BUF[0] = res;
                USART_RX_STA = 1;
            }
            else if (USART_RX_STA >= 1 && USART_RX_STA < sizeof(USART_RX_BUF))
            {
                USART_RX_BUF[USART_RX_STA++] = res;

                if (res == 0x0A)  // 检测到帧尾
                {
                    USART_RX_STA |= 0x8000;
                    u16 length = USART_RX_STA & 0x7F;
                    USART_RX_BUF[length] = '\0';

                    // 距离值解析逻辑
                    u8 *p = USART_RX_BUF;
                    while ((p - USART_RX_BUF) < length - 9) {
                        if (memcmp(p, "Distance: ", 10) == 0) {
                            p += 10;
                            u8 *end = p;
                            uint32_t tempDistance = 0;  // 临时存储计算值

                            // 手动转换ASCII到十进制
                            while ((end - USART_RX_BUF) < length - 1) {
                                if (end[0] == 'm' && end[1] == 'm') {
                                    // 遍历数字字符
                                    for (u8 *q = p; q < end; q++) {
                                        if (*q >= '0' && *q <= '9') {
                                            tempDistance = tempDistance * 10 + (*q - '0');
                                        }
                                    }
                                    g_u32Distance = tempDistance/10;  // 更新全局变量
                                    break;
                                }
                                end++;
                            }
                            break;
                        }
                        p++;
                    }

                    USART_RX_STA = 0;
                }
            }
            else
            {
                USART_RX_STA = 0;
            }
        }
    }

    MAP_UARTIntClear(UART1_BASE, ui32Status);
}
//void USART1_IRQHandler(void)
//{
//    u8 res;
//    uint32_t ui32Status;
//    ui32Status = MAP_UARTIntStatus(UART1_BASE, true);  // 读取串口1的中断状态
////    Blue_Light_Control(1);

//    // 使用一个标志位来判断是否接收到完整的数据帧
//    static u8 USART_RX_BUF[100];  // 数据接收缓冲区，假设最大帧长为100字节
//    static u8 USART_RX_STA = 0;   // 状态寄存器：0=未接收，1=接收中，0x8000=接收完成

//    while (MAP_UARTCharsAvail(UART1_BASE))  // 判断缓存区是否有字符
//    {
//        res = UARTCharGetNonBlocking(UART1_BASE);

//        if ((USART_RX_STA & 0x8000) == 0)  // 接收未完成
//        {
//            if (USART_RX_STA == 0 && res == 0x44)  // 检查帧头 'D' (0x44)
//            {
//                USART_RX_BUF[0] = res;  // 存储帧头
//                USART_RX_STA = 1;  // 开始接收数据
//            }
//            else if (USART_RX_STA >= 1 && USART_RX_STA < sizeof(USART_RX_BUF))  // 接收接下来的字节
//            {
//                USART_RX_BUF[USART_RX_STA++] = res;

//                if (res == 0x0A)  // 结束标志：换行符 (0x0A)，表示帧尾
//                {
//                    USART_RX_STA |= 0x8000;  // 接收完成

//                    // 校验 "Distance: XXm" 结构
//                    if (USART_RX_BUF[1] == 0x69 && USART_RX_BUF[2] == 0x73 && USART_RX_BUF[3] == 0x74) 
//                    { // "ist" (匹配 "Distance:")
////                        printf("匹配到 Distance: 结构\n");

//                        // 确保第 11 个字节是空格 (0x20)
//                        if (USART_RX_BUF[10] == 0x20) 
//                        {
////                            printf("检测到空格，准备提取距离\n");

//                            // 读取第 12 和 13 位数据（16进制表示的数）
//                            u8 hex1 = USART_RX_BUF[12];  // 第一个16进制数
//                            u8 hex2 = USART_RX_BUF[11];  // 第二个16进制数

//                            // **转换为10进制**
//                            int dec1 = (hex1 / 16) * 10 + (hex1 % 16);  // 把16进制转成10进制
//                            int dec2 = (hex2 / 16) * 10 + (hex2 % 16);  

//                            // **最终计算**
//                            int distance = dec1 + dec2;

////                            // 打印出距离
////                            printf("%c", USART_RX_BUF[11]);
////							printf("%c", USART_RX_BUF[12]);
////							printf("%c", USART_RX_BUF[13]);
////							printf("\n");

//                        }
//                    }

//                    // 重置接收状态，准备接收下一帧
//                    USART_RX_STA = 0;
//                }
//            }
//            else  // 接收错误或超出缓冲区范围
//            {
//                USART_RX_STA = 0;  // 重置接收状态
//            }
//        }
//    }

//    MAP_UARTIntClear(UART1_BASE, ui32Status);  // 清除串口1的中断状态
//}



/**************************************************************************
函数功能：串口1输出字符串函数
入口参数：const uint8_t，uint32_t
函数说明：作为串口1输出字符串的函数，注意字符串的长度
**************************************************************************/
void UART1Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART1_BASE, *pui8Buffer++);
    }
}
