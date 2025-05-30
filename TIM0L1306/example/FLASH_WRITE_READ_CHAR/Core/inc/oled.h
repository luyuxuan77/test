//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   P1^0（SCL）
//              D1   P1^1（SDA）
//              RES  接P12
//              DC   接P13
//              CS   接P14               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	
#include "ti_msp_dl_config.h"
//#include "sys.h"
//#include "stdlib.h"	  
#define  u8  char 
#define  u32 unsigned int 
	

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0



//原示例
//sbit OLED_CS=P0^3; //片选
//sbit OLED_RST =P0^5;//复位
//sbit OLED_DC =P0^4;//数据/命令控制
//sbit OLED_SCL=P0^7;//时钟 D0（SCLK?
//sbit OLED_SDIN=P0^6;//D1（MOSI） 数据

//原示例
/*
#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1;
*/

//修改为M0
//PA2
#define OLED_CS_Clr()  DL_GPIO_clearPins(GPIOA,GPIO_GRP_0_CS_PIN)
#define OLED_CS_Set()  DL_GPIO_setPins(GPIOA,GPIO_GRP_0_CS_PIN)


#define OLED_RST_Clr()  ;
#define OLED_RST_Set()  ;

#define OLED_DC_Clr() DL_GPIO_clearPins(GPIOA,GPIO_GRP_0_DC_PIN)
#define OLED_DC_Set() DL_GPIO_setPins(GPIOA,GPIO_GRP_0_DC_PIN)



#define OLED_SCLK_Clr() DL_GPIO_clearPins(GPIOA,GPIO_GRP_0_D0_PIN)
#define OLED_SCLK_Set() DL_GPIO_setPins(GPIOA,GPIO_GRP_0_D0_PIN )

#define OLED_SDIN_Clr() DL_GPIO_clearPins(GPIOA,GPIO_GRP_0_D1_PIN)
#define OLED_SDIN_Set() DL_GPIO_setPins(GPIOA,GPIO_GRP_0_D1_PIN )







//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   

void delay_ms(unsigned int ms);


 		     

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



