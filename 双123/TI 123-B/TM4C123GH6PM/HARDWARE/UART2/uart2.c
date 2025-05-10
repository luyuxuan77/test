#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart2.h"
/**************************************************************************
函数功能：串口2初始化
入口参数：uint32_t、uint32_t
函数说明：输入值为串口波特率、UART的源时钟频率
**************************************************************************/
void uart2_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);  // 使能 GPIOD 外设
	//解锁PD7引脚，要放在UART2外设使用前，否则无法使用
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR)  |= GPIO_PIN_7;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x0;
	
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);  // 使能 UART2 外设
    GPIOPinConfigure(GPIO_PD6_U2RX);  // 配置GPIO引脚的备用功能
    GPIOPinConfigure(GPIO_PD7_U2TX);  // 配置GPIO引脚的备用功能
	UARTFIFOEnable(UART2_BASE);  // 使能FIFO
	MAP_IntMasterEnable();  // 启动处理器中断
	MAP_IntEnable(INT_UART2);  // 启动串口2中断
	MAP_UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);  // 启动串口2的中断源
    MAP_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);  // 配置UART外围设备使用的引脚 PD6|PD7
	UARTIntRegister(UART2_BASE, USART2_IRQHandler);  // 注册串口中断函数
	IntPrioritySet(INT_UART2, 0);  // 设置中断优先级
    UARTClockSourceSet(UART2_BASE, UART_CLOCK_PIOSC);  // 为指定的UART设置波特时钟源，此时为16MHz
    UARTStdioConfig(2, ui32Baud, ui32SrcClock);  // 参数设定函数，分别是串口编号、波特率、UART的源时钟频率
}

/**************************************************************************
函数功能：串口2中断函数
入口参数：void
函数说明：作为串口2的中断函数，结构与F407定时器中断类似
**************************************************************************/
void USART2_IRQHandler(void)
{
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART2_BASE, true);  // 读取串口2的中断状态
    MAP_UARTIntClear(UART2_BASE, ui32Status);  // 清除串口2的中断状态
    while(MAP_UARTCharsAvail(UART2_BASE))  // 判断缓存区是否有字符
    {
	} 
        MAP_UARTCharPutNonBlocking(UART2_BASE, MAP_UARTCharGetNonBlocking(UART2_BASE));  // 接收并返回从指定串口FIFO发送的一个字符
}

/**************************************************************************
函数功能：串口2输出字符串函数
入口参数：const uint8_t，uint32_t
函数说明：作为串口2输出字符串的函数，注意字符串的长度
**************************************************************************/
void UART2Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART2_BASE, *pui8Buffer++);
    }
}


