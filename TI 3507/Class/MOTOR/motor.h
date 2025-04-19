#ifndef _MOTOR_H_
#define _MOTOR_H_

typedef struct
{
	unsigned char pwm_outch;	//Tim8ͨ����
	unsigned char direction;	//�����ת����0:��ת��1:��ת
	float current;				//�������
	int speed;				//���ת�٣���λr/m(תÿ��)
	float location;				//���ת�أ�����������ֵ
	int32_t motor_pwm;			//���PWM�Ƚ�ֵ��
}Motor_Data_TypeDef;			//����ṹ�������������ĸ�������
extern Motor_Data_TypeDef Motor_Data[4];
#define ROTO_RATIO		44	//����������*��ʱ����Ƶϵ������11 * 4 = 44(������תһȦ����ʱ����44����)
#define REDUCTION_RATIO	30	//������ٱ�
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

#define L1_Motor	Motor_Data[3]	//L1������嵽Motor[0]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[0]ΪPWM0_OUT0ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	R1_Motor	Motor_Data[0]	//R1������嵽Motor[1]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[1]ΪPWM0_OUT1ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	L2_Motor	Motor_Data[2]	//L2������嵽Motor[2]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[2]ΪPWM0_OUT2ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	R2_Motor	Motor_Data[1]	//R2������嵽Motor[3]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[3]ΪPWM0_OUT3ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�

void motor_4(int LU,int LD,int RU,int RD);
void motor_init(void);

#endif
