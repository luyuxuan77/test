#include "track.h"
#include "motor.h"
#include "uart1.h"
#include "uart0.h"
#include "pid.h"

float sensor_weights[NUM_SENSORS] = {3.4, 2.5, 1, 0, -1, -2.5, -3.4};	//灯的位权数组
float sensor_weights_inner[NUM_SENSORS] = {2.3, 1.8, 1, 0, -1, -8.6, -12};	//灯的位权数组

void Line7_Init(void)
{
    // 开启 GPIO 外设
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // 配置 PD2 和 PD3 为输入（移除 PD0 和 PD1）
    GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    
    // 配置 PB2 和 PB3 为输入（保持不变）
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    
    // 配置 PE2、PE4 和 PE5 为输入（新增 PE4 和 PE5）
    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
}

Grayscale_TypeDef Grayscale_Data = {1, 0, 0};
// 定义卡尔曼滤波器变量
KalmanFilter grayscale_kalman = {0, 1, 0};

void KalmanFilter_Init(KalmanFilter* kf) {
    kf->X = 0;  // 初始估计值
    kf->P = 1;  // 初始估计误差
    kf->K = 0;  // 初始卡尔曼增益
}

float KalmanFilter_Update(KalmanFilter* kf, float measurement) {
    // 预测更新
    kf->P += 0.1;  // 假设过程噪声 Q = 0.1

    // 测量更新
    kf->K = kf->P / (kf->P + 0.5);  // 假设测量噪声 R = 0.5
    kf->X += kf->K * (measurement - kf->X);
    kf->P *= (1 - kf->K);

    return kf->X;
}
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
    Grayscale_Data.grayscale_offset = KalmanFilter_Update(&grayscale_kalman, position_error);
//	printf("                                            %d\r\n",Grayscale_Data.grayscale_offset);
	return Grayscale_Data.grayscale_offset;
}

int Get_Grayscale_Offset1(void)
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
			position_error += sensor_weights_inner[i];
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
    Grayscale_Data.grayscale_offset = KalmanFilter_Update(&grayscale_kalman, position_error);
//	printf("                                            %d\r\n",Grayscale_Data.grayscale_offset);
	return Grayscale_Data.grayscale_offset;
}

//定义灰度传感器方向环pid参数结构体变量和p，i，d三值结构体变量数组
PID_TypeDef Grayscale_Direction_Pid;/*!pid算法!中用到的参数集合*/
p_i_d_Value_TypeDef PID_Value_Grayscale_Direction[3] = {{24, 0.0, 2.5 },	//预存三组pid参数，用于不同的巡线速度
														{ 34.5, 0.0, 2.5},/*比例，积分，微分3值大小结构体*/
														{ 0.0, 0.0, 0.0 }};
/******************************************************************************
函数功能：灰度传感器做循迹方向环（巡线速度改变时，p，i，d参数的值也要改变）
入口参数：speed:巡线速度
返回  值：无，改变电机速度环目标值
*******************************************************************************/ 
void Track_Direction_Control(float speed)
{
//	printf("2");
	Get_Grayscale_Offset();
//	printf("%d\r\n",Grayscale_Data.grayscale_offset);
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
//	printf("ActualValue=%.2f\r\n",Grayscale_Direction_Pid.ActualValue);
	Pid_OutLimit(&Grayscale_Direction_Pid, 40);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue;
//	L1_Speed_Pid.SetPoint = 250;
//	R1_Speed_Pid.SetPoint = 250;
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}







void Track_Direction_Control1(float speed)
{
//	printf("2");
	Get_Grayscale_Offset();
//	printf("%d\r\n",Grayscale_Data.grayscale_offset);
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
//	printf("ActualValue=%.2f\r\n",Grayscale_Direction_Pid.ActualValue);
	Pid_OutLimit(&Grayscale_Direction_Pid, 50);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue*1.22;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue*1.22 ;
//	L1_Speed_Pid.SetPoint = 250;
//	R1_Speed_Pid.SetPoint = 250;
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}
void Track_Direction_Control2(float speed)
{
//	printf("2");
	Get_Grayscale_Offset1();
//	printf("%d\r\n",Grayscale_Data.grayscale_offset);
	Pid_control(&Grayscale_Direction_Pid, Grayscale_Data.grayscale_offset, 0);
//	printf("ActualValue=%.2f\r\n",Grayscale_Direction_Pid.ActualValue);
	Pid_OutLimit(&Grayscale_Direction_Pid, 50);
	L1_Speed_Pid.SetPoint = speed + Grayscale_Direction_Pid.ActualValue*1.22;
	R1_Speed_Pid.SetPoint = speed - Grayscale_Direction_Pid.ActualValue*1.22 ;
//	L1_Speed_Pid.SetPoint = 250;
//	R1_Speed_Pid.SetPoint = 250;
//	L2_Speed_Pid.SetPoint = speed /*- Grayscale_Direction_Pid.ActualValue*/;
//	R2_Speed_Pid.SetPoint = speed /*+ Grayscale_Direction_Pid.ActualValue*/;
}

UWB_TypeDef UWB_Data = {1, 0};
PID_TypeDef UWB_Angle_Pid;/*!pid算法!中用到的参数集合*/
PID_TypeDef UWB_Distance_Pid;/*!pid算法!中用到的参数集合*/

//定义灰度传感器方向环pid参数结构体变量和p，i，d三值结构体变量数组
p_i_d_Value_TypeDef PID_Value_UWB_Angle[1] = {{ 3, 0.0, 0 },	//预存三组pid参数，用于不同的巡线速度/*比例，积分，微分3值大小结构体*/
											 };
p_i_d_Value_TypeDef PID_Value_UWB_Distance[1] = {{ 1, 0.0, 0 },	//预存三组pid参数，用于不同的巡线速度/*比例，积分，微分3值大小结构体*/
											 };
float UWB_Current_Angle = 60;
float UWB_Current_Distance = 100;
/**************************************************************************
函数功能：得到UWB角度差
入口参数：无
返回  值：UWB_Data.angle_offset(float形)标签与我的角度差
**************************************************************************/
float Get_Angle_Offset(void)
{
	UWB_Data.angle_offset = UWB_Angle_Pid.SetPoint - UWB_Current_Angle;
	return UWB_Data.angle_offset;
}

/**************************************************************************
函数功能：得到UWB距离差
入口参数：无
返回  值：UWB_Data.angle_offset(float形)标签与我的距离差
**************************************************************************/
float Get_Distance_Offset(void)
{
	UWB_Data.distance_offset = UWB_Current_Distance - UWB_Distance_Pid.SetPoint;
	return UWB_Data.distance_offset;
}

/******************************************************************************
函数功能：UWB传感器做追踪功能
入口参数：
返回  值：
*******************************************************************************/ 
//void Track_Angle_Distance_Control(void)
//{
//	Get_Angle_Offset();
//	Get_Distance_Offset();

//	Pid_control(&UWB_Angle_Pid, UWB_Current_Angle, 0);
//	Pid_control(&UWB_Distance_Pid, UWB_Current_Distance, 0);
//	Pid_OutLimit(&UWB_Angle_Pid, 50);
//	Pid_OutLimit(&UWB_Distance_Pid, 50);
////	printf("Dis: %f	   disc: %f   Ang: %f   angc: %f\r\n",UWB_Distance_Pid.ActualValue,UWB_Current_Distance,UWB_Angle_Pid.ActualValue,UWB_Current_Angle);
////	L1_Speed_Pid.SetPoint = UWB_Distance_Pid.ActualValue - UWB_Angle_Pid.ActualValue;
////	R1_Speed_Pid.SetPoint = UWB_Distance_Pid.ActualValue + UWB_Angle_Pid.ActualValue;

////	motor_4(-UWB_Distance_Pid.ActualValue - UWB_Angle_Pid.ActualValue,    -UWB_Distance_Pid.ActualValue + UWB_Angle_Pid.ActualValue,0,0);
//}

