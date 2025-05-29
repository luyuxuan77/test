#ifndef SCREEN_H
#define SCREEN_H
#include "sys.h"

void Load_Drow_Dialog(void);
void rtp_test(void);

void Numeric_Keyboard(u16 x1, u16 y1, u16 x2, u16 y2);
char LCD_KeyBoard_Scan(u16 x1, u16 y1, u16 x2, u16 y2);
float Get_LCD_KeyBoard_Value(u16 x1, u16 y1, u16 x2, u16 y2);
float Get_Array_Value(char* value);
char* Array_Data_Conversion(char* value);
#endif
