#ifndef GYRO_U5_H
#define GYRO_U5_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

//�����ǽṹ�����
typedef struct
{
	unsigned char receive_data[11];	//���յ�������
	unsigned char receive_flag;		//һ֡���ݽ�����ɱ�־
	unsigned char receive_i;		//һ֡���ݽ��յ���receive_iλ
	float angle[3];					//�����ǵõ�������Ƕ�ֵ
	float w[3];						//�����ǵõ���������ٶ�ֵ
	float a[3];						//�����ǵõ���������ٶ�ֵ
	float tem;						//�������¶�ֵ
}Gyro_Data_TypeDef;
extern Gyro_Data_TypeDef Gyro_Data;

#define Gyro_A		0x01	//ѡ����������ǽǼ��ٶ�����
#define Gyro_W		0x02	//ѡ����������ǽ��ٶ�����
#define Gyro_Angle	0x04	//ѡ����������ǽǶ�����

#define Angle_x	Gyro_Data.angle[0]	//��������x��Ƕ�ֵ
#define Angle_y	Gyro_Data.angle[1]	//��������y��Ƕ�ֵ
#define Angle_z	Gyro_Data.angle[2]	//��������z��Ƕ�ֵ

#define W_x	Gyro_Data.w[0]			//��������x����ٶ�ֵ
#define W_y	Gyro_Data.w[1]			//��������y����ٶ�ֵ
#define W_z	Gyro_Data.w[2]			//��������z����ٶ�ֵ

#define A_x	Gyro_Data.a[0]			//��������x��Ǽ��ٶ�ֵ
#define A_y	Gyro_Data.a[1]			//��������y��Ǽ��ٶ�ֵ
#define A_z	Gyro_Data.a[2]			//��������z��Ǽ��ٶ�ֵ

void USART5_IRQHandler(void);
void uart5_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART5Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void Attitude_Information_Acquisition(unsigned char data_choice);
#endif
