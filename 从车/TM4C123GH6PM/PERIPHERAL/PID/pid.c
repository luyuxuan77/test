#include "pid.h"

//����λPID�����ṹ������
//���������ԭ��:Ϊ�˺ÿ���Գ�
PID_TypeDef Position_position_Pid[4];	//λ�ÿ���λ�û�(�⻷)4�����ݣ���Ӧ4�����
PID_TypeDef	Position_speed_Pid[4];		//λ�ÿ����ٶȻ�(�ڻ�)4�����ݣ���Ӧ4�����
PID_TypeDef Speed_Pid[4];				//�ٶȿ���4�����ݣ���Ӧ4�����

//����λ�ÿ���λ�û�p��i��d��ֵ�ṹ���������
//PID_Value_Position[0]��0�ŵ��λ�ÿ���λ�û�p��i��d��ֵ����
//PID_Value_Position[1]��1�ŵ��λ�ÿ���λ�û�p��i��d��ֵ����
//PID_Value_Position[2]��2�ŵ��λ�ÿ���λ�û�p��i��d��ֵ����
//PID_Value_Position[3]��3�ŵ��λ�ÿ���λ�û�p��i��d��ֵ����
//��Ӳ�������Ӧ��p��i��d��ֵ��С�����йأ��˴�����ýṹ�����������ڵ��p��i��d��ֵ����
#if	POSITION_Position_PID_TYPE == 0	/*λ��ʽP��I��D������С*/
p_i_d_Value_TypeDef PID_Value_Position_position[4] = {{ 0.48 , 0.0 , 0.1 },
													  { 0.48 , 0.0 , 0.1 },
													  { 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 }};	//λ�ÿ���λ�û�pid������С
#else								/*����ʽP��I��D������С*/
p_i_d_Value_TypeDef PID_Value_Position_position[4] = {{ 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 },
													  { 0.0 , 0.0 , 0.0 }};	//λ�ÿ���λ�û���pid������С
#endif
//����λ�ÿ����ٶȻ�p��i��d��ֵ�ṹ���������
//PID_Value_Position_speed[0]��0�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ����
//PID_Value_Position_speed[1]��1�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ����
//PID_Value_Position_speed[2]��2�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ����
//PID_Value_Position_speed[3]��3�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ����
//��Ӳ�������Ӧ��p��i��d��ֵ��С�����йأ��˴�����ýṹ�����������ڵ��p��i��d��ֵ����
#if	POSITION_Speed_PID_TYPE == 0	/*λ��ʽP��I��D������С*/
p_i_d_Value_TypeDef PID_Value_Position_speed[4] = {{ 17.0725 , 10.0081 , 0.0 },
												   { 17.0725 , 10.0081 , 0.0 },
												   { 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 }};	//λ�ÿ����ٶȻ�pid������С
#else								/*����ʽP��I��D������С*/
p_i_d_Value_TypeDef PID_Value_Position_speed[4] = {{ 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 },
												   { 0.0 , 0.0 , 0.0 }};	//λ�ÿ����ٶȻ�pid������С
#endif


//�����ٶȿ���p��i��d��ֵ�ṹ���������
//PID_Value_Speed[0]��0�ŵ���ٶȿ���p��i��d��ֵ����
//PID_Value_Speed[1]��1�ŵ���ٶȿ���p��i��d��ֵ����
//PID_Value_Speed[2]��2�ŵ���ٶȿ���p��i��d��ֵ����
//PID_Value_Speed[3]��3�ŵ���ٶȿ���p��i��d��ֵ����
//��Ӳ�������Ӧ��p��i��d��ֵ��С�����йأ��˴�����ýṹ�����������ڵ��p��i��d��ֵ����
#if	SPEED_PID_TYPE == 0		/*λ��ʽP��I��D������С*/
p_i_d_Value_TypeDef PID_Value_Speed[4] = {{ 190 , 7.6 , 0.78 },
										  { 190 , 7.6 , 0.78 },
										  { 0 , 0 , 0 },
										  { 0 , 0 , 0 }}; 	//�ٶȿ���pid������С
#else						/*����ʽP��I��D������С*/
p_i_d_Value_TypeDef PID_Value_Speed[4] = {{ 0.0 , 0.0 , 0.0 },
										  { 0.0 , 0.0 , 0.0 },
										  { 0.0 , 0.0 , 0.0 },
										  { 0.0 , 0.0 , 0.0 }};	//�ٶȿ���pid������С
#endif

/**************************************************************************
�������ܣ�PID��ʼ����������PID�㷨���õ��Ĳ������㣬p i d��ֵ��λ
��ڲ�����PID_TypeDef:Ҫ�����PID�������ϣ�pid_value:p i d��ֵ�ĸ�λֵ����
����  ֵ����
**************************************************************************/
void Pid_Init( PID_TypeDef* PID, p_i_d_Value_TypeDef* pid_Value )
{
	PID->SetPoint = 0.0;
	PID->ActualValue = 0.0;
	PID->SumError = 0.0;
	PID->Error = 0.0;
	PID->LastError = 0.0;
	PID->PrevError = 0.0;
	PID->Proportion = pid_Value->KP;
	PID->Integral = pid_Value->KI;
	PID->Derivative = pid_Value->KD;
}

/**************************************************************************
�������ܣ�PID�ջ������㷨
��ڲ�����PID_TypeDef:PID�������ϣ�Feedback_value:����ֵ/��ʵֵ��Mode:ģʽѡ��0λ��ʽ��1����ʽ
����  ֵ����PID�㷨���������ֵ
**************************************************************************/
int32_t Pid_control( PID_TypeDef* PID, float Feedback_value, unsigned char Mode )
{  
//	printf("Feedback_value=%.2f\r\n",Feedback_value);

		if ( PID->SumError > 10000)
		{
			PID->SumError = 10000;
		}
		else if (PID->SumError < -10000)
		{
			PID->SumError = -10000;
		}
	PID->Error = PID->SetPoint - Feedback_value;
//	printf("PID->Error=%.2f\r\n",PID->Error);
	if (Mode == 0)
	{
		PID->SumError += PID->Error;
		PID->ActualValue = PID->Proportion * PID->Error
							+ PID->Integral * PID->SumError
							+ PID->Derivative * (PID->Error - PID->LastError);
		PID->LastError = PID->Error;
	}
	else
	{
		PID->ActualValue += PID->Proportion * (PID->Error - PID->LastError)
							+ PID->Integral * PID->Error
							+ PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError);
		PID->PrevError = PID->LastError;
		PID->LastError = PID->Error;
	}
//	printf("ActualValue=%.2f\r\n",PID->ActualValue);
	return PID->ActualValue;
}

/**************************************************************************
�������ܣ�PID�����ֵ�޷�
��ڲ�����PID_TypeDef:PID�������ϣ�limit:��ֵ��С(-limit~limit)
����  ֵ�������ƺ��PID���ֵ
**************************************************************************/
int32_t Pid_OutLimit( PID_TypeDef* PID, int32_t limit)
{
	if ( PID->ActualValue > limit)
	{
		PID->ActualValue = limit;
	}
	else if (PID->ActualValue < -limit)
	{
		PID->ActualValue = -limit;
	}
	return PID->ActualValue;
}
