#include "encoder.h"
#include "motor.h"

/**************************************************************************
�������ܣ�����������0��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void QEI0_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);  	//ʹ�� QEI0 ����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	//ʹ�� GPIOD ����
	
	//����PD7���ţ�Ҫ��������ʹ��ǰ�������޷�ʹ��
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;
	
	GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6|GPIO_PIN_7);	//����QEI��Χ�豸ʹ�õ����� PD6|PD7
	GPIOPinConfigure(GPIO_PD6_PHA0);	//����GPIO���ŵı��ù���
	GPIOPinConfigure(GPIO_PD7_PHB0);	//����GPIO���ŵı��ù���
	
	QEIConfigure(QEI0_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP), QEI_MAX_POSITION);	//��������������
	QEIEnable(QEI0_BASE);	//ʹ������������
	QEIPositionSet(QEI0_BASE, 0);	//���ñ�������ʼֵ
}

/**************************************************************************
�������ܣ�����������1��ʼ��
��ڲ�������
����  ֵ����
**************************************************************************/
void QEI1_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);  	//ʹ�� QEI0 ����
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	//ʹ�� GPIOD ����
	
	GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5|GPIO_PIN_6);	//����QEI��Χ�豸ʹ�õ����� PC5|PC6
	GPIOPinConfigure(GPIO_PC5_PHA1);	//����GPIO���ŵı��ù���
	GPIOPinConfigure(GPIO_PC6_PHB1);	//����GPIO���ŵı��ù���
	
	QEIConfigure(QEI1_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP), QEI_MAX_POSITION);	//��������������
	QEIEnable(QEI1_BASE);			//ʹ������������
	QEIPositionSet(QEI1_BASE, 0);	//���ñ�������ʼֵ
}

/**************************************************************************
�������ܣ��õ�����������ֵ���������ݰ�����Ӳ�����ӣ�Ӳ���ӣ����������׸��ģ�
��ڲ�����num�����������	L�����ֱ�����	R:���ֱ�����
����  ֵ��������������Լ���ֵ
**************************************************************************/
int32_t Get_QEI_Count(unsigned char num)
{
	int32_t count;
	
	if (num == L)	count = QEIPositionGet(QEI0_BASE);	//������λ�û�ȡ
	else	count = QEIPositionGet(QEI1_BASE);	//������λ�û�ȡ

#if	L_Change_Diretion
	if (num == L)	count = -count;
#endif

#if R_Change_Diretion
	if (num == R)	count = -count;
#endif
	
	return count;
}

/**************************************************************************
�������ܣ��õ�����ٶ�
��ڲ�����num�����������(L/R)��time��������ټ��(ms)
����  ֵ�����ת���ٶ�(תÿ��)	//������ý���һ����ʱ���ڲ����ٶȣ���Ȼ�ᵼ�²��ټ����׼ȷ
**************************************************************************/
float Get_Motor_Speed(unsigned char num, uint16_t time)
{
	int32_t count;
	int count_change;
	static int32_t L_Last_Location = 0, R_Last_Location = 0;
	float Speed;
	
	count = Get_QEI_Count(num);
	
	if (num == L)
	{
		count_change = count - L_Last_Location;
		L_Last_Location = count;
	}
	else
	{
		count_change = count - R_Last_Location;
		R_Last_Location = count;
	}
	
	/*�ٶȼ��㣺 �ٶ�(r/min) =  ((count_chage / ROTO_RATIO / REDUCTION_RATIO) / (time / 1000)) * 60 */
	Speed = (float)(60000 * count_change) / (float)(time * ROTO_RATIO * REDUCTION_RATIO);
	
	return Speed;
}
