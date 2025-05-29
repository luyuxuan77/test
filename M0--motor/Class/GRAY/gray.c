#include "gray.h"
#include "bsp.h"

float sensor_weights[NUM_SENSORS] = {-3, -2, -1, 0, 1, 2, 3};	//�Ƶ�λȨ����

void gray_init(void)
{
    DL_GPIO_initDigitalInputFeatures(Gray_P0_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(Gray_P1_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(Gray_P2_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(Gray_P3_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(Gray_P4_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(Gray_P5_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
    DL_GPIO_initDigitalInputFeatures(Gray_P6_IOMUX,
                                     DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                     DL_GPIO_HYSTERESIS_DISABLE, DL_GPIO_WAKEUP_DISABLE);
}


uint8_t gray_read(void)
{
	uint8_t val=0;
	if(DL_GPIO_readPins(Gray_P6_PORT,Gray_P6_PIN)) val |=1<<6;
	else                                           val &=0<<6;
	if(DL_GPIO_readPins(Gray_P5_PORT,Gray_P5_PIN)) val |=1<<5;
	else                                           val &=0<<5;
	if(DL_GPIO_readPins(Gray_P4_PORT,Gray_P4_PIN)) val |=1<<4;
	else                                           val &=0<<4;
	if(DL_GPIO_readPins(Gray_P3_PORT,Gray_P3_PIN)) val |=1<<3;
	else                                           val &=0<<3;
	if(DL_GPIO_readPins(Gray_P2_PORT,Gray_P2_PIN)) val |=1<<2;
	else                                           val &=0<<2;
	if(DL_GPIO_readPins(Gray_P1_PORT,Gray_P1_PIN)) val |=1<<1;
	else                                           val &=0<<1;
	if(DL_GPIO_readPins(Gray_P0_PORT,Gray_P0_PIN)) val |=1<<0;
	else                                           val &=0<<0;
	return val;
}
Grayscale_TypeDef Grayscale_Data = {1, 0, 0};



/**************************************************************************
�������ܣ��õ���·�Ҷ�ƫ����(ģ��)
��ڲ�������
����  ֵ����ģ������ֵ��Grayscale_Data.grayscale_offset
**************************************************************************/
int Get_Grayscale_Offset(void)
{
	float position_error = 0;
	int active_sensors = 0;
	int i = 0;
	Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
	
	Grayscale_Data.grayscale_state = 1;
	for (i = 0; i < NUM_SENSORS; i++)
	{
		if(Grayscale_Data.grayscale_digital & (1 << i))
		{
			position_error += sensor_weights[i];
			active_sensors++;
		}
	}
	if(active_sensors != 0)
	{
		position_error /= active_sensors;
	}
	else
	{
		Grayscale_Data.grayscale_state = 0;
		return Grayscale_Data.grayscale_offset;
	}
	Grayscale_Data.grayscale_offset = position_error;
	return Grayscale_Data.grayscale_offset;
}

//����Ҷȴ���������pid�����ṹ�������p��i��d��ֵ�ṹ���������
PID_TypeDef Grayscale_Direction_Pid;/*!pid�㷨!���õ��Ĳ�������*/
p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3] = {{ 70, 0.0, 0.0 },	//Ԥ������pid���������ڲ�ͬ��Ѳ���ٶ�
														{ 80, 0.0, 0.0 },/*���������֣�΢��3ֵ��С�ṹ��*/
														{ 200, 0.0, 0.0 }}  ;


/******************************************************************************
�������ܣ��Ҷȴ�������ѭ�����򻷣�Ѳ���ٶȸı�ʱ��p��i��d������ֵҲҪ�ı䣩
��ڲ�����speed:Ѳ���ٶ�
����  ֵ���ޣ��ı����ٶȻ�Ŀ��ֵ
*******************************************************************************/
void Track_Direction_Control(float speed)
{
	Get_Grayscale_Offset();
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
	Pid_OutLimit(&Grayscale_Direction_Pid, 100);//130

	L1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue;
	L2_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue;
	R2_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue;
}

