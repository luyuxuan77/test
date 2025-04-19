#include "uart0.h"
#include "State_Machine.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "udma.h"
#include "track.h"

/**************************************************************************
函数功能：串口0中断服务函数
入口参数：无
函数说明：无
**************************************************************************/
void UART0_IRQHandler(void)
{
//	Blue_Light_Control(1);
//    uint32_t ui32Status;
//    ui32Status = MAP_UARTIntStatus(UART0_BASE, true);  // 读取串口0的中断状态
//	printf("456\r\n");
//    MAP_UARTIntClear(UART0_BASE, ui32Status);  // 清除串口0的中断状态
//    while(MAP_UARTCharsAvail(UART0_BASE))  // 判断缓存区是否有字符
//    {
//		  char c = 0;
//        c = UARTCharGetNonBlocking(UART0_BASE);
//		printf("%c			%x				%d\r\n",c,c,c);
//		if(c == 48)
//		{
//			printf("1\r\n");
//			UWB_Current_Angle = 20;
//			UWB_Current_Distance = 120;
//		}
//		else if(c == 49)
//		{
//			printf("2\r\n");
//			UWB_Current_Angle = 10;
//			UWB_Current_Distance = 110;
//		}
//		else if(c == 50)
//		{
//			printf("3\r\n");
//			UWB_Current_Angle = 5;
//			UWB_Current_Distance = 105;
//		}
//		else if(c == 51)
//		{
//			printf("4\r\n");
//			UWB_Current_Angle = 0;
//			UWB_Current_Distance = 100;
//		}
//        if(c == '2')
//        {
//			step = 2;
//        }
//        else if(c == 0x03)
//        {
//           step = 3;
//        }
//        else if(c == 0x04)
//        {
//           step = 4;
//        }


        //MAP_UARTCharPutNonBlocking(UART0_BASE, MAP_UARTCharGetNonBlocking(UART0_BASE));  // 接收并返回从指定串口FIFO发送的一个字符
//    }
}

/**************************************************************************
函数功能：串口0初始化
入口参数：ui32Baud：串口波特率、ui32SrcClock：串口源时钟频率
返回参数：无
**************************************************************************/
void uart0_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);	//使能UART0外设
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);	//使能GPIOA外设
	MAP_GPIOPinConfigure(GPIO_PA0_U0RX);	//配置GPIO引脚备用功能
	MAP_GPIOPinConfigure(GPIO_PA1_U0TX);	//配置GPIO引脚备用功能
	
	MAP_UARTFIFOEnable(UART0_BASE);	//使能FIFO
	MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);	//配置UART外围设备使用的引脚PA0|PA1
	MAP_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);	//为指定的UART设置波特率时钟源，此时为16MHz
	UARTStdioConfig(0 , ui32Baud, ui32SrcClock);			//参数设定函数，分别是串口编号、波特率、UART的时钟频率
	
	UARTIntRegister(UART0_BASE, UART0_IRQHandler);	//注册串口中断函数
	MAP_IntPrioritySet(INT_UART0, 3);				//设置中断优先级
	
	MAP_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);	//启动串口0中断源
	MAP_IntEnable(INT_UART0);	//启动串口0中断
	MAP_IntMasterEnable();		//启动处理器中断
}

/**************************************************************************
函数功能：串口0输出字符串函数
入口参数：pui8Buffer：字符串，ui32Count：字符串长度
返回参数：无
**************************************************************************/
void UART0Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{
	UARTCharPut(UART0_BASE,(unsigned char)ch);//如果用其他串口，?恍栊薷幕?UART0_BASE)即可。
	return ch;
}
#endif
