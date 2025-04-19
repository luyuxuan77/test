#include "gyro_u5.h"

//�������������ݽṹ�����
Gyro_Data_TypeDef Gyro_Data;
/******************************************************************************
�������ܣ�UART5�жϷ���������ȡ���������ݲ�����(Attitude_Information_Acquisition()����)
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void UART5_IRQHandler(void)
{
	unsigned char res;
	
    uint32_t ui32Status;
    ui32Status = MAP_UARTIntStatus(UART5_BASE, true);  // ��ȡ����5���ж�״̬
    MAP_UARTIntClear(UART5_BASE, ui32Status);  // �������5���ж�״̬
    while(MAP_UARTCharsAvail(UART5_BASE))  // �жϻ������Ƿ����ַ�
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
			
			//��ӡ�������ǽ�������
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
�������ܣ�����5��ʼ��PE5(TX)��PE4(RX)
��ڲ�����ui32Baud�����ڲ����ʣ�ui32SrcClock��UART��Դʱ��Ƶ��
���ز�������
**************************************************************************/
void uart5_Init(uint32_t ui32Baud, uint32_t ui32SrcClock)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  // ʹ�� GPIOE ����
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);  // ʹ�� UART5 ����
    GPIOPinConfigure(GPIO_PE4_U5RX);  // ����GPIO���ŵı��ù���
    GPIOPinConfigure(GPIO_PE5_U5TX);  // ����GPIO���ŵı��ù���
	UARTFIFOEnable(UART5_BASE);  // ʹ��FIFO
	MAP_IntMasterEnable();  // �����������ж�
	MAP_IntEnable(INT_UART5);  // ��������5�ж�
	MAP_UARTIntEnable(UART5_BASE, UART_INT_RX | UART_INT_RT);  // ��������5���ж�Դ
    MAP_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);  // ����UART��Χ�豸ʹ�õ����� PE4|PE5
	UARTIntRegister(UART5_BASE, UART5_IRQHandler);  // ע�ᴮ���жϺ���
	IntPrioritySet(INT_UART5, 0);  // �����ж����ȼ�
    UARTClockSourceSet(UART5_BASE, UART_CLOCK_PIOSC);  // Ϊָ����UART���ò���ʱ��Դ����ʱΪ16MHz
	UARTConfigSetExpClk(UART5_BASE, ui32SrcClock, ui32Baud, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);  // ���ô��ڲ���
//	UARTStdioConfig(5, ui32Baud, ui32SrcClock);  // �����趨�������ֱ��Ǵ��ڱ�š������ʡ�UART��Դʱ��Ƶ��
}

/******************************************************************************
�������ܣ�UART5�����ַ�������
��ڲ�����pui8Buffer���ַ�����ui32Count���ַ�������
����  ֵ����
*******************************************************************************/ 
void UART5Send(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        MAP_UARTCharPutNonBlocking(UART5_BASE, *pui8Buffer++);
    }
}

/******************************************************************************
�������ܣ����������ݽ�������
��ڲ�����data_choice��ѡ����Ҫ������������(Gyro_A/Gyro_W/Gyro_Angle)
����  ֵ����
*******************************************************************************/ 
void Attitude_Information_Acquisition(unsigned char data_choice)
{
	if (Gyro_Data.receive_flag == 1 && Gyro_Data.receive_i == 0)	//ȷ�����ݰ�Ϊ������ͬһ֡����
	{
		if (Gyro_Data.receive_data[0] == 0x55)	//֡ͷ���
		{
			if (Gyro_Data.receive_data[1] == 0x51 && (data_choice & Gyro_A) == Gyro_A)	//���ݰ�����ж������ݽ���ѡ��
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

