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
	u8 Distance_state;		//当前距离状态(或远或近)
	u8 Distance_length;	//当前距离
	u8 Distance_offset;	//当前距离偏移量
}Distance_TypeDef;

extern Distance_TypeDef Distance_Data;
extern PID_TypeDef Distance_Pid;/*!pid算法!中用到的参数集合*/
extern p_i_d_Value_TypeDef PID_Value_Distance[3];

void Track_Direction_Distance_Control(float speed);

#endif
