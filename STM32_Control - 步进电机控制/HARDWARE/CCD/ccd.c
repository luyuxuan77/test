#include "ccd.h"
#include "adc.h"
#include "pid.h"
#include "led.h"
#include "delay.h"
#include "debug.h"
#include "usart.h"
#include "motor.h"
#include "mymath.h"
#include "control.h"

/******************************************************************************
�������ܣ�����CCDģ���ʼ����ʹ��ADC3
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void CCD_Init(void)
{
	//�ȳ�ʼ��IO��
	RCC->AHB1ENR |= 1<<5;  // ʹ��PORTFʱ��
	GPIO_Set(GPIOF, PIN14|PIN15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_2M, GPIO_PUPD_PD);  // PF14,PF15,�������������
	GPIO_Set(GPIOF, PIN5, GPIO_MODE_AIN, 0, 0, GPIO_PUPD_PD);  // PF5,ģ������,����
	Adc3_Init();
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
	u8 i;
	
	CCD_CLK = 0;
	CCD_SI = 0;
	Dly_us();
	Dly_us();
	
	CCD_CLK = 0;
	CCD_SI = 1;
	Dly_us();
	Dly_us();

	CCD_CLK = 1;
	Dly_us();
	Dly_us();
	
	CCD_SI = 0;
	Dly_us();
	Dly_us();
	
	for (i = 0; i < 128; i++)
	{
		CCD_CLK = 0;
		Dly_us();
		
		CCD_Data.receive_data[i] = (Get_Adc3(CCD_AO_CH)) >> 4;
		
		CCD_CLK = 1;
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
	for(ii = 0; ii < 10; ii++);  
}

/******************************************************************************
�������ܣ����������ֵ������
��ڲ�����mdoe:������Ϣ���0:����ADC�ɼ���ԭʼֵ��1:�������ݶ�ֵ���������ֵ
����  ֵ����
*******************************************************************************/ 
void Send_To_Serial_Port(u8 mode)
{
	u8 i;
	if (CCD_Data.receive_flag == 1)
	{
		printf("{");
		for(i = 0; i < 128; i++)
		{
			if (mode) printf("%d,",CCD_Data.binaryzation_data[i]);
			else printf("%d,",CCD_Data.receive_data[i]);
		}
		printf("}");
		printf("\r\n");printf("\r\n");printf("\r\n");
		
		CCD_Data.receive_flag = 0;
	}
}

/******************************************************************************
�������ܣ��������͵���Ϣͨ�����ڷ�������λ��*
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Send_To_Debug(u8 mode)
{ 
	int i;
	if (CCD_Data.receive_flag == 1)
	{
		printf("*");
		printf("LD");
		for(i = 2; i < 134; i++)
		{
			if (mode)
			{
				printf("%c",binToHex_high(CCD_Data.process_data[i]));  // ���ַ���ʽ���͸�4λ��Ӧ��16����
				printf("%c",binToHex_low(CCD_Data.process_data[i]));  // ���ַ���ʽ���͵�?λ��Ӧ��16����
			}
			else
			{
				printf("%c",binToHex_high(CCD_Data.receive_data[i]));  // ���ַ���ʽ���͸�4λ��Ӧ��16����
				printf("%c",binToHex_low(CCD_Data.receive_data[i]));  // ���ַ���ʽ���͵�?λ��Ӧ��16����
			}
		}
		printf("00");  // ͨ��Э��Ҫ��
		printf("#");  // ͨ��Э��Ҫ��
		CCD_Data.receive_flag = 0;
	}
}

/******************************************************************************
�������ܣ��������Ƶ�8λת��16����*
��ڲ�����u8
����  ֵ��char
*******************************************************************************/ 	 	 
char binToHex_low(u8 num)
{
	u8 low_four;
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
char binToHex_high(u8 num)
{
	u8 high_four;
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
	u8 i;
	u8 min, max;
	
	Get_CCD_Value();
	delay_ms(50);
	Get_CCD_Value();
	delay_ms(50);
	
	max = CCD_Data.receive_data[0];
	min = CCD_Data.receive_data[0];
	for (i = 0; i < 128; i++)
	{
		if (CCD_Data.receive_data[i] > max)  max = CCD_Data.receive_data[i];
		else if (CCD_Data.receive_data[i] < min)  min = CCD_Data.receive_data[i];
	}
	CCD_Data.threshold_value = (max + min) / 2;
	//printf("CCD_Data.threshold_value = %d\r\n\r\n",CCD_Data.threshold_value);
}

/******************************************************************************
�������ܣ��õ����ߵ�����غ��ұ���λ�ã����뻷����ֵ��
��ڲ�����threshold_value:������ֵ(���ֺڰױ���)��mode:0������ߵ��ߵ�ƫ�1�����ұߵ��ߵ�ƫ��
����  ֵ����
*******************************************************************************/ 
void Get_Edge_StaticThreshold(u8 threshold_value)
{
	u8 i;
	
	for (i = 0; i < 128; i++)
	{
		if (CCD_Data.receive_data[i] < threshold_value && CCD_Data.receive_data[i + 1] 
			< threshold_value && CCD_Data.receive_data[i + 2] < threshold_value
			&& CCD_Data.receive_data[i + 3] < threshold_value && CCD_Data.receive_data[i + 4] < threshold_value)
		{
			CCD_Data.left_edge_location = i;
			break;
		}
	}
	for (i = 127; i > 0; i--)
	{
		if (CCD_Data.receive_data[i] < threshold_value && CCD_Data.receive_data[i - 1] 
			< threshold_value && CCD_Data.receive_data[i - 2] < threshold_value
			&& CCD_Data.receive_data[i - 3] < threshold_value && CCD_Data.receive_data[i - 4] < threshold_value)
		{
			CCD_Data.right_edge_location = i;
			break;
		}
	}
}

/******************************************************************************
�������ܣ��õ����ߵ�����غ��ұ���λ�ã���̬��ֵ��
��ڲ�������
����  ֵ����
*******************************************************************************/ 
void Get_Edge_DynamicThreshold(void)
{
	u8 i, j;
	u8 min, min_j, max, max_j;
	u8 last_data;
	u8 middle_value;
	
	//�Բ�õ����ݽ���һ�׵�ͨ�˲�(�ò�õ����߱�ƽ��,�����ʹ��)�����浽process_data�����У�֮�������Ĵ�����������process_data
	last_data = CCD_Data.receive_data[0];
	for (i = 0; i < 128; i ++)
	{
		CCD_Data.process_data[i] = CCD_Data.receive_data[i] * 1.0f + last_data * 0.0f;
		last_data = CCD_Data.receive_data[i];
	}
	
	//ȥ��5�����ֵ��5����Сֵ
	for (i = 0; i < 5; i++)
	{
		min = CCD_Data.process_data[0];
		max = CCD_Data.process_data[0];
		for (j = 0; j < 128; j++)
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
	for (i = 0; i < 128; i++)
	{
		if (CCD_Data.process_data[i] > max)	max = CCD_Data.process_data[i];
		else if (CCD_Data.process_data[i] < min)	min = CCD_Data.process_data[i];
	}
	middle_value = (max + min) / 2;
	
	//���ݶ�ֵ��������ֵ�������ݴ洢��CCD_Data.binaryzation_data
	for (i = 0; i < 128; i++)
	{
		if (CCD_Data.process_data[i] > middle_value)	CCD_Data.binaryzation_data[i] = 1;
		else CCD_Data.binaryzation_data[i] = 0;
	}
	
	//ȥ������ë��
	for (i = 0; i < 123; i++)
	{
		if (CCD_Data.binaryzation_data[i] == 0)
		{
			if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i + 1] 
				== 0 && CCD_Data.binaryzation_data[i + 2] == 0
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
	for (i = 0; i < 128; i++)
	{
		if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i + 1] 
			== 0 && CCD_Data.binaryzation_data[i + 2] == 0
			&& CCD_Data.binaryzation_data[i + 3] == 0 && CCD_Data.binaryzation_data[i + 4] == 0)
		{
			CCD_Data.left_edge_location = i;
			break;
		}
	}
	for (i = 127; i > 0; i--)
	{
		if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i - 1] 
			== 0 && CCD_Data.binaryzation_data[i - 2] == 0
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
�������ܣ������ش��������ɸ���ʵ���������е�������
��ڲ�����mode����ƫ��ֵ���󽻽���/�ҽ�����/����Ϊ��׼��0:���ߣ�1:���ߣ�2:����
����  ֵ����		�õ�����ƫ��ֵ
*******************************************************************************/ 
void Edge_Dispose(u8 mode)
{
	static u8 last_line_location = 64;
	static u8 last_line_width = 20;
	static u8 last_right = 80, last_left = 50;

	//�����������������ز�ֵ����100����ʾ��⵽ʮ�ֻ�T��·��
	//if (CCD_Data.right_edge_location - CCD_Data.left_edge_location > 100)	Automobile_Data_Recorder.Car_State_Flag = Stop_And_Switch_State;
	
	//������������ͻ�䣬�������ϴ�����λ�ã�������������ͻ�䣬�������ϴ�����λ�ã�	��ֹ�����ϵ����ո���
	if (last_left - CCD_Data.left_edge_location > 25)	Line_Data.line_left = last_left;
	else Line_Data.line_left = CCD_Data.left_edge_location;
	if (CCD_Data.right_edge_location - last_right > 25)	Line_Data.line_right = last_right;
	else Line_Data.line_right = CCD_Data.right_edge_location;
	
	//�ߵ�λ�����󽻽���/�ҽ�����/����Ϊ��׼���Ӽ�	����Ѳ�������һ���
	if (mode == 0)	Line_Data.line_location = (Line_Data.line_left + Line_Data.line_right) / 2;
	else if (mode == 1)	Line_Data.line_location = Line_Data.line_left + 10;
	else if (mode == 2)	Line_Data.line_location = Line_Data.line_right - 10;
	
	//�ߵ�λ��ͻ�䣬��������һ���ߵ�λ��	�˳�����ȷ������
	if ((Line_Data.line_location - last_line_location) > 40 || (Line_Data.line_location - last_line_location) < -40)	Line_Data.line_location = last_line_location;
	
	//�ߵĿ��ͻ�䣬��������һ���ߵ�λ��	�˳�����ȷ������
	Line_Data.line_width = Line_Data.line_right - Line_Data.line_left;
	if (Line_Data.line_width - last_line_width > 20)	Line_Data.line_location = last_line_location;
	
	//������������������ͬʱ�������߿����ĳֵ����ʶ��Ϊ��⵽�յ㣬LED�Ƹı�״̬
	if (last_left - CCD_Data.left_edge_location > 12 && CCD_Data.right_edge_location - last_right 
		> 12 && Line_Data.line_width > 20)	LED1 = !LED1;
	
	//�ϴ����ݸ�ֵ
	last_line_location = Line_Data.line_location;
	last_line_width = Line_Data.line_width;
	last_left = Line_Data.line_left;
	last_right = Line_Data.line_right;
	
	//�ߵ�ƫ��ֵ����
	Line_Data.line_offset = Line_Data.line_location - 64;
}

//����CCD��pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef CCD_Pid;
p_i_d_Value_TypeDef PID_Value_CCD[3] = {{ 70.0, 0.0, 0.0 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶȣ�
										{ 100.0, 0.0, 0.0 },
										{ 200.0, 0.0, 0.0 }};
/******************************************************************************
�������ܣ�����CCD��pidѭ����Ѳ���ٶȸı�ʱ��p��i��d������ֵҲҪ�ı䣩
��ڲ�����pwm:pwm�Ƚ�ֵ�����Ƶ��Ѳ���ٶ�
����  ֵ���ޣ��ı�����������pwm�Ƚ�ֵ
*******************************************************************************/ 
void CCD_Track_Pid(u16 pwm)
{
	Get_CCD_Value();
	Get_Edge_DynamicThreshold();
	//Get_Edge_StaticThreshold(CCD_Data.threshold_value);
	Edge_Dispose(1);
	
	Pid_control(&CCD_Pid, Line_Data.line_offset, 0);
	Pid_OutLimit(&CCD_Pid, 7000);
	
	L1_Motor.motor_pwm = pwm - CCD_Pid.ActualValue;
	R1_Motor.motor_pwm = pwm + CCD_Pid.ActualValue;
	Motor_InLimit( &L1_Motor, 15000);
	Motor_InLimit( &R1_Motor, 15000);
	
	Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
	Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
}

//����CCD�����򻷵�pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef CCD_Direction_Pid;
p_i_d_Value_TypeDef PID_Value_CCD_Direction[3] = {{2.0, 0.0, 0.18},  // Ԥ������pid���������ڲ�ͬ��Ѳ���ٶȣ�
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
