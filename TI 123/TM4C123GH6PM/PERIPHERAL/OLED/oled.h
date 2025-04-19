#ifndef __OLED_H
#define __OLED_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

//OLED模式设置
//0: 4线串行模式  （模块的BS1，BS2均接GND）
		    						  
//-----------------OLED端口定义----------------
//#define OLED_SCLK 	PDout(11) 	//CLK (PD11)
//#define OLED_SDIN 	PDout(12) 	//MOSI(PD12)
//#define OLED_RST  	PDout(13)	//RES (PD13)
//#define OLED_RS 	PDout(14) 	//DC  (PD14)
//#define OLED_CS 	PDout(15)  	//CS  (PD15)
		     
#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据
//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);	 
void OLED_ShowGBK(uint8_t x, uint8_t y, uint8_t num, uint8_t size, uint8_t mode);
void menu(void);
void IO_Reversal(void);
#endif  
	 



