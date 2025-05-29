#ifndef CCD_H
#define CCD_H
#include "sys.h"
#include "pid.h"

//线性CCD模块数据结构体
typedef struct
{
	u8 receive_data[128];  // 采集到的CCD原始数据
	u8 process_data[128];  // 用于做数据处理的数据
	u8 binaryzation_data[128];  // 二值化处理后的数据
	u8 receive_flag;  // 采集完成标志
	u8 threshold_value;  // 数据阈值
	u8 left_edge_location;  // 左跳变沿位置
	u8 right_edge_location;  // 右跳变沿位置
}CCD_TypeDef;
extern CCD_TypeDef CCD_Data;

//巡线，黑线相关数据结构体
typedef struct
{
	u8 line_left;  // 线的左边坐标
	u8 line_right;  // 线的右边坐标
	u8 line_width;  // 线的宽度
	u8 line_location;  // 线的位置
	int line_offset;  // 线的偏差
}Line_TypeDef;
extern Line_TypeDef Line_Data;

//线性CCD巡线Pid相关结构体
extern PID_TypeDef CCD_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD[3];

//线性CCD做方向环相关结构体
extern PID_TypeDef CCD_Direction_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD_Direction[3];

//线性CCD引脚定义
#define CCD_CLK	PFout(14)  // CCD时钟信号引脚
#define CCD_SI	PFout(15)  // CCD输入信号引脚
#define	CCD_AO_CH	15  // 灰度值输出引脚，输出为ADC值，此为ADC3的通道数

void CCD_Init(void);
void Get_CCD_Value(void);
void Dly_us(void);
void Send_To_Serial_Port(u8 mode);
void Send_To_Debug(u8 mode);
char binToHex_low(u8 num);
char binToHex_high(u8 num);

void Get_CCD_Threshold_Value(void);
void Get_Edge_StaticThreshold(u8 threshold_value);
void Get_Edge_DynamicThreshold(void);
void Edge_Dispose(u8 mode);
void CCD_Track_Pid(u16 pwm);
void CCD_Track_Direction_Control(float speed);
#endif

