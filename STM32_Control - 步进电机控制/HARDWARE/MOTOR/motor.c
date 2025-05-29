#include "motor.h"
#include "Emm_V5.h"

/**************************************************************************
�������ܣ������ʼ�����ã���PC6,PC7,PC7,PC9��ʼ��Ϊ����ڣ���TIM8��ʼ��ΪPWM���ģʽ��4·ͨ�����10KHzPWM��
��ڲ�������
����  ֵ����
**************************************************************************/
void Motor_Init(void)
{
	RCC->AHB1ENR |= 1<<2;  // ʹ��GPIOC
	GPIO_Set(GPIOC, PIN2|PIN3|PIN4|PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);  // ʹ��IO��
	GPIO_Set(GPIOC, PIN6|PIN7|PIN8|PIN9, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_PD);  // PWM�����
	GPIO_AF_Set(GPIOC, 6, 3);
	GPIO_AF_Set(GPIOC, 7, 3);
	GPIO_AF_Set(GPIOC, 8, 3);
	GPIO_AF_Set(GPIOC, 9, 3);
	
	RCC->APB2ENR |= 1<<1;  // TIM8ʹ��
	TIM8->ARR =	Motor_ARR;  // �������Խ��PWMƵ��Խ��
	TIM8->PSC = Motor_PSC;  // APB2ENR 168M,APB1ENR 84M
	TIM8->CR1 |= 0X01;
	TIM8->CR1 |= 1<<7;
	TIM8->BDTR |= 1<<15;  // �߼���ʱ�����PWM �����ʹ��
	//CH1
	TIM8->CCMR1 |= 6<<4;
	TIM8->CCMR1 |= 1<<3;
	TIM8->CCER |= 1<<0;
	TIM8->CCER |= 1<<1;
	//CH2
	TIM8->CCMR1 |= 6<<12;
	TIM8->CCMR1 |= 1<<11;
	TIM8->CCER |= 1<<4;
	TIM8->CCER |= 1<<5;
	//CH3
	TIM8->CCMR2 |= 6<<4;
	TIM8->CCMR2 |= 1<<3;
	TIM8->CCER |= 1<<8;
	TIM8->CCER |= 1<<9;
	//CH4
	TIM8->CCMR2 |= 6<<12;
	TIM8->CCMR2 |= 1<<11;
	TIM8->CCER |= 1<<12;
	TIM8->CCER |= 1<<13;
	
	//ʹ�����ʼ��ת
	Set_MotorDutyCycle(&Motor_Data[0], 0);
	Set_MotorDutyCycle(&Motor_Data[1], 0);
	Set_MotorDutyCycle(&Motor_Data[2], 0);
	Set_MotorDutyCycle(&Motor_Data[3], 0);
}

/*���������ݽṹ���������
  Motor_Data[0]��Ӧch1ͨ��
  Motor_Data[1]��Ӧch2ͨ��
  Motor_Data[2]��Ӧch3ͨ��
  Motor_Data[3]��Ӧch4ͨ��
  ��Ӳ��tim8ͨ����Ӧ���������׸���ch_number*/
Motor_Data_TypeDef	Motor_Data[4] = {{ 1, 0, 0, 0, 0, 0 },
									 { 2, 0, 0, 0, 0, 0 },
									 { 3, 0, 0, 0, 0, 0 },
									 { 4, 0, 0, 0, 0, 0 },};

/**************************************************************************
�������ܣ����õ���������ı����ķ����������pwm�Ƚ�ֵ
��ڲ�����������ݽṹ�����ָ�룬���а������Ӧ����ĸ��ֲ���
����  ֵ���ޣ����õײ�Ӳ�����������׸��ģ�
**************************************************************************/
void Set_Motor(Motor_Data_TypeDef* motor_data)
{
	if (motor_data->ch_number == 1)  // �����Ӧ�Ķ�ʱ��ͨ�����ж�
	{
		MOTOR0_DIRECTION = motor_data->direction;  // �������ŵ�ƽת��
		MOTOR0_CCR = motor_data->motor_pwm;  // ���pwmռ�ձȼ���
	}
	else if (motor_data->ch_number == 2)
	{
		MOTOR1_DIRECTION = motor_data->direction;
		MOTOR1_CCR = motor_data->motor_pwm;
	}
	else if (motor_data->ch_number == 3)
	{
		MOTOR2_DIRECTION = motor_data->direction;
		MOTOR2_CCR = motor_data->motor_pwm;
	}
	else if (motor_data->ch_number == 4)
	{
		MOTOR3_DIRECTION = motor_data->direction;
		MOTOR3_CCR = motor_data->motor_pwm;
	}
}

/**************************************************************************
�������ܣ����õ��pwmռ�ձȣ����׿��Ƶ��ת�٣�
��ڲ�����������ݽṹ�����ָ�루�ں�����������������pwmռ�ձ�
����  ֵ����
**************************************************************************/
void Set_MotorDutyCycle(Motor_Data_TypeDef* motor_data, float duty_cycle)
{
#if	Motor_Drive_Direction == 1
	duty_cycle = -duty_cycle;
#endif
	if (duty_cycle >= 0)
	{
		motor_data->direction = 0;
	}
	else
	{
		motor_data->direction = 1;
		duty_cycle = 99.99f + duty_cycle;
	}
	motor_data->motor_pwm = ((Motor_ARR + 1) / 100) * (99.99f - duty_cycle);  // ���pwm����
	
	Set_Motor(motor_data);  // ���õ��״̬������������ĵ�����򼰵��pwmռ�ձȴ�С����������������Ƶ���ĺ���
}

/**************************************************************************
�������ܣ����õ��pwm�Ƚ�ֵ����ȷ���Ƶ��ת�٣�
��ڲ�����������ݽṹ�����ָ�루�ں��������������pwm�Ƚ�ֵ
����  ֵ����
**************************************************************************/
void Set_MotorPwm(Motor_Data_TypeDef* motor_data, float pwm)
{
#if	Motor_Drive_Direction == 1
	pwm = -pwm;
#endif
	if (pwm >= 0)
	{
		motor_data->direction = 0;
	}
	else
	{
		motor_data->direction = 1;
		pwm = Motor_ARR + pwm;
	}
	motor_data->motor_pwm = Motor_ARR - pwm;  // ���pwm����
	
	Set_Motor(motor_data);  // ���õ��״̬������������ĵ�����򼰵��pwmռ�ձȴ�С����������������Ƶ���ĺ���
}

/**************************************************************************
�������ܣ��������PWM�Ƚ�ֵ�޷�
��ڲ�����������ݽṹ�����ָ�루�ں��������������pwm�Ƚ�ֵ��ֵ(-pwm~pwm)
����  ֵ�������ƺ��pwm�Ƚ�ֵ
**************************************************************************/
int32_t Motor_InLimit(Motor_Data_TypeDef* motor_data, int32_t limit)
{
	if ( motor_data->motor_pwm > limit)
	{
		motor_data->motor_pwm = limit;
	}
	else if (motor_data->motor_pwm < -limit)
	{
		motor_data->motor_pwm = -limit;  // ����ֵ
	}
	return motor_data->motor_pwm;
}

/**************************************************************************
�������ܣ�4·������׿���
��ڲ�����4·�����pwmռ�ձ�
����  ֵ����
**************************************************************************/
void motor_4(float duty_cycle1, float duty_cycle2, float duty_cycle3, float duty_cycle4)
{
    Set_MotorDutyCycle(&L1_Motor, duty_cycle1);  // L1
    Set_MotorDutyCycle(&R1_Motor, duty_cycle2);  // R1
    Set_MotorDutyCycle(&L2_Motor, duty_cycle3);  // L2
    Set_MotorDutyCycle(&R2_Motor, duty_cycle4);  // R2
}

/**************************************************************************
�������ܣ�2·������׿���
��ڲ�����2·�����pwmռ�ձ�
����  ֵ����
**************************************************************************/
void motor_2(float duty_cycle1, float duty_cycle2)
{
    Set_MotorDutyCycle(&L1_Motor, duty_cycle1);  // L1
    Set_MotorDutyCycle(&R1_Motor, duty_cycle2);  // R1
}

/**************************************************************************
�������ܣ�1·����������׿���
��ڲ�����1·�����������תȦ��
����  ֵ����
**************************************************************************/

void step_motor_1(int vel, int acc, int time)
{
	if(vel>=0)
	{
		Emm_V5_Vel_Control(1,1,vel,acc,0);  //������ʱ�뷽��
		delay_us(time);
	}
	else
	{
		Emm_V5_Vel_Control(1,0,vel,acc,0);  //����˳ʱ�뷽��
		delay_us(time);
	}
   
}
