#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "motor.h"
#include "pid.h"
#include "uart1.h"
#include "uart7.h"
#include "track.h"
typedef struct
{
	u8 Distance_state;
	u16 Distance_length;
	int16_t Distance_offset; // 改为有符号类型
} Distance_TypeDef;


extern Distance_TypeDef Distance_Data;
extern PID_TypeDef Distance_Pid;/*!pid算法!中用到的参数集合*/
extern p_i_d_Value_TypeDef PID_Value_Distance[3];
extern volatile int checkpoint_count ;  // 记录当前圈内经过的监测点数量

void Track_Direction_Distance_Control(float speed);
void Track_Direction_Distance_Control1(float speed);
void Track_Direction_Distance_Control2(float speed);
void Track_Direction_Distance_Control3(float speed);


int task1(void);
int task2(void);
int task3(void);
int task4(void);
#endif
