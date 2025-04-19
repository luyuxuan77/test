#ifndef	MOTOR_H
#define	MOTOR_H
#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"

//������ݽṹ�壬�ں����������
typedef struct
{
	unsigned char pwm_outch;	//PWM0ͨ����
	unsigned char direction;	//�����ת����0:��ת��1:��ת
	float current;				//�������
	float speed;				//���ת�٣���λr/m(תÿ��)
	float location;				//���ת�أ�����������ֵ
	int32_t motor_pwm;			//���PWM�Ƚ�ֵ��
}Motor_Data_TypeDef;			//����ṹ�������������ĸ�������
extern Motor_Data_TypeDef Motor_Data[4];

#define ROTO_RATIO		44	//����������*��ʱ����Ƶϵ������11 * 4 = 44(������תһȦ����ʱ����44����)
#define REDUCTION_RATIO	30	//������ٱ�

#define	MOTOR_PWMARR	(10000 - 1)	//PWM���Զ���װ��ֵ
#define MOTOR_PWMCLOCK	80000000	//PWMʱ��

#define MOTOR0_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2)	//motor0���������
#define MOTOR0_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0)			//motor0���������
#define MOTOR1_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3)	//motor1���������
#define MOTOR1_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0)			//motor1���������
#define MOTOR2_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4)	//motor2���������
#define MOTOR2_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0)			//motor2���������
#define MOTOR3_DIRECTION_UP		MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5)	//motor3���������
#define MOTOR3_DIRECTION_DOWN	MAP_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0)			//motor3���������

#define	MOTOR0_PWMOUTCH		PWM0_BASE, PWM_OUT_0	//motor0��pwm���ͨ��
#define	MOTOR1_PWMOUTCH		PWM0_BASE, PWM_OUT_1	//motor1��pwm���ͨ��
#define	MOTOR2_PWMOUTCH		PWM0_BASE, PWM_OUT_2	//motor2��pwm���ͨ��
#define	MOTOR3_PWMOUTCH		PWM0_BASE, PWM_OUT_3	//motor3��pwm���ͨ��

#define L1_Motor	Motor_Data[0]	//L1������嵽Motor[0]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[0]ΪPWM0_OUT0ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	R1_Motor	Motor_Data[1]	//R1������嵽Motor[1]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[1]ΪPWM0_OUT1ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	L2_Motor	Motor_Data[2]	//L2������嵽Motor[2]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[2]ΪPWM0_OUT2ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�
#define	R2_Motor	Motor_Data[3]	//R2������嵽Motor[3]�ṹ�壨�����ӣ��ɶ�����޸ģ���Motor_Data[3]ΪPWM0_OUT3ͨ����Ӳ���ӣ��Ǳ�Ҫ���޸ģ�

void Motor_Init(void);
void Set_Motor(Motor_Data_TypeDef* motor_data);
void Set_MotorDutyCycle(Motor_Data_TypeDef* motor_data, float duty_cycle);
void Set_MotorPwm(Motor_Data_TypeDef* motor_data, float pwm);
int32_t Motor_InLimit( Motor_Data_TypeDef* motor_data, int32_t limit);
void motor_4_speed(float duty_cycle1, float duty_cycle2, float duty_cycle3, float duty_cycle4);
void motor_2(float duty_cycle1, float duty_cycle2);
#endif

