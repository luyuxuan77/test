#include "gyro_u5.h"

//定义陀螺仪数据结构体变量
Gyro_Data_TypeDef Gyro_Data;
/******************************************************************************
函数功能：UART5中断服务函数，获取陀螺仪数据并解析(Attitude_Information_Acquisition()函数)
入口参数：无
返回  值：无
*******************************************************************************/ 
void UART5_IRQHandler(void)
{
	unsigned char res;
	
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART5_BASE, true);  // 读取串口5的中断状态
    MAP_UARTIntClear(UART5_BASE, ui32Status);  // 清除串口5的中断状态
    while(MAP_UARTCharsAvail(UART5_BASE))  // 判断缓存区是否有字符
    {
       	res = MAP_UARTCharGetNonBlocking(UART5_BASE);
		
		Gyro_Data.receive_data[Gyro_Data.receive_i] = res;
		if (Gyro_Data.receive_i == 0 && Gyro_Data.receive_data[0] != 0x55)	return;
		Gyro_Data.receive_i++;
		if (Gyro_Data.receive_i == 11)
		{
			Gyro_Data.receive_i = 0;
			Gyro_Data.receive_flag = 1;
			Attitude_Information_Acquisition(Gyro_Angle);
			
			//打印出陀螺仪解析数据
//			UARTprintf("Angle_x = %d\r\n",(int)Angle_x);
//			UARTprintf("Angle_y = %d\r\n",(int)Angle_y);
//			UARTprintf("Angle_z = %d\r\n",(int)Angle_z);
//			
//			UARTprintf("W_x = %d\r\n",(int)W_x);
//			UARTprintf("W_y = %d\r\n",(int)W_y);
//			UARTprintf("W_z = %d\r\n",(int)W_z);
//			
//			UARTprintf("A_x = %d\r\n",(int)A_x);
//			UARTprintf("A_y = %d\r\n",(int)A_y);
//			UARTprintf("A_z = %d\r\n",(int)A_z);
//			UARTprintf("\r\n\r\n");
		}
    }
}

/**************************************************************************
函数功能：串口5初始化PE5(TX)、PE4(RX)
入口参数：ui32Baud；串口波特率；ui32SrcClock：UART的源时钟频率
返回参数：无
**************************************************************************/
void uart5_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // 使能 GPIOE 外设
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);  // 使能 UART5 外设
    GPIOPinConfigure(GPIO_PE4_U5RX);  // 配置GPIO引脚的备用功能
    GPIOPinConfigure(GPIO_PE5_U5TX);  // 配置GPIO引脚的备用功能
	UARTFIFOEnable(UART5_BASE);  // 使能FIFO
	MAP_IntMasterEnable();  // 启动处理器中断
	MAP_IntEnable(INT_UART5);  // 启动串口5中断
	MAP_UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);  // 启动串口5的中断源
    MAP_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);  // 配置UART外围设备使用的引脚 PE4|PE5
	UARTIntRegister(UART5_BASE, UART5_IRQHandler);  // 注册串口中断函数
	IntPrioritySet(INT_UART5, 0);  // 设置中断优先级
    UARTClockSourceSet(UART5_BASE, UART_CLOCK_PIOSC);  // 为指定的UART设置波特时钟源，此时为16MHz
	UARTConfigSetExpClk(UART5_BASE, ui32SrcClock, ui32Baud, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);  // 配置串口参数
//	UARTStdioConfig(5, ui32Baud, ui32SrcClock);  // 参数设定函数，分别是串口编号、波特率、UART的源时钟频率
}

/******************************************************************************
函数功能：UART5发送字符串函数
入口参数：pui8Buffer：字符串；ui32Count：字符串长度
返回  值：无
*******************************************************************************/ 
void UART5Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART5_BASE, *pui8Buffer++);
    }
}

/******************************************************************************
函数功能：陀螺仪数据解析函数
入口参数：data_choice：选择需要解析出的数据(Gyro_A/Gyro_W/Gyro_Angle)
返回  值：无
*******************************************************************************/ 
void Attitude_Information_Acquisition(unsigned char data_choice)
{
	if (Gyro_Data.receive_flag == 1 && Gyro_Data.receive_i == 0)	//确保数据包为完整的同一帧数据
	{
		if (Gyro_Data.receive_data[0] == 0x55)	//帧头检测
		{
			if (Gyro_Data.receive_data[1] == 0x51 && (data_choice & Gyro_A) == Gyro_A)	//数据包类别判断与数据解析选择
			{
				Gyro_Data.a[0] = (short)(Gyro_Data.receive_data[3] << 8 | Gyro_Data.receive_data[2]) / 32768.0 * 16;
				Gyro_Data.a[1] = (short)(Gyro_Data.receive_data[5] << 8 | Gyro_Data.receive_data[4]) / 32768.0 * 16;
				Gyro_Data.a[2] = (short)(Gyro_Data.receive_data[7] << 8 | Gyro_Data.receive_data[6]) / 32768.0 * 16;
			}
			else if (Gyro_Data.receive_data[1] == 0x52 && (data_choice & Gyro_W) == Gyro_W)
			{
				Gyro_Data.w[0] = (short)(Gyro_Data.receive_data[3] << 8 | Gyro_Data.receive_data[2]) / 32768.0 * 2000;
				Gyro_Data.w[1] = (short)(Gyro_Data.receive_data[5] << 8 | Gyro_Data.receive_data[4]) / 32768.0 * 2000;
				Gyro_Data.w[2] = (short)(Gyro_Data.receive_data[7] << 8 | Gyro_Data.receive_data[6]) / 32768.0 * 2000;
			}
			else if (Gyro_Data.receive_data[1] == 0x53 && (data_choice & Gyro_Angle) == Gyro_Angle)
			{
				Gyro_Data.angle[0] = (short)(Gyro_Data.receive_data[3] << 8 | Gyro_Data.receive_data[2]) / 32768.0 * 180.0;
				Gyro_Data.angle[1] = (short)(Gyro_Data.receive_data[5] << 8 | Gyro_Data.receive_data[4]) / 32768.0 * 180.0;
				Gyro_Data.angle[2] = (short)(Gyro_Data.receive_data[7] << 8 | Gyro_Data.receive_data[6]) / 32768.0 * 180.0;
			}
			Gyro_Data.tem = (short)(Gyro_Data.receive_data[9] << 8 | Gyro_Data.receive_data[8]) / 340.0 + 36.25;
			Gyro_Data.receive_flag = 0;
		}
	}
}

