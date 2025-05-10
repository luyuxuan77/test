#ifndef TRACK_H
#define TRACK_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "pid.h"

#define u8 unsigned char
#define u16 unsigned int

//7·�ҶȽṹ�����������Ҷȸ�����
typedef struct
{
	u8 grayscale_state;		//��ǰ�Ҷ�״̬
	u8 grayscale_digital;	//��ǰ�Ҷ�������
	int grayscale_offset;	//��ǰ�Ҷ�ƫ����
}Grayscale_TypeDef;


typedef struct
{
	float angle_offset;	//��ǰ�Ҷ�ƫ����
	float distance_offset;
}UWB_TypeDef;

extern UWB_TypeDef UWB_Data;
extern Grayscale_TypeDef Grayscale_Data;

extern PID_TypeDef Grayscale_Direction_Pid;
extern PID_TypeDef UWB_Angle_Pid;/*!pid�㷨!���õ��Ĳ�������*/
extern PID_TypeDef UWB_Distance_Pid;/*!pid�㷨!���õ��Ĳ�������*/
extern PID_TypeDef Grayscale_Pid;


//����Ҷȴ���������pid�����ṹ�������p��i��d��ֵ�ṹ���������
extern p_i_d_Value_TypeDef PID_Value_UWB_Angle[1];	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�/*���������֣�΢��3ֵ��С�ṹ��*/

extern p_i_d_Value_TypeDef PID_Value_UWB_Distance[1];	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�/*���������֣�΢��3ֵ��С�ṹ��*/

extern p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3];


extern float UWB_Current_Angle;
extern float UWB_Current_Distance;

#define Grayscale_Sensor_Digital	(unsigned char)(PE2 << 6) | (PD3 << 5) | (PD2 << 4) | (PD1 << 3) | (PD0 << 2) | (PB3 << 1) | (PB2 << 0)	//�Ҷȴ���������������


#define PB2 						GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) / GPIO_PIN_2
#define PB3 						GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3) / GPIO_PIN_3
#define PD0 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0) / GPIO_PIN_0
#define PD1 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1) / GPIO_PIN_1
#define PD2 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) / GPIO_PIN_2
#define PD3 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) / GPIO_PIN_3
#define PE2 						GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2) / GPIO_PIN_2

#define NUM_SENSORS 7		//�Ƶ�����
extern float sensor_weights[NUM_SENSORS];
void Line7_Init(void);
int Get_Grayscale_Offset(void);
void Grren_Light_Control(unsigned char mode);
void Blue_Light_Control(unsigned char mode);
void Track_Direction_Control(float speed);
void Track_Angle_Distance_Control(void);

#endif

