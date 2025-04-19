#include "track.h"
#include "motor.h"
#include "uart1.h"
#include "uart0.h"
#include "pid.h"

float sensor_weights[NUM_SENSORS] = {3.4, 2.5, 1, 0, -1, -2.5, -3.4};	//�Ƶ�λȨ����
float sensor_weights_inner[NUM_SENSORS] = {2.3, 1.8, 1, 0, -1, -8.6, -12};	//�Ƶ�λȨ����

void Line7_Init(void)
{
    // ���� GPIO ����
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // ���� PD2 �� PD3 Ϊ���루�Ƴ� PD0 �� PD1��
    GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    
    // ���� PB2 �� PB3 Ϊ���루���ֲ��䣩
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    
    // ���� PE2��PE4 �� PE5 Ϊ���루���� PE4 �� PE5��
    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
}

Grayscale_TypeDef Grayscale_Data = {1, 0, 0};
// ���忨�����˲�������
KalmanFilter grayscale_kalman = {0, 1, 0};

void KalmanFilter_Init(KalmanFilter* kf) {
    kf->X = 0;  // ��ʼ����ֵ
    kf->P = 1;  // ��ʼ�������
    kf->K = 0;  // ��ʼ����������
}

float KalmanFilter_Update(KalmanFilter* kf, float measurement) {
    // Ԥ�����
    kf->P += 0.1;  // ����������� Q = 0.1

    // ��������
    kf->K = kf->P / (kf->P + 0.5);  // ����������� R = 0.5
    kf->X += kf->K * (measurement - kf->X);
    kf->P *= (1 - kf->K);

    return kf->X;
}
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
    Grayscale_Data.grayscale_offset = KalmanFilter_Update(&grayscale_kalman, position_error);
//	printf("                                            %d\r\n",Grayscale_Data.grayscale_offset);
	return Grayscale_Data.grayscale_offset;
}

int Get_Grayscale_Offset1(void)
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
			position_error += sensor_weights_inner[i];
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
    Grayscale_Data.grayscale_offset = KalmanFilter_Update(&grayscale_kalman, position_error);
//	printf("                                            %d\r\n",Grayscale_Data.grayscale_offset);
	return Grayscale_Data.grayscale_offset;
}

//����Ҷȴ���������pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef Grayscale_Direction_Pid;/*!pid�㷨!���õ��Ĳ�������*/
p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3] = {{24, 0.0, 2.5 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�
														{ 34.5, 0.0, 2.5},/*���������֣�΢��3ֵ��С�ṹ��*/
														{ 0.0, 0.0, 0.0 }};
/******************************************************************************
�������ܣ��Ҷȴ�������ѭ�����򻷣�Ѳ���ٶȸı�ʱ��p��i��d������ֵҲҪ�ı䣩
��ڲ�����speed:Ѳ���ٶ�
����  ֵ���ޣ��ı����ٶȻ�Ŀ��ֵ
*******************************************************************************/ 
void Track_Direction_Control(float speed)
{
//	printf("2");
	Get_Grayscale_Offset();
//	printf("%d\r\n",Grayscale_Data.grayscale_offset);
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
//	printf("ActualValue=%.2f\r\n",Grayscale_Direction_Pid.ActualValue);
	Pid_OutLimit(&Grayscale_Direction_Pid, 40);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue;
//	L1_Speed_Pid.SetPoint = 250;
//	R1_Speed_Pid.SetPoint = 250;
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}







void Track_Direction_Control1(float speed)
{
//	printf("2");
	Get_Grayscale_Offset();
//	printf("%d\r\n",Grayscale_Data.grayscale_offset);
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
//	printf("ActualValue=%.2f\r\n",Grayscale_Direction_Pid.ActualValue);
	Pid_OutLimit(&Grayscale_Direction_Pid, 50);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue*1.22;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue*1.22 ;
//	L1_Speed_Pid.SetPoint = 250;
//	R1_Speed_Pid.SetPoint = 250;
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}
void Track_Direction_Control2(float speed)
{
//	printf("2");
	Get_Grayscale_Offset1();
//	printf("%d\r\n",Grayscale_Data.grayscale_offset);
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
//	printf("ActualValue=%.2f\r\n",Grayscale_Direction_Pid.ActualValue);
	Pid_OutLimit(&Grayscale_Direction_Pid, 50);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue*1.22;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue*1.22 ;
//	L1_Speed_Pid.SetPoint = 250;
//	R1_Speed_Pid.SetPoint = 250;
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
//void Track_Angle_Distance_Control(void)
//{
//	Get_Angle_Offset();
//	Get_Distance_Offset();

//	Pid_control(&UWB_Angle_Pid, UWB_Current_Angle, 0);
//	Pid_control(&UWB_Distance_Pid, UWB_Current_Distance, 0);
//	Pid_OutLimit(&UWB_Angle_Pid, 50);
//	Pid_OutLimit(&UWB_Distance_Pid, 50);
////	printf("Dis: %f	   disc: %f   Ang: %f   angc: %f\r\n",UWB_Distance_Pid.ActualValue,UWB_Current_Distance,UWB_Angle_Pid.ActualValue,UWB_Current_Angle);
////	L1_Speed_Pid.SetPoint = UWB_Distance_Pid.ActualValue - UWB_Angle_Pid.ActualValue;
////	R1_Speed_Pid.SetPoint = UWB_Distance_Pid.ActualValue + UWB_Angle_Pid.ActualValue;

////	motor_4(-UWB_Distance_Pid.ActualValue - UWB_Angle_Pid.ActualValue,    -UWB_Distance_Pid.ActualValue + UWB_Angle_Pid.ActualValue,0,0);
//}

