#ifndef CCD_H
#define CCD_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "pid.h"

//����CCDģ�����ݽṹ��
typedef struct
{
	unsigned char receive_data[128];		//�ɼ�����CCDԭʼ����
	unsigned char process_data[100];		//���������ݴ��������
	unsigned char binaryzation_data[100];	//��ֵ������������
	unsigned char receive_flag;				//�ɼ���ɱ�־
	unsigned char threshold_value;			//������ֵ
	unsigned char left_edge_location;		//��������λ��
	unsigned char right_edge_location;		//��������λ��
}CCD_TypeDef;
extern CCD_TypeDef CCD_Data;

//Ѳ�ߣ�����������ݽṹ��
typedef struct
{
	unsigned char line_left;	//�ߵ��������
	unsigned char line_right;	//�ߵ��ұ�����
	unsigned char line_width;	//�ߵĿ��
	unsigned char line_location;//�ߵ�λ��
	
	unsigned char last_line_left;		//�����ϴ�������
	unsigned char last_line_right;		//�����ϴ������
	unsigned char last_line_width;		//���ߵĿ��
	unsigned char last_line_location;	//�����ϴ�λ��
	
	int line_offset;	//�ߵ�ƫ��
}Line_TypeDef;
extern Line_TypeDef Line_Data;

//����CCDѲ��Pid��ؽṹ��
extern PID_TypeDef CCD_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD[3];

//����CCD��������ؽṹ��
extern PID_TypeDef CCD_Direction_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD_Direction[3];

//����CCD���Ŷ���
#define CCD_CLK_DOWN	MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0)			//CCDʱ���ź���������
#define CCD_CLK_UP		MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2)	//CCDʱ���ź���������
#define CCD_SI_DOWN		MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0)			//CCD�����ź�����
#define CCD_SI_UP		MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3)	//CCD�����ź�����

void CCD_Init(void);
void Line_Data_Init(void);
void Get_CCD_Value(void);
void Dly_us(void);
void Send_To_Serial_Port(unsigned char mode);
void Send_To_Debug(unsigned char mode);
char binToHex_low(unsigned char num);
char binToHex_high(unsigned char num);

void Get_CCD_Threshold_Value(void);
void Get_Edge_StaticThreshold(unsigned char threshold_value);
void Get_Edge_DynamicThreshold(void);
void Edge_Dispose(unsigned char mode);
void CCD_Track_Pid(uint16_t pwm);
void CCD_Track_Direction_Control(float speed);

#endif


