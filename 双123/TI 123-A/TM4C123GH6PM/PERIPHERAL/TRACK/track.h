#ifndef TRACK_H
#define TRACK_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "pid.h"

#define u8 unsigned char
#define u16 unsigned int

//7路灰度结构体变量，储存灰度各数据
typedef struct
{
	u8 grayscale_state;		//当前灰度状态
	u8 grayscale_digital;	//当前灰度数字量
	int grayscale_offset;	//当前灰度偏移量
}Grayscale_TypeDef;


typedef struct
{
	float angle_offset;	//当前灰度偏移量
	float distance_offset;
}UWB_TypeDef;

extern UWB_TypeDef UWB_Data;
extern Grayscale_TypeDef Grayscale_Data;

extern PID_TypeDef Grayscale_Direction_Pid;
extern PID_TypeDef UWB_Angle_Pid;/*!pid算法!中用到的参数集合*/
extern PID_TypeDef UWB_Distance_Pid;/*!pid算法!中用到的参数集合*/
extern PID_TypeDef Grayscale_Pid;


//定义灰度传感器方向环pid参数结构体变量和p，i，d三值结构体变量数组
extern p_i_d_Value_TypeDef PID_Value_UWB_Angle[1];	//预存三组pid参数，用于不同的巡线速度/*比例，积分，微分3值大小结构体*/

extern p_i_d_Value_TypeDef PID_Value_UWB_Distance[1];	//预存三组pid参数，用于不同的巡线速度/*比例，积分，微分3值大小结构体*/

extern p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3];


extern float UWB_Current_Angle;
extern float UWB_Current_Distance;

#define Grayscale_Sensor_Digital	(unsigned char)(PE2 << 6) | (PD3 << 5) | (PD2 << 4) | (PD1 << 3) | (PD0 << 2) | (PB3 << 1) | (PB2 << 0)	//灰度传感器数字量计算


#define PB2 						GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2) / GPIO_PIN_2
#define PB3 						GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3) / GPIO_PIN_3
#define PD0 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0) / GPIO_PIN_0
#define PD1 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1) / GPIO_PIN_1
#define PD2 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2) / GPIO_PIN_2
#define PD3 						GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3) / GPIO_PIN_3
#define PE2 						GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2) / GPIO_PIN_2

#define NUM_SENSORS 7		//灯的数量
extern float sensor_weights[NUM_SENSORS];
void Line7_Init(void);
int Get_Grayscale_Offset(void);
void Grren_Light_Control(unsigned char mode);
void Blue_Light_Control(unsigned char mode);
void Track_Direction_Control(float speed);
void Track_Angle_Distance_Control(void);

#endif

