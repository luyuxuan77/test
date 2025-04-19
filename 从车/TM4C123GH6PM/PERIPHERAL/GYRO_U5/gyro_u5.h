#ifndef GYRO_U5_H
#define GYRO_U5_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

//陀螺仪结构体变量
typedef struct
{
	unsigned char receive_data[11];	//接收到的数据
	unsigned char receive_flag;		//一帧数据接收完成标志
	unsigned char receive_i;		//一帧数据接收到第receive_i位
	float angle[3];					//陀螺仪得到的三轴角度值
	float w[3];						//陀螺仪得到的三轴角速度值
	float a[3];						//陀螺仪得到的三轴加速度值
	float tem;						//陀螺仪温度值
}Gyro_Data_TypeDef;
extern Gyro_Data_TypeDef Gyro_Data;

#define Gyro_A		0x01	//选择解析陀螺仪角加速度数据
#define Gyro_W		0x02	//选择解析陀螺仪角速度数据
#define Gyro_Angle	0x04	//选择解析陀螺仪角度数据

#define Angle_x	Gyro_Data.angle[0]	//陀螺仪绕x轴角度值
#define Angle_y	Gyro_Data.angle[1]	//陀螺仪绕y轴角度值
#define Angle_z	Gyro_Data.angle[2]	//陀螺仪绕z轴角度值

#define W_x	Gyro_Data.w[0]			//陀螺仪绕x轴角速度值
#define W_y	Gyro_Data.w[1]			//陀螺仪绕y轴角速度值
#define W_z	Gyro_Data.w[2]			//陀螺仪绕z轴角速度值

#define A_x	Gyro_Data.a[0]			//陀螺仪绕x轴角加速度值
#define A_y	Gyro_Data.a[1]			//陀螺仪绕y轴角加速度值
#define A_z	Gyro_Data.a[2]			//陀螺仪绕z轴角加速度值

void USART5_IRQHandler(void);
void uart5_Init(uint32_t ui32Baud, uint32_t ui32SrcClock);
void UART5Send(const uint8_t *pui8Buffer, uint32_t ui32Count);
void Attitude_Information_Acquisition(unsigned char data_choice);
#endif
