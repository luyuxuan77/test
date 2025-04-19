#ifndef _GRAY_H_
#define _GRAY_H_
#include "bsp.h"
#include "PID/pid.h"


/* Defines for P0: GPIOA.15 with pinCMx 37 on package pin 8 */
#define Gray_P0_PORT                                                     (GPIOA)
#define Gray_P0_PIN                                             (DL_GPIO_PIN_15)
#define Gray_P0_IOMUX                                            (IOMUX_PINCM37)
/* Defines for P1: GPIOA.16 with pinCMx 38 on package pin 9 */
#define Gray_P1_PORT                                                     (GPIOA)
#define Gray_P1_PIN                                             (DL_GPIO_PIN_16)
#define Gray_P1_IOMUX                                            (IOMUX_PINCM38)
/* Defines for P2: GPIOA.17 with pinCMx 39 on package pin 10 */
#define Gray_P2_PORT                                                     (GPIOA)
#define Gray_P2_PIN                                             (DL_GPIO_PIN_17)
#define Gray_P2_IOMUX                                            (IOMUX_PINCM39)
/* Defines for P3: GPIOB.17 with pinCMx 43 on package pin 14 */
#define Gray_P3_PORT                                                     (GPIOB)
#define Gray_P3_PIN                                             (DL_GPIO_PIN_17)
#define Gray_P3_IOMUX                                            (IOMUX_PINCM43)
/* Defines for P4: GPIOB.18 with pinCMx 44 on package pin 15 */
#define Gray_P4_PORT                                                     (GPIOB)
#define Gray_P4_PIN                                             (DL_GPIO_PIN_18)
#define Gray_P4_IOMUX                                            (IOMUX_PINCM44)
/* Defines for P5: GPIOB.19 with pinCMx 45 on package pin 16 */
#define Gray_P5_PORT                                                     (GPIOB)
#define Gray_P5_PIN                                             (DL_GPIO_PIN_19)
#define Gray_P5_IOMUX                                            (IOMUX_PINCM45)
/* Defines for P6: GPIOA.22 with pinCMx 47 on package pin 18 */
#define Gray_P6_PORT                                                     (GPIOA)
#define Gray_P6_PIN                                             (DL_GPIO_PIN_22)
#define Gray_P6_IOMUX                                            (IOMUX_PINCM47)

#define u8 unsigned char
#define Grayscale_Sensor_Digital	(unsigned char)(P6<<6)|(P5<<5)|(P4<<4)|(P3<<3)|(P2<<2)|(P1<<1)|(P0<<0)	//灰度传感器数字量计算

#define P0 						DL_GPIO_readPins(Gray_P0_PORT,Gray_P0_PIN)/Gray_P0_PIN
#define P1 						DL_GPIO_readPins(Gray_P1_PORT,Gray_P1_PIN)/Gray_P1_PIN
#define P2 						DL_GPIO_readPins(Gray_P2_PORT,Gray_P2_PIN)/Gray_P2_PIN
#define P3 						DL_GPIO_readPins(Gray_P3_PORT,Gray_P3_PIN)/Gray_P3_PIN
#define P4 						DL_GPIO_readPins(Gray_P4_PORT,Gray_P4_PIN)/Gray_P4_PIN
#define P5 						DL_GPIO_readPins(Gray_P5_PORT,Gray_P5_PIN)/Gray_P5_PIN
#define P6 						DL_GPIO_readPins(Gray_P6_PORT,Gray_P6_PIN)/Gray_P6_PIN

//7路灰度结构体变量，储存灰度各数据
typedef struct
{
	u8 grayscale_state;		//当前灰度状态
	u8 grayscale_digital;	//当前灰度数字量
	int grayscale_offset;	//当前灰度偏移量
}Grayscale_TypeDef;

extern Grayscale_TypeDef Grayscale_Data;

extern PID_TypeDef Grayscale_Pid;
extern p_i_d_Value_TypeDef PID_Value_Grayscale[3];

extern PID_TypeDef Grayscale_Direction_Pid;
extern p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3];
#define NUM_SENSORS 7		//灯的数量
extern float sensor_weights[NUM_SENSORS];	//灯的位权数组

void gray_init(void);
uint8_t gray_read(void);
int Get_Grayscale_Offset(void);
void Track_Direction_Control(float speed);
void Grren_Light_Control(unsigned char mode);
void Blue_Light_Control(unsigned char mode);





#endif
