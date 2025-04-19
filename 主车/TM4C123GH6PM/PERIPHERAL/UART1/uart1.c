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
void USART1_IRQHandler(void)
{
	u8 res;
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART1_BASE, true);  // 读取串口1的中断状态
//	Blue_Light_Control(1);
    while(MAP_UARTCharsAvail(UART1_BASE))  // 判断缓存区是否有字符
    {
        res = UARTCharGetNonBlocking(UART1_BASE);
//		printf("%X ",res);
		if ((USART_RX_STA & 0x8000) == 0) 
		{ // 接收未完成
			if (res == 0xA5 && USART_RX_STA == 0) 
			{ // 检查帧起始字节
				USART_RX_BUF[0] = res;
				USART_RX_STA = 1; // 开始接收新帧
			}
			else if (USART_RX_STA >= 1 && USART_RX_STA < 7) 
			{ // 接收接下来的字节
				USART_RX_BUF[USART_RX_STA++] = res;
				if (USART_RX_STA == 7) 
				{
					// 完整帧接收完成
					if (USART_RX_BUF[1] == 0x5A && USART_RX_BUF[6] == 0xAA) 
					{ // 校验帧结束字节和第二个起始字节
						// 计算校验和以验证数据完整性
						u8 checksum = (USART_RX_BUF[2] + USART_RX_BUF[3] + USART_RX_BUF[4]) % 256;
						if (checksum == USART_RX_BUF[5])
						{
							// 数据有效，处理第3、4、5位的目标值
							processSliderData(USART_RX_BUF[3], USART_RX_BUF[4]);
						}
					}
					USART_RX_STA = 0; // 重置接收状态标记
				}
			}
			else 
			{
				USART_RX_STA = 0; // 接收错误，重新开始
			}
		}
	}
	MAP_UARTIntClear(UART1_BASE, ui32Status);  // 清除串口1的中断状态
}

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
