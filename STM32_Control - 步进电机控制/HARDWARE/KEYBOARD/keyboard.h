#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "sys.h"

/************************数字键盘定义*********************/
/*	K1	K2	K3	K4			1	2	3	Tes
	K5	K6	K7	K8	---->	4	5	6	Del
	K9	K10	K11	K12	---->	7	8	9	Res
	K13	K14	K15	K16			-	0	.	Ent
*/

/************************按键接线*************************/
/*
行	PIN4-->PG15		PIN5-->PG13		PIN6-->PG11		PIN7-->PG10
列	PIN3-->PF0		PIN2-->PF1		PIN1-->PF2		PIN0-->PG8
*/

void KeyBoard_Init(void);
u8 KeyBoard_Scan(u8 Mode);
void Key_Reset(u8 column);
u8 Key_Transtition_Value(u8 key);

int Get_Int_Numeric_Keyboard(void);
float Get_KeyBoard_Value(void);
#endif
