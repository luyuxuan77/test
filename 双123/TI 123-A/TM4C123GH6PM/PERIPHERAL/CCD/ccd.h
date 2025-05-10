#ifndef CCD_H
#define CCD_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "pid.h"

//线性CCD模块数据结构体
typedef struct
{
	unsigned char receive_data[128];		//采集到的CCD原始数据
	unsigned char process_data[100];		//用于做数据处理的数据
	unsigned char binaryzation_data[100];	//二值化处理后的数据
	unsigned char receive_flag;				//采集完成标志
	unsigned char threshold_value;			//数据阈值
	unsigned char left_edge_location;		//左跳变沿位置
	unsigned char right_edge_location;		//右跳变沿位置
}CCD_TypeDef;
extern CCD_TypeDef CCD_Data;

//巡线，黑线相关数据结构体
typedef struct
{
	unsigned char line_left;	//线的左边坐标
	unsigned char line_right;	//线的右边坐标
	unsigned char line_width;	//线的宽度
	unsigned char line_location;//线的位置
	
	unsigned char last_line_left;		//黑线上次左坐标
	unsigned char last_line_right;		//黑线上次右左标
	unsigned char last_line_width;		//黑线的宽度
	unsigned char last_line_location;	//黑线上次位置
	
	int line_offset;	//线的偏差
}Line_TypeDef;
extern Line_TypeDef Line_Data;

//线性CCD巡线Pid相关结构体
extern PID_TypeDef CCD_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD[3];

//线性CCD做方向环相关结构体
extern PID_TypeDef CCD_Direction_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD_Direction[3];

//线性CCD引脚定义
#define CCD_CLK_DOWN	MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0)			//CCD时钟信号引脚拉低
#define CCD_CLK_UP		MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_PIN_2)	//CCD时钟信号引脚拉低
#define CCD_SI_DOWN		MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0)			//CCD输入信号引脚
#define CCD_SI_UP		MAP_GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3)	//CCD输入信号引脚

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


