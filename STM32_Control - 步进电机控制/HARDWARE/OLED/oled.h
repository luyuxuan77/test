#ifndef OLED_H
#define OLED_H
#include "sys.h"
#include "stdlib.h"

#define OLED_CS 	PDout(13)  // CSƬѡ
#define OLED_RS 	PDout(12)  // D/C����/ʱ��
#define OLED_RST  	PDout(11)  // RES��λ
#define OLED_SDIN 	PDout(7)  // D1������˿�
#define OLED_SCLK 	PDout(6)  // D0ʱ��CLK

#define OLED_CMD  	0  // д����
#define OLED_DATA 	1  // д����

void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);

void OLED_ShowGBK(u8 x, u8 y,  u8 num, u8 size,u8 mode);
#endif

