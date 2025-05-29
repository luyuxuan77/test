#ifndef CONTROL_H
#define CONTROL_H
#include "sys.h"
#include "delay.h"

//小车状态标志
#define Line_Walking_State					1  // 巡线状态
#define Specific_Action_Execute_State		2  //  特定动作执行状态
#define Specific_Action_Accomplish_State	3  //  特定动作完成状态
#define	Stop_And_Switch_State				0  //  停止与模式切换状态

//小车特殊动作
#define Stop		0  //  停止动作
#define Advance 	1  //  执行动作
#define	Left_Turn	2  //  左转动作
#define Right_Turn	3  //  右转动作
#define Turn_Round	4  //  掉头动作
#define Park		5  //  停车动作

#define Specific_Action_Group_Amount	6  //  一套动作的数量

//行车记录仪结构体变量
typedef struct
{
	u8 Car_State_Flag;  //小车的状态
	u8 Specific_Action_Group[Specific_Action_Group_Amount];  //  小车特殊动作数组，用于储存小车动作组
	u8 Specific_Action_Group_i;  //  小车特殊动作组执行位数，用于判断小车执行到第几个动作
}Automobile_Data_Recorder_TypeDef;
extern Automobile_Data_Recorder_TypeDef Automobile_Data_Recorder;
extern int Error,pid_result;

void Control_1(void);
void Control_2(void);
void Specific_Action_Execute(void);
void Car_Stop(void);
void Go_Straight(void);
void Left_turn(void);
void Right_turn(void);
void Turn_About(void);
void Car_Park(void);
int My_D_algorithm(int error);

#endif
