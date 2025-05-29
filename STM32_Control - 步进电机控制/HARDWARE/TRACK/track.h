#ifndef TRACK_H
#define TRACK_H
#include "sys.h"
#include "pid.h"

//7·�ҶȽṹ�����������Ҷȸ�����
typedef struct
{
	u8 grayscale_state;  // ��ǰ�Ҷ�״̬
	u8 grayscale_digital;  // ��ǰ�Ҷ�������
	int grayscale_offset;  // ��ǰ�Ҷ�ƫ����
}Grayscale_TypeDef;
extern Grayscale_TypeDef Grayscale_Data;

extern PID_TypeDef Grayscale_Pid;
extern p_i_d_Value_TypeDef PID_Value_Grayscale[3];

extern PID_TypeDef Grayscale_Direction_Pid;
extern p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3];

#define Laser_Sensor_Data	PGin(7)  // �����伤�⴫�������Ÿߵ�
#define Grayscale_Sensor_Digital	(u8)(GPIOG->IDR | 0x80)  // �Ҷȴ���������������

void Line7_Init(void);
void Laser_Sensor_Init(void);
void Track_Line7(u8 Mode);
void Road_Condition_Recognition(void);

int Get_Grayscale_Offset(void);
void Track_Pid(u16 pwm);
void Track_Direction_Control(float speed);
#endif

