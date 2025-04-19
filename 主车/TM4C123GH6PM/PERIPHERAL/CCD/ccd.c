#include "ccd.h"
#include "rgb.h"
#include "motor.h"
#include "my_adc.h"
#include "mymath.h"
#include "control.h"

/******************************************************************************
�������ܣ�����CCDģ���ʼ����ʹ��ADC0(PE3)��PB2|PB3
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void CCD_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  //ʹ�� GPIOB ����
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);  //ʹ��PB2|PB3�������
	MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);	//����+����
	ADC0_Init();
}

//����CCDģ�����ݽṹ��
CCD_TypeDef CCD_Data;
/******************************************************************************
�������ܣ����͸�����CCD�̶��źŴ����ɼ���������128��ʱ���źſ�ʼ�ɼ����洢�ɼ�ԭʼֵ
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Get_CCD_Value(void)
{
	unsigned char i;
	
	CCD_CLK_DOWN;
	CCD_SI_DOWN;
	Dly_us();
	
	CCD_CLK_DOWN;
	CCD_SI_UP;
	Dly_us();

	CCD_CLK_UP;
	Dly_us();
	
	CCD_SI_DOWN;
	Dly_us();
	
	for (i = 0; i < 128; i++)
	{
		CCD_CLK_DOWN;
		Dly_us();
		
		CCD_Data.receive_data[i] = ADC0_Get() >> 4;
		
		CCD_CLK_UP;
		Dly_us();
	}
	CCD_Data.receive_flag = 1;
}

/******************************************************************************
�������ܣ�����us����ʱ
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Dly_us(void)
{
	int ii;    
	for(ii=0;ii<10;ii++);
}

/******************************************************************************
�������ܣ����������ֵ������
��ڲ�����mdoe:������Ϣ���0:����ADC�ɼ���ԭʼֵ��1:�������ݶ�ֵ���������ֵ(ȷ���н��й���ֵ������)
����  ֵ����
*******************************************************************************/ 
void Send_To_Serial_Port(unsigned char mode)
{
	unsigned char i;
	if (CCD_Data.receive_flag == 1)
	{
		UARTprintf("{");
		for(i=0;i<128;i++)
		{
			if (mode) UARTprintf("%d,",CCD_Data.binaryzation_data[i]);
			else UARTprintf("%d,",CCD_Data.receive_data[i]);
		}
		UARTprintf("}");
		UARTprintf("\r\n");UARTprintf("\r\n");UARTprintf("\r\n");
		
		CCD_Data.receive_flag = 0;
	}
}

/******************************************************************************
�������ܣ��������͵���Ϣͨ�����ڷ�������λ��
��ڲ�����mode��0���Ͳɼ�����ԭʼ���ݣ�1���;�������������
����  ֵ����
*******************************************************************************/ 
void Send_To_Debug(unsigned char mode)
{ 
	int i;
	if (CCD_Data.receive_flag == 1)
	{
		UARTprintf("*");
		UARTprintf("LD");
		for(i = 2; i < 134; i++)
		{
			if (mode)
			{
				UARTprintf("%c",binToHex_high(CCD_Data.process_data[i])); //���ַ���ʽ���͸�4λ��Ӧ��16����
				UARTprintf("%c",binToHex_low(CCD_Data.process_data[i]));  //���ַ���ʽ���͵�?λ��Ӧ��16����
			}
			else
			{
				UARTprintf("%c",binToHex_high(CCD_Data.receive_data[i])); //���ַ���ʽ���͸�4λ��Ӧ��16����
				UARTprintf("%c",binToHex_low(CCD_Data.receive_data[i]));  //���ַ���ʽ���͵�?λ��Ӧ��16����
			}
		}
		UARTprintf("00");   //ͨ��Э��Ҫ��
		UARTprintf("#");    //ͨ��Э��Ҫ��
		CCD_Data.receive_flag = 0;
	}
}

/******************************************************************************
�������ܣ��������Ƶ�8λת��16����*
��ڲ�����u8
����  ֵ��char
*******************************************************************************/ 
char binToHex_low(unsigned char num)
{
	unsigned char low_four;
	low_four=num&0x0f;
	if(low_four==0)
		return('0');
	else if(low_four==1)
		return('1');
	else if(low_four==2)
		return('2');
	else if(low_four==3)
		return('3');
	else if(low_four==4)
		return('4');
	else if(low_four==5)
		return('5');
	else if(low_four==6)
		return('6');
	else if(low_four==7)
		return('7');
	else if(low_four==8)
		return('8');
	else if(low_four==9)
		return('9');
	else if(low_four==10)
		return('A');
	else if(low_four==11)
		return('B');
	else if(low_four==12)
		return('C');
	else if(low_four==13)
		return('D');
	else if(low_four==14)
		return('E');
	else if(low_four==15)
		return('F');
	else
		return 0;
}

/******************************************************************************
�������ܣ��������Ƹ�8λת��16����*
��ڲ�����u8
����  ֵ��char
*******************************************************************************/ 	
char binToHex_high(unsigned char num)
{
	unsigned char high_four;
	high_four=(num>>4)&0x0f;
	if(high_four==0)
		return('0');
	else if(high_four==1)
		return('1');
	else if(high_four==2)
		return('2');
	else if(high_four==3)
		return('3');
	else if(high_four==4)
		return('4');
	else if(high_four==5)
		return('5');
	else if(high_four==6)
		return('6');
	else if(high_four==7)
		return('7');
	else if(high_four==8)
		return('8');
	else if(high_four==9)
		return('9');
	else if(high_four==10)
		return('A');
	else if(high_four==11)
		return('B');
	else if(high_four==12)
		return('C');
	else if(high_four==13)
		return('D');
	else if(high_four==14)
		return('E');
	else if(high_four==15)
		return('F');
	else
		return 0;
}

/******************************************************************************
�������ܣ��õ���ǰ�������ֺڰ�ɫ����ֵ
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Get_CCD_Threshold_Value(void)
{
	unsigned char i;
	unsigned char min, max;
	
	Get_CCD_Value();
	SysCtlDelay(((SysCtlClockGet() / 3000) * 30) - 1);	//30ms��ʱ
	
	Get_CCD_Value();
	SysCtlDelay(((SysCtlClockGet() / 3000) * 30) - 1);	//30ms��ʱ
	
	max = CCD_Data.receive_data[0];
	min = CCD_Data.receive_data[0];
	for (i = 0; i < 128; i++)
	{
		if (CCD_Data.receive_data[i] > max)	max = CCD_Data.receive_data[i];
		else if (CCD_Data.receive_data[i] < min)	min = CCD_Data.receive_data[i];
	}
	CCD_Data.threshold_value = (max + min) / 2;
	//UARTprintf("CCD_Data.threshold_value = %d\r\n\r\n",CCD_Data.threshold_value);
}

/******************************************************************************
�������ܣ���ʹ������ccd�м�100λ�������жϣ��õ����ߵ�����غ��ұ���λ�ã����뻷����ֵ��
��ڲ�����threshold_value:������ֵ(���ֺڰױ���)
����  ֵ����
*******************************************************************************/ 
void Get_Edge_StaticThreshold(unsigned char threshold_value)
{
	unsigned char i;
	
	for (i = 15; i < 115; i++)
	{
		if (CCD_Data.receive_data[i] < threshold_value && CCD_Data.receive_data[i + 1] < threshold_value && CCD_Data.receive_data[i + 2] < threshold_value
			&& CCD_Data.receive_data[i + 3] < threshold_value && CCD_Data.receive_data[i + 4] < threshold_value)
		{
			CCD_Data.left_edge_location = i - 15;
			break;
		}
	}
	for (i = 114; i > 14; i--)
	{
		if (CCD_Data.receive_data[i] < threshold_value && CCD_Data.receive_data[i - 1] < threshold_value && CCD_Data.receive_data[i - 2] < threshold_value
			&& CCD_Data.receive_data[i - 3] < threshold_value && CCD_Data.receive_data[i - 4] < threshold_value)
		{
			CCD_Data.right_edge_location = i - 15;
			break;
		}
	}
}

/******************************************************************************
�������ܣ���ʹ������ccd�м�100λ�������жϣ��õ����ߵ�����غ��ұ���λ�ã���̬��ֵ��
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Get_Edge_DynamicThreshold(void)
{
	unsigned char i, j;
	unsigned char min, min_j, max, max_j;
	unsigned char last_data;
	unsigned char middle_value;
	
	//�Բ�õ����ݽ���һ�׵�ͨ�˲�(�ò�õ����߱�ƽ��,�����ʹ��)�����浽process_data�����У�֮�������Ĵ�����������process_data
	last_data = CCD_Data.receive_data[0];
	for (i = 0; i < 100; i ++)
	{
		CCD_Data.process_data[i] = CCD_Data.receive_data[i + 15] * 1.0f + last_data * 0.0f;
		last_data = CCD_Data.receive_data[i + 15];
	}
	
	//ȥ��5�����ֵ��5����Сֵ
	for (i = 0; i < 5; i++)
	{
		min = CCD_Data.process_data[0];
		max = CCD_Data.process_data[0];
		for (j = 0; j < 100; j++)
		{
			if (CCD_Data.process_data[j] > max)
			{
				max = CCD_Data.process_data[j];
				max_j = j;
			}
			else if (CCD_Data.process_data[j] < min)
			{
				min = CCD_Data.process_data[j];
				min_j = j;
			}
		}
		CCD_Data.process_data[max_j] = CCD_Data.process_data[max_j + 1];
		CCD_Data.process_data[min_j] = CCD_Data.process_data[min_j + 1];
	}
	
	//�����������ֵ����Сֵ����ֵ����Ϊ��ֵʹ��
	min = CCD_Data.process_data[0];
	max = CCD_Data.process_data[0];
	for (i = 0; i < 100; i++)
	{
		if (CCD_Data.process_data[i] > max)	max = CCD_Data.process_data[i];
		else if (CCD_Data.process_data[i] < min)	min = CCD_Data.process_data[i];
	}
	middle_value = (max + min) / 2;
	
	//���ݶ�ֵ��������ֵ�������ݴ洢��CCD_Data.binaryzation_data
	for (i = 0; i < 100; i++)
	{
		if (CCD_Data.process_data[i] > middle_value)	CCD_Data.binaryzation_data[i] = 1;
		else CCD_Data.binaryzation_data[i] = 0;
	}
	
	//ȥ������ë��
	for (i = 0; i < 95; i++)
	{
		if (CCD_Data.binaryzation_data[i] == 0)
		{
			if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i + 1] == 0 && CCD_Data.binaryzation_data[i + 2] == 0
				&& CCD_Data.binaryzation_data[i + 3] == 0);
			else
			{
				CCD_Data.binaryzation_data[i] = 1;
				CCD_Data.binaryzation_data[i + 1] = 1;
				CCD_Data.binaryzation_data[i + 2] = 1;
				CCD_Data.binaryzation_data[i + 3] = 1;
				i = i + 5;
			}
		}
	}
	
	//�õ���������غ��Ҳ�������
	for (i = 0; i < 100; i++)
	{
		if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i + 1] == 0 && CCD_Data.binaryzation_data[i + 2] == 0
			&& CCD_Data.binaryzation_data[i + 3] == 0 && CCD_Data.binaryzation_data[i + 4] == 0)
		{
			CCD_Data.left_edge_location = i;
			break;
		}
	}
	for (i = 99; i > 0; i--)
	{
		if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i - 1] == 0 && CCD_Data.binaryzation_data[i - 2] == 0
			&& CCD_Data.binaryzation_data[i - 3] == 0 && CCD_Data.binaryzation_data[i - 4] == 0)
		{
			CCD_Data.right_edge_location = i;
			break;
		}
	}
}
//����������ݽṹ�����
Line_TypeDef Line_Data;

/******************************************************************************
�������ܣ��������ݳ�ʼ������
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Line_Data_Init(void)
{
	Line_Data.last_line_location = 50;
	Line_Data.last_line_width = 20;
	Line_Data.last_line_right = 65;
	Line_Data.last_line_left = 35;
}

/******************************************************************************
�������ܣ������ش��������ɸ���ʵ���������е�������
��ڲ�����mode����ƫ��ֵ���󽻽���/�ҽ�����/����Ϊ��׼��0:���ߣ�1:���ߣ�2:����
����  ֵ����		�õ�����ƫ��ֵ
*******************************************************************************/ 
void Edge_Dispose(unsigned char mode)
{
	static unsigned char state = 0;
	//�����������������ز�ֵ����80����ʾ��⵽ʮ�ֻ�T��·��
	//if (CCD_Data.right_edge_location - CCD_Data.left_edge_location > 80)	Automobile_Data_Recorder.Car_State_Flag = Stop_And_Switch_State;
	//if (CCD_Data.right_edge_location > 85 && (CCD_Data.right_edge_location - CCD_Data.left_edge_location) > 40 && (CCD_Data.right_edge_location - CCD_Data.left_edge_location) < 80)	Automobile_Data_Recorder.Car_State_Flag = Stop_And_Switch_State;
	
	//������������ͻ�䣬�������ϴ�����λ�ã�������������ͻ�䣬�������ϴ�����λ�ã�	��ֹ�����ϵ����ո���
	if (Line_Data.last_line_left - CCD_Data.left_edge_location > 20)	Line_Data.line_left = Line_Data.last_line_left;
	else Line_Data.line_left = CCD_Data.left_edge_location;
	if (CCD_Data.right_edge_location - Line_Data.last_line_right > 20)	Line_Data.line_right = Line_Data.last_line_right;
	else Line_Data.line_right = CCD_Data.right_edge_location;
	
	//�ߵ�λ�����󽻽���/�ҽ�����/����Ϊ��׼���Ӽ�	����Ѳ�������һ���
	if (mode == 0)	Line_Data.line_location = (Line_Data.line_left + Line_Data.line_right) / 2;
	else if (mode == 1)	Line_Data.line_location = Line_Data.line_left + 10;
	else if (mode == 2)	Line_Data.line_location = Line_Data.line_right - 10;
	
	//�ߵ�λ��ͻ�䣬��������һ���ߵ�λ��	�˳�����ȷ������
	if (Get_Absolute_Value(Line_Data.line_location - Line_Data.last_line_location) > 30)	Line_Data.line_location = Line_Data.last_line_location;
	
	//�ߵĿ��ͻ�䣬��������һ���ߵ�λ��	�˳�����ȷ������
	Line_Data.line_width = Line_Data.line_right - Line_Data.line_left;
	if (Line_Data.line_width - Line_Data.last_line_width > 20)	Line_Data.line_location = Line_Data.last_line_location;
	
	//������������������ͬʱ�������߿����ĳֵ����ʶ��Ϊ��⵽�յ㣬LED�Ƹı�״̬
	if (Line_Data.last_line_left - CCD_Data.left_edge_location > 12 && CCD_Data.right_edge_location - Line_Data.last_line_right > 12 && Line_Data.line_width > 20)
	{
		state = !state;
		Grren_Light_Control(state);
	}
	
	//�ϴ����ݸ�ֵ
	Line_Data.last_line_location = Line_Data.line_location;
	Line_Data.last_line_width = Line_Data.line_width;
	Line_Data.last_line_left = Line_Data.line_left;
	Line_Data.last_line_right = Line_Data.line_right;
	
	//�ߵ�ƫ��ֵ����
	Line_Data.line_offset = Line_Data.line_location - 50;
}

//����CCD��pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef CCD_Pid;
p_i_d_Value_TypeDef PID_Value_CCD[3] = {{45.0, 0.0, 0.0},	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶȣ�
										{70.0, 0.0, 10.0},
										{150.0, 0.0, 0.0}};
/******************************************************************************
�������ܣ�����CCD��pidѭ����Ѳ���ٶȸı�ʱ��p��i��d������ֵҲҪ�ı䣩
��ڲ�����pwm:pwm�Ƚ�ֵ�����Ƶ��Ѳ���ٶ�
����  ֵ���ޣ��ı�����������pwm�Ƚ�ֵ
*******************************************************************************/ 
void CCD_Track_Pid(uint16_t pwm)
{
	//Get_Edge_DynamicThreshold();
	Get_Edge_StaticThreshold(CCD_Data.threshold_value);
	Edge_Dispose(2);
	
	Pid_control(&CCD_Pid, Line_Data.line_offset, 0);
	Pid_OutLimit(&CCD_Pid, 5000);
	
	L1_Motor.motor_pwm = pwm - CCD_Pid.ActualValue;
	R1_Motor.motor_pwm = pwm + CCD_Pid.ActualValue;
//	Motor_InLimit( &L1_Motor, 9000);
//	Motor_InLimit( &R1_Motor, 9000);
	
//	Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
//	Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
}

//����CCD�����򻷵�pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef CCD_Direction_Pid;
p_i_d_Value_TypeDef PID_Value_CCD_Direction[3] = {{2.0, 0.0, 0.18},	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶȣ�
												  {2.3, 0.0, 0.5},
												  {0.0, 0.0, 0.0}};
/******************************************************************************
�������ܣ�����CCD��ѭ�����򻷣�Ѳ���ٶȸı�ʱ��p��i��d������ֵҲҪ�ı䣩
��ڲ�����speed:Ѳ���ٶ�
����  ֵ���ޣ��ı����ٶȻ�Ŀ��ֵ
*******************************************************************************/ 
void CCD_Track_Direction_Control(float speed)
{
	Get_CCD_Value();
	Get_Edge_DynamicThreshold();
	//Get_Edge_StaticThreshold(CCD_Data.threshold_value);
	Edge_Dispose(1);

	Pid_control(&CCD_Direction_Pid, Line_Data.line_offset, 0);
	Pid_OutLimit(&CCD_Direction_Pid, 150);
	
	L1_Speed_Pid.SetPoint = speed - CCD_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed + CCD_Direction_Pid.ActualValue;
}

