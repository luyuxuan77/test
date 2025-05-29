#ifndef MOTOR_H
#define MOTOR_H
#include "sys.h"
#include "delay.h"

//������ݽṹ�壬�ں����������
typedef struct
{
	u8 ch_number;  // Tim8ͨ����
	u8 direction;  // �����ת����0:��ת��1:��ת
	float current;  // �������
	float speed;  // ���ת�٣���λr/m(תÿ��)
	float location;  // ���ת�أ�����������ֵ
	int32_t motor_pwm;  // ���PWM�Ƚ�ֵ��
}Motor_Data_TypeDef;  // ����ṹ�������������ĸ�������
extern Motor_Data_TypeDef Motor_Data[4];

#define	Motor_Drive_Direction	0  // ���������ƣ�0:������1:������

#define ROTO_RATIO		44  // ����������*��ʱ����Ƶϵ������11 * 4 = 44(������תһȦ����ʱ����44����)
#define REDUCTION_RATIO	30  // ������ٱ�

#define Motor_ARR	(16800 - 1)  // TIM8���Զ���װ��ֵ(��ֵ����С��100 - 1)
#define Motor_PSC	(8 - 1)  // TIM8��Ԥ��Ƶϵ��

#define MOTOR0_DIRECTION 	PCout(2)  // Motor0�����
#define MOTOR1_DIRECTION 	PCout(3)  // Motor1�����
#define MOTOR2_DIRECTION 	PCout(4)  // Motor2�����
#define MOTOR3_DIRECTION 	PCout(5)  // Motor3�����
#define MOTOR0_CCR	TIM8->CCR1  // TIM8��CH1ͨ����Ԥװ��ֵ������Motor0
#define MOTOR1_CCR	TIM8->CCR2  // TIM8��CH2ͨ����Ԥװ��ֵ������Motor1
#define MOTOR2_CCR	TIM8->CCR3  // TIM8��CH3ͨ����Ԥװ��ֵ������Motor2
#define MOTOR3_CCR	TIM8->CCR4  // TIM8��CH4ͨ����Ԥװ��ֵ������Motor3

#define L1_Motor	Motor_Data[0]  // L1������嵽Motor[0]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[0]ΪCH1ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	R1_Motor	Motor_Data[1]  // R1������嵽Motor[1]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[1]ΪCH2ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	L2_Motor	Motor_Data[2]  // L2������嵽Motor[2]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[2]ΪCH3ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	R2_Motor	Motor_Data[3]  // R2������嵽Motor[3]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[3]ΪCH4ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�

void Motor_Init(void);
void Set_Motor(Motor_Data_TypeDef* motor_data);
void Set_MotorDutyCycle(Motor_Data_TypeDef* motor_data, float duty_cycle);
void Set_MotorPwm(Motor_Data_TypeDef* motor_data, float pwm);
int32_t Motor_InLimit( Motor_Data_TypeDef* motor_data, int32_t limit);
void motor_4(float duty_cycle1, float duty_cycle2, float duty_cycle3, float duty_cycle4);
void motor_2(float duty_cycle1, float duty_cycle2);
void step_motor_1(int vel, int acc, int time);

#endif

