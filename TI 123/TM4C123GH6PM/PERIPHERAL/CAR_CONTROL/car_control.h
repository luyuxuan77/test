#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

//С��״̬��־
#define Line_Walking_State					1	//Ѳ��״̬
#define Specific_Action_Execute_State		2	//�ض�����ִ��״̬
#define Specific_Action_Accomplish_State	3	//�ض��������״̬
#define	Stop_And_Switch_State				0	//ֹͣ��ģʽ�л�״̬

//С�����⶯��
#define Stop		0	//ֹͣ����
#define Advance 	1	//ֱ�ж���
#define	Left_Turn	2	//��ת����
#define Right_Turn	3	//��ת����
#define Turn_Round	4	//��ͷ����
#define Park		5	//ͣ������

#define Special_1	6	//���⶯��1
#define Special_2	7	//���⶯��2

#define Specific_Action_Group_Amount	2	//һ�׶���������

//�г���¼�ǽṹ�����
typedef struct
{
	unsigned char Car_State_Flag;		//С����״̬
	unsigned char Specific_Action_Group[Specific_Action_Group_Amount];	//С�����⶯�����飬���ڴ���С��������
	unsigned char Specific_Action_Group_i;	//С�����⶯����ִ��λ���������ж�С��ִ�е��ڼ�������
}Automobile_Data_Recorder_TypeDef;
extern Automobile_Data_Recorder_TypeDef Automobile_Data_Recorder;

void Specific_Action_Execute(void);
void Car_Stop(void);
void Go_Straight(void);
void Left_turn(void);
void Right_turn(void);
void Turn_About(void);
void Car_Park(void);
void Car_Special1(void);
void Car_Special2(void);
#endif
