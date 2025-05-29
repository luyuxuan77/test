#ifndef _MOTOR_H_
#define _MOTOR_H_

typedef struct
{
	unsigned char pwm_outch;	//Tim8通道号
	unsigned char direction;	//电机旋转方向，0:正转，1:反转
	float current;				//电机电流
	int speed;				//电机转速，单位r/m(转每分)
	float location;				//电机转矩，编码器计数值
	int32_t motor_pwm;			//电机PWM比较值。
}Motor_Data_TypeDef;			//定义结构体用来储存电机的各个参数
extern Motor_Data_TypeDef Motor_Data[4];
#define ROTO_RATIO		44	//编码器线数*定时器倍频系数，即11 * 4 = 44(编码器转一圈，定时器记44个数)
#define REDUCTION_RATIO	30	//电机减速比
/* Port definition for Pin Group S */
#define S_PORT                                                           (GPIOB)

/* Defines for LU_N: GPIOB.9 with pinCMx 26 on package pin 61 */
#define S_LU_N_PIN                                               (DL_GPIO_PIN_9)
#define S_LU_N_IOMUX                                             (IOMUX_PINCM26)
/* Defines for LD_N: GPIOB.20 with pinCMx 48 on package pin 19 */
#define S_LD_N_PIN                                              (DL_GPIO_PIN_20)
#define S_LD_N_IOMUX                                             (IOMUX_PINCM48)
/* Defines for RU_N: GPIOB.6 with pinCMx 23 on package pin 58 */
#define S_RU_N_PIN                                               (DL_GPIO_PIN_6)
#define S_RU_N_IOMUX                                             (IOMUX_PINCM23)
/* Defines for RD_N: GPIOB.3 with pinCMx 16 on package pin 51 */
#define S_RD_N_PIN                                               (DL_GPIO_PIN_3)
#define S_RD_N_IOMUX                                             (IOMUX_PINCM16)

#define L1_Motor	Motor_Data[3]	//L1电机定义到Motor[0]结构体（软连接，可定情况修改），Motor_Data[0]为PWM0_OUT0通道（硬连接，非必要不修改）
#define	R1_Motor	Motor_Data[0]	//R1电机定义到Motor[1]结构体（软连接，可定情况修改），Motor_Data[1]为PWM0_OUT1通道（硬连接，非必要不修改）
#define	L2_Motor	Motor_Data[2]	//L2电机定义到Motor[2]结构体（软连接，可定情况修改），Motor_Data[2]为PWM0_OUT2通道（硬连接，非必要不修改）
#define	R2_Motor	Motor_Data[1]	//R2电机定义到Motor[3]结构体（软连接，可定情况修改），Motor_Data[3]为PWM0_OUT3通道（硬连接，非必要不修改）

void motor_4(int LU,int LD,int RU,int RD);
void motor_init(void);

#endif
