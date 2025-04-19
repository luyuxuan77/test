#ifndef PID_H
#define PID_H
#include <stdint.h>
#include <stdbool.h>

//PID�ṹ�����������PID������
typedef struct
{
	float SetPoint;		/*Ŀ��ֵ*/
	float ActualValue;	/*����ֵ*/
	float SumError;		/*�ۼ�ƫ��*/
	float Proportion;	/*����ϵ��*/
	float Integral;		/*����ϵ��*/
	float Derivative;	/*΢��ϵ��*/
	float Error;		/*�˴�ƫ��*/
	float LastError;	/*�ϴ�ƫ��*/
	float PrevError;	/*���ϴ�ƫ��*/
}PID_TypeDef;			/*pid�㷨���õ��Ĳ�������*/
extern PID_TypeDef Position_position_Pid[4];
extern PID_TypeDef Position_speed_Pid[4];
extern PID_TypeDef Speed_Pid[4];

#define L1_Position_position_Pid	Position_position_Pid[0]	//��Position_Pid[0]������ΪL1_Position_Pid����������д
#define R1_Position_position_Pid	Position_position_Pid[1]	//��Position_Pid[1]������ΪR1_Position_Pid����������д
#define L2_Position_position_Pid	Position_position_Pid[2]	//��Position_Pid[2]������ΪL2_Position_Pid����������д
#define R2_Position_position_Pid	Position_position_Pid[3]	//��Position_Pid[3]������ΪR2_Position_Pid����������д
#define L1_Position_speed_Pid	Position_speed_Pid[0]	//��Position_speed_Pid[0]������ΪL1_Position_Pid����������д
#define R1_Position_speed_Pid	Position_speed_Pid[1]	//��Position_speed_Pid[1]������ΪR1_Position_Pid����������д
#define L2_Position_speed_Pid	Position_speed_Pid[2]	//��Position_speed_Pid[2]������ΪL2_Position_Pid����������д
#define R2_Position_speed_Pid	Position_speed_Pid[3]	//��Position_speed_Pid[3]������ΪR2_Position_Pid����������д

#define L1_Speed_Pid	Speed_Pid[0]	//��Speed_Pid[0]������ΪL1_Speed_Pid����������д
#define R1_Speed_Pid	Speed_Pid[1]	//��Speed_Pid[1]������ΪR1_Speed_Pid����������д
#define L2_Speed_Pid	Speed_Pid[2]	//��Speed_Pid[2]������ΪL2_Speed_Pid����������д
#define R2_Speed_Pid	Speed_Pid[3]	//��Speed_Pid[3]������ΪR2_Speed_Pid����������д

//p��i��d��ֵ�ṹ�壬����ĳpid���������е�p��i��d��ֵ
typedef struct
{
	float KP;			/*��������*/
	float KI;			/*���ֳ���*/
	float KD;			/*΢�ֳ���*/
}p_i_d_Value_TypeDef;	/*���������֣�΢��3ֵ��С�ṹ��*/
extern p_i_d_Value_TypeDef PID_Value_Position_position[4];
extern p_i_d_Value_TypeDef PID_Value_Position_speed[4];
extern p_i_d_Value_TypeDef PID_Value_Speed[4];

#define POSITION_Position_PID_TYPE	0	/*λ�ÿ���λ�û�PID�㷨���ͣ�0λ��ʽ��1����ʽ*/
#define POSITION_Speed_PID_TYPE 	0	/*λ�ÿ���λ�û�PID�㷨���ͣ�0λ��ʽ��1����ʽ*/
#define SPEED_PID_TYPE				0	/*�ٶȿ���PID�㷨���ͣ�0λ��ʽ��1����ʽ*/

#define L1_PID_Value_Position_position	PID_Value_Position_position[0]	//��0�ŵ������Ϊ��1�֣�PID_Value_Position[0]Ϊ0�ŵ��λ�ÿ���λ�û�p��i��d��ֵ�����ڴ���0�ŵ��p��i��d��ֵ��
#define R1_PID_Value_Position_position	PID_Value_Position_position[1]	//��1�ŵ������Ϊ��1�֣�PID_Value_Position[1]Ϊ1�ŵ��λ�ÿ���λ�û�p��i��d��ֵ�����ڴ���1�ŵ��p��i��d��ֵ��
#define L2_PID_Value_Position_position	PID_Value_Position_position[2]	//��2�ŵ������Ϊ��2�֣�PID_Value_Position[2]Ϊ2�ŵ��λ�ÿ���λ�û�p��i��d��ֵ�����ڴ���2�ŵ��p��i��d��ֵ��
#define R2_PID_Value_Position_position	PID_Value_Position_position[3]	//��3�ŵ������Ϊ��2�֣�PID_Value_Position[3]Ϊ3�ŵ��λ�ÿ���λ�û�p��i��d��ֵ�����ڴ���3�ŵ��p��i��d��ֵ��
#define L1_PID_Value_Position_speed	PID_Value_Position_speed[0]	//��0�ŵ������Ϊ��1�֣�PID_Value_Position[0]Ϊ0�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ�����ڴ���0�ŵ��p��i��d��ֵ��
#define R1_PID_Value_Position_speed	PID_Value_Position_speed[1]	//��1�ŵ������Ϊ��1�֣�PID_Value_Position[1]Ϊ1�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ�����ڴ���1�ŵ��p��i��d��ֵ��
#define L2_PID_Value_Position_speed	PID_Value_Position_speed[2]	//��2�ŵ������Ϊ��2�֣�PID_Value_Position[2]Ϊ2�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ�����ڴ���2�ŵ��p��i��d��ֵ��
#define R2_PID_Value_Position_speed	PID_Value_Position_speed[3]	//��3�ŵ������Ϊ��2�֣�PID_Value_Position[3]Ϊ3�ŵ��λ�ÿ����ٶȻ�p��i��d��ֵ�����ڴ���3�ŵ��p��i��d��ֵ��

#define L1_PID_Value_Speed	PID_Value_Speed[0]	//��0�ŵ������Ϊ��1�֣�PID_Value_Position[0]Ϊ0�ŵ���ٶȿ���p��i��d��ֵ�����ڴ���0�ŵ��p��i��d��ֵ��
#define R1_PID_Value_Speed	PID_Value_Speed[1]	//��1�ŵ������Ϊ��1�֣�PID_Value_Position[1]Ϊ1�ŵ���ٶȿ���p��i��d��ֵ�����ڴ���1�ŵ��p��i��d��ֵ��
#define L2_PID_Value_Speed	PID_Value_Speed[2]	//��2�ŵ������Ϊ��2�֣�PID_Value_Position[2]Ϊ2�ŵ���ٶȿ���p��i��d��ֵ�����ڴ���2�ŵ��p��i��d��ֵ��
#define R2_PID_Value_Speed	PID_Value_Speed[3]	//��3�ŵ������Ϊ��2�֣�PID_Value_Position[3]Ϊ3�ŵ���ٶȿ���p��i��d��ֵ�����ڴ���3�ŵ��p��i��d��ֵ��

void Pid_Init( PID_TypeDef* PID, p_i_d_Value_TypeDef* pid_Value );
int32_t Pid_control( PID_TypeDef* PID, float Feedback_value, unsigned char Mode );
int32_t Pid_OutLimit( PID_TypeDef* PID, int32_t limit);
#endif




