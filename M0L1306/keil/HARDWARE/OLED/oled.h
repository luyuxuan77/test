#ifndef OLED_H
#define OLED_H

#include "bsp.h" // 确保包含GPIO相关定义
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

//OLED控制用函数
void oled_init(void);
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
/* Port definition for Pin Group SWITCH */
#define SWITCH_PORT                                                      (GPIOA)

/* Defines for PIN_0: GPIOA.1 with pinCMx 2 on package pin 2 */
// pins affected by this interrupt request:["PIN_0"]
#define SWITCH_INT_IRQN                                         (GPIOA_INT_IRQn)
#define SWITCH_INT_IIDX                         (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define SWITCH_PIN_0_IIDX                                    (DL_GPIO_IIDX_DIO1)
#define SWITCH_PIN_0_PIN                                         (DL_GPIO_PIN_1)
#define SWITCH_PIN_0_IOMUX                                        (IOMUX_PINCM2)
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOA)

/* Defines for PIN_1: GPIOA.8 with pinCMx 9 on package pin 12 */
#define LED_PIN_1_PIN                                            (DL_GPIO_PIN_8)
#define LED_PIN_1_IOMUX                                           (IOMUX_PINCM9)
/* Port definition for Pin Group GPIO_GRP_0 */
#define GPIO_GRP_0_PORT                                                  (GPIOA)

/* Defines for CS: GPIOA.2 with pinCMx 3 on package pin 6 */
#define GPIO_GRP_0_CS_PIN                                        (DL_GPIO_PIN_2)
#define GPIO_GRP_0_CS_IOMUX                                       (IOMUX_PINCM3)
/* Defines for DC: GPIOA.4 with pinCMx 5 on package pin 8 */
#define GPIO_GRP_0_DC_PIN                                        (DL_GPIO_PIN_4)
#define GPIO_GRP_0_DC_IOMUX                                       (IOMUX_PINCM5)
/* Defines for D0: GPIOA.6 with pinCMx 7 on package pin 10 */
#define GPIO_GRP_0_D0_PIN                                        (DL_GPIO_PIN_6)
#define GPIO_GRP_0_D0_IOMUX                                       (IOMUX_PINCM7)
/* Defines for D1: GPIOA.5 with pinCMx 6 on package pin 9 */
#define GPIO_GRP_0_D1_PIN                                        (DL_GPIO_PIN_5)
#define GPIO_GRP_0_D1_IOMUX                                       (IOMUX_PINCM6)


#endif  
	 
