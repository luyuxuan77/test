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
	u8 Distance_state;		//��ǰ����״̬(��Զ���)
	u8 Distance_length;	//��ǰ����
	u8 Distance_offset;	//��ǰ����ƫ����
}Distance_TypeDef;

extern Distance_TypeDef Distance_Data;
extern PID_TypeDef Distance_Pid;/*!pid�㷨!���õ��Ĳ�������*/
extern p_i_d_Value_TypeDef PID_Value_Distance[3];

void Track_Direction_Distance_Control(float speed);

#endif
