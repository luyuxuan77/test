#ifndef TRACK_H
#define TRACK_H
#include "sys.h"
#include "pid.h"

//7路灰度结构体变量，储存灰度各数据
typedef struct
{
	u8 grayscale_state;  // 当前灰度状态
	u8 grayscale_digital;  // 当前灰度数字量
	int grayscale_offset;  // 当前灰度偏移量
}Grayscale_TypeDef;
extern Grayscale_TypeDef Grayscale_Data;

extern PID_TypeDef Grayscale_Pid;
extern p_i_d_Value_TypeDef PID_Value_Grayscale[3];

extern PID_TypeDef Grayscale_Direction_Pid;
extern p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3];

#define Laser_Sensor_Data	PGin(7)  // 漫反射激光传感器引脚高低
#define Grayscale_Sensor_Digital	(u8)(GPIOG->IDR | 0x80)  // 灰度传感器数字量计算

void Line7_Init(void);
void Laser_Sensor_Init(void);
void Track_Line7(u8 Mode);
void Road_Condition_Recognition(void);

int Get_Grayscale_Offset(void);
void Track_Pid(u16 pwm);
void Track_Direction_Control(float speed);
#endif

