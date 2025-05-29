#include "track.h"
#include "motor.h"
#include "usart.h"
#include "pid.h"

/**************************************************************************
�������ܣ���·�Ҷȳ�ʼ��
��ڲ�������
����  ֵ����		//��7·�ҶȽ�����ӿڰ�8·ѭ���ӿڣ���Ϊ��·ѭ���ӿ��밴����ͻ
**************************************************************************/
void Line7_Init(void)
{
	RCC->AHB1ENR |= 1<<6;
	GPIO_Set(GPIOG, PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6, GPIO_MODE_IN, 0, 0, GPIO_PUPD_PD);
}

/**************************************************************************
�������ܣ������伤�⴫����ѭ��ģ���ʼ��
��ڲ�������
����  ֵ����	
**************************************************************************/
void Laser_Sensor_Init(void)
{
	RCC->AHB1ENR |= 1<<6;
	GPIO_Set(GPIOG, PIN7, GPIO_MODE_IN, 0, 0, GPIO_PUPD_PD);
}


//����ҶȲ����ṹ�����
Grayscale_TypeDef Grayscale_Data = { 1, 0, 0 };

/**************************************************************************
�������ܣ���·�Ҷȿ���ѭ������ͬ�Ҷ�ֵ��Ӧ��ͬ�ĵ���ٶ�
��ڲ�����Modeģʽ���ƣ����١����ٵȵȣ����Զ���
����  ֵ����	
**************************************************************************/
void Track_Line7(u8 Mode)
{
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	if (Mode == 1)
	{
		switch (Grayscale_Data.grayscale_digital)
		{
			case 0x88:	motor_4( 30, 30, 30, 30);	break;  // 1 0001000
			
			case 0x8C:	motor_4( 30, 30, 20, 20 );	break;  // 1 0001100
			case 0x84:	motor_4( 20, 20, -10, -10 );break;  // 1 0000100
			case 0x82:	motor_4( 30, 30, 20, 20 );	break;  // 1 0000010
			
			case 0x98:	motor_4( 20, 20, 30, 30 );	break;  // 1 0011000
			case 0x90:	motor_4( -10, -10, 20, 20 );break;  // 1 0010000
			case 0xA0:	motor_4( 20, 20, 30, 30 );	break;  // 1 0100000
			default:	break;
		}
	}
	else if (Mode == 2)
	{
		switch (Grayscale_Data.grayscale_digital)
		{
			case 0x88:	motor_4( 50, 50, 50, 50 );	break;  // 1 0001000
			
			case 0x8C:	motor_4( 50, 50, 35, 35 );	break;  // 1 0001100
			case 0x84:	motor_4( 35, 35, -10, -10 );break;  // 1 0000100
			
			case 0x98:	motor_4( 35, 35, 50, 50 );	break;  // 1 0011000
			case 0x90:	motor_4( -10, -10, 35, 35 );break;  // 1 0010000
			default:	break;
		}
	}

}

/**************************************************************************
�������ܣ���·�Ҷ�ʶ��·������ͬ��·����Ӧ��ͬ���߼�
��ڲ�������
����  ֵ����	
**************************************************************************/
void Road_Condition_Recognition(void)
{
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	switch (Grayscale_Data.grayscale_digital)
	{
		case 0xFF:	;	break;  // 1 1111111
			
		case 0x8F:	;	break;  // 1 0001111
		case 0x87:	;	break;  // 1 0000111
	
		case 0xF8:	;	break;  // 1 1111000
		case 0xF0:	;	break;  // 1 1110000
		default:	break;
	}
}

/**************************************************************************
�������ܣ��õ���·�Ҷ�ƫ����(ģ��)
��ڲ�������
����  ֵ����	��ģ������ֵ��Grayscale_Data.grayscale_offset
**************************************************************************/
int Get_Grayscale_Offset(void)
{
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	Grayscale_Data.grayscale_state = 1;
	switch (Grayscale_Data.grayscale_digital | 0xC1)
	{
		case 0xC9:	Grayscale_Data.grayscale_offset = 0;	break;  // 1 1001001
			
		case 0xCD:	Grayscale_Data.grayscale_offset = 60;	break;  // 1 1001101
		case 0xC5:	Grayscale_Data.grayscale_offset = 120;	break;  // 1 1000101
		case 0xC7:	Grayscale_Data.grayscale_offset = 180;	break;  // 1 1000111
		case 0xC3:	Grayscale_Data.grayscale_offset = 240;	break;  // 1 1000011
		
		case 0xD9:	Grayscale_Data.grayscale_offset = -60;	break;  // 1 1011001
		case 0xD1:	Grayscale_Data.grayscale_offset = -120;	break;  // 1 1010001
		case 0xF1:	Grayscale_Data.grayscale_offset = -180;	break;  // 1 1110001
		case 0xE1:	Grayscale_Data.grayscale_offset = -240;	break;  // 1 1100001
		default:	Grayscale_Data.grayscale_state = 0; break;
	}
	return Grayscale_Data.grayscale_offset;
}
//����Ҷȴ�������pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef Grayscale_Pid;
p_i_d_Value_TypeDef PID_Value_Grayscale[3] = {{ 18.0, 0.0, 5.0 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶȣ�
											  { 0.0, 0.0, 0.0 },
											  { 55.0, 0.0, 10.0 }};

/******************************************************************************
�������ܣ��Ҷȴ�������pidѭ����Ѳ���ٶȸı�ʱ��p��i��d������ֵҲҪ�ı䣩
��ڲ�����pwm:pwm�Ƚ�ֵ�����Ƶ��Ѳ���ٶ�
����  ֵ���ޣ��ı�����������pwm�Ƚ�ֵ
*******************************************************************************/ 
void Track_Pid(u16 pwm)
{
	Get_Grayscale_Offset();
	Pid_control(&Grayscale_Pid, Grayscale_Data.grayscale_offset, 0);

	Pid_OutLimit(&Grayscale_Pid, 7000);
	
	L1_Motor.motor_pwm = pwm + Grayscale_Pid.ActualValue;
	R1_Motor.motor_pwm = pwm - Grayscale_Pid.ActualValue;
	Motor_InLimit(&L1_Motor, 15000);
	Motor_InLimit(&R1_Motor, 15000);
	
	Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
	Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
}

//����Ҷȴ���������pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef Grayscale_Direction_Pid;
p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3] = {{ 0.3, 0.0, 0.1 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶȣ�
														{ 0.0, 0.0, 0.0 },
														{ 0.0, 0.0, 0.0 }};

/******************************************************************************
�������ܣ��Ҷȴ�������ѭ�����򻷣�Ѳ���ٶȸı�ʱ��p��i��d������ֵҲҪ�ı䣩
��ڲ�����speed:Ѳ���ٶ�
����  ֵ���ޣ��ı����ٶȻ�Ŀ��ֵ
*******************************************************************************/ 
void Track_Direction_Control(float speed)
{
	Get_Grayscale_Offset();
	
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 100);
	
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue;
}
