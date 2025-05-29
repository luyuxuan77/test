#include "gray.h"
#include "bsp.h"

float sensor_weights[NUM_SENSORS] = {-3, -2, -1, 0, 1, 2, 3};	//灯的位权数组

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
函数功能：得到七路灰度偏移量(模拟)
入口参数：无
返回  值：无模拟量赋值给Grayscale_Data.grayscale_offset
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

//定义灰度传感器方向环pid参数结构体变量和p，i，d三值结构体变量数组
PID_TypeDef Grayscale_Direction_Pid;/*!pid算法!中用到的参数集合*/
p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3] = {{ 70, 0.0, 0.0 },	//预存三组pid参数，用于不同的巡线速度
														{ 80, 0.0, 0.0 },/*比例，积分，微分3值大小结构体*/
														{ 200, 0.0, 0.0 }}  ;


/******************************************************************************
函数功能：灰度传感器做循迹方向环（巡线速度改变时，p，i，d参数的值也要改变）
入口参数：speed:巡线速度
返回  值：无，改变电机速度环目标值
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

