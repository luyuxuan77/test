#include "track.h"
#include "motor.h"
#include "uart1.h"
#include "uart0.h"

float sensor_weights[NUM_SENSORS] = {-3, -2, -1, 0, 1, 2, 3};	//�Ƶ�λȨ����

void Line7_Init(void)
{
	//���� GPIO ����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE)	;

	GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 , GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
	
	GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3 , GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
	
	GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_2 , GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
}

Grayscale_TypeDef Grayscale_Data = {1, 0, 0};

/**************************************************************************
�������ܣ��õ���·�Ҷ�ƫ����(ģ��)
��ڲ�������
����  ֵ����ģ������ֵ��Grayscale_Data.grayscale_offset
**************************************************************************/
int Get_Grayscale_Offset(void)
{
	float position_error = 0;
	int active_sensors = 0;
	int i = 0;
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	Grayscale_Data.grayscale_state = 1;
	for (i = 0; i < NUM_SENSORS; i++)
	{
		if(Grayscale_Data.grayscale_digital & (1 << i))
		{
			position_error += sensor_weights[i];
			active_sensors++;
		}
	}
	if(active_sensors != 0)
	{
		position_error /= active_sensors;
	}
	else
	{
		Grayscale_Data.grayscale_state = 0;
		return Grayscale_Data.grayscale_offset;
	}
	Grayscale_Data.grayscale_offset = position_error;
//	printf("                                            %d\r\n",Grayscale_Data.grayscale_offset);
	return Grayscale_Data.grayscale_offset;
}

//����Ҷȴ���������pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef Grayscale_Direction_Pid;/*!pid�㷨!���õ��Ĳ�������*/
p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3] = {{ 300, 0.0, 0 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�
														{ 0.0, 0.0, 0.0 },/*���������֣�΢��3ֵ��С�ṹ��*/
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
	Pid_OutLimit(&Grayscale_Direction_Pid, 300);


	L1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue;
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}









UWB_TypeDef UWB_Data = {1, 0};
PID_TypeDef UWB_Angle_Pid;/*!pid�㷨!���õ��Ĳ�������*/
PID_TypeDef UWB_Distance_Pid;/*!pid�㷨!���õ��Ĳ�������*/

//����Ҷȴ���������pid�����ṹ�������p��i��d��ֵ�ṹ���������
p_i_d_Value_TypeDef PID_Value_UWB_Angle[1] = {{ 3, 0.0, 0 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�/*���������֣�΢��3ֵ��С�ṹ��*/
											 };
p_i_d_Value_TypeDef PID_Value_UWB_Distance[1] = {{ 1, 0.0, 0 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�/*���������֣�΢��3ֵ��С�ṹ��*/
											 };
float UWB_Current_Angle = 60;
float UWB_Current_Distance = 100;
/**************************************************************************
�������ܣ��õ�UWB�ǶȲ�
��ڲ�������
����  ֵ��UWB_Data.angle_offset(float��)��ǩ���ҵĽǶȲ�
**************************************************************************/
float Get_Angle_Offset(void)
{
	UWB_Data.angle_offset = UWB_Angle_Pid.SetPoint - UWB_Current_Angle;
	return UWB_Data.angle_offset;
}

/**************************************************************************
�������ܣ��õ�UWB�����
��ڲ�������
����  ֵ��UWB_Data.angle_offset(float��)��ǩ���ҵľ����
**************************************************************************/
float Get_Distance_Offset(void)
{
	UWB_Data.distance_offset = UWB_Current_Distance - UWB_Distance_Pid.SetPoint;
	return UWB_Data.distance_offset;
}

/******************************************************************************
�������ܣ�UWB��������׷�ٹ���
��ڲ�����
����  ֵ��
*******************************************************************************/ 
void Track_Angle_Distance_Control(void)
{
	Get_Angle_Offset();
	Get_Distance_Offset();

	Pid_control(&UWB_Angle_Pid, UWB_Current_Angle, 0);
	Pid_control(&UWB_Distance_Pid, UWB_Current_Distance, 0);
	Pid_OutLimit(&UWB_Angle_Pid, 50);
	Pid_OutLimit(&UWB_Distance_Pid, 50);
//	printf("Dis: %f	   disc: %f   Ang: %f   angc: %f\r\n",UWB_Distance_Pid.ActualValue,UWB_Current_Distance,UWB_Angle_Pid.ActualValue,UWB_Current_Angle);
//	L1_Speed_Pid.SetPoint = UWB_Distance_Pid.ActualValue - UWB_Angle_Pid.ActualValue;
//	R1_Speed_Pid.SetPoint = UWB_Distance_Pid.ActualValue + UWB_Angle_Pid.ActualValue;

//	motor_4(-UWB_Distance_Pid.ActualValue - UWB_Angle_Pid.ActualValue,    -UWB_Distance_Pid.ActualValue + UWB_Angle_Pid.ActualValue,0,0);
}

