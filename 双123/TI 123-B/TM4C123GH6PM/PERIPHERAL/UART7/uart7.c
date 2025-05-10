#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart7.h"
#include "uart1.h"
unsigned char USART7_RX_BUF[13] = {0};
unsigned char i;
unsigned char Distance;//cm
/**************************************************************************
函数功能：串口7初始化
入口参数：uint32_t、uint32_t
函数说明：输入值为串口波特率、UART的源时钟频率
**************************************************************************/
void uart7_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // 使能 GPIOE 外设
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);  // 使能 UART7 外设
    GPIOPinConfigure(GPIO_PE0_U7RX);  // 配置GPIO引脚的备用功能
    GPIOPinConfigure(GPIO_PE1_U7TX);  // 配置GPIO引脚的备用功能
	UARTFIFOEnable(UART7_BASE);  // 使能FIFO
	MAP_IntMasterEnable();  // 启动处理器中断
	MAP_IntEnable(INT_UART7);  // 启动串口7中断
	MAP_UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_RT);  // 启动串口7的中断源
    MAP_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);  // 配置UART外围设备使用的引脚 PE4|PE5
	UARTIntRegister(UART7_BASE, USART7_IRQHandler);  // 注册串口中断函数
	IntPrioritySet(INT_UART7, 0);  // 设置中断优先级
    UARTClockSourceSet(UART7_BASE, UART_CLOCK_PIOSC);  // 为指定的UART设置波特时钟源，此时为16MHz
	UARTConfigSetExpClk(UART7_BASE, ui32SrcClock, ui32Baud, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);  // 配置串口参数
//	UARTStdioConfig(7, ui32Baud, ui32SrcClock);  // 参数设定函数，分别是串口编号、波特率、UART的源时钟频率
}

int data_test(unsigned char data[])
{
	if(data[0]!=0xa3) return false;  //脰隆脥路
	if(data[1]!=0xb3) return false;  //脰隆脥路
	if(data[9]!=0xc3) return false;  //脰隆脦虏
	
	return true;
}

/**************************************************************************
函数功能：串口7中断函数
入口参数：void
函数说明：作为串口7的中断函数，结构与F407定时器中断类似
**************************************************************************/
void USART7_IRQHandler(void)
{
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART7_BASE, true);  // 读取串口7的中断状态
 	   MAP_UARTIntClear(UART7_BASE, ui32Status);  // 清除串口7的中断状态
    while(MAP_UARTCharsAvail(UART7_BASE))  // 判断缓存区是否有字符
    {
		unsigned char res; 
		res=UARTCharGetNonBlocking(UART7_BASE);
		USART7_RX_BUF[i++]=res;
		if(USART7_RX_BUF[0]!=0xa3) i=0;
		if((i==2)&&(USART7_RX_BUF[1]!=0xb3)) i=0; 
		if(i==10)i = 0;
		if(data_test(USART7_RX_BUF))
		{ 
			Distance = USART7_RX_BUF[2];
		}
//		if(i == 1 && USART7_RX_BUF[0] !=0xAA)//判断是否接收到数据包头9
//		{
//			i = 0;
//			return;
//		}
//		if(i == 4 && USART7_RX_BUF[3] != 0x22)
//		{
//			i =0;
//			return;
//		}
//		if(i > 12)
//		{
//			for(int z = 0;z < 12;z++)
//			{
//				sum += USART7_RX_BUF[z];
//			}
//			if(sum == USART7_RX_BUF[12])
//			{
//				laser[0]=USART7_RX_BUF[6];
//				laser[1]=USART7_RX_BUF[7];
//				laser[2]=USART7_RX_BUF[8];
//				laser[3]=USART7_RX_BUF[9];
//				i = 0;
//				for (int j = 0; j < 4; j++) 
//				{
//					result = result << 8 |laser [j];//十六进制的1位相当于二进制的4位，所以二进制左移8位相当于十六进制左移2位
//				}
//			}
//			else
//			{
//				i = 0;
//			}
//		}
			//printf("%d\n",result);

        //MAP_UARTCharPutNonBlocking(UART7_BASE, MAP_UARTCharGetNonBlocking(UART7_BASE));  // 接收并返回从指定串口FIFO发送的一个字符
	}
}
/**************************************************************************
函数功能：串口7输出字符串函数
入口参数：const uint8_t，uint32_t
函数说明：作为串口7输出字符串的函数，注意字符串的长度
**************************************************************************/
void UART7Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART7_BASE, *pui8Buffer++);
    }
}


