#include "ccd.h"
#include "rgb.h"
#include "motor.h"
#include "my_adc.h"
#include "mymath.h"
#include "control.h"

/******************************************************************************
函数功能：线性CCD模块初始化，使用ADC0(PE3)，PB2|PB3
入口参数：无
返回  值：无
*******************************************************************************/ 
void CCD_Init(void)
{
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);  //使能 GPIOB 外设
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);  //使能PB2|PB3输出引脚
	MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);	//推挽+下拉
	ADC0_Init();
}

//定义CCD模块数据结构体
CCD_TypeDef CCD_Data;
/******************************************************************************
函数功能：发送给线性CCD固定信号触发采集，并发送128个时钟信号开始采集并存储采集原始值
入口参数：无
返回  值：无
*******************************************************************************/ 
void Get_CCD_Value(void)
{
	unsigned char i;
	
	CCD_CLK_DOWN;
	CCD_SI_DOWN;
	Dly_us();
	
	CCD_CLK_DOWN;
	CCD_SI_UP;
	Dly_us();

	CCD_CLK_UP;
	Dly_us();
	
	CCD_SI_DOWN;
	Dly_us();
	
	for (i = 0; i < 128; i++)
	{
		CCD_CLK_DOWN;
		Dly_us();
		
		CCD_Data.receive_data[i] = ADC0_Get() >> 4;
		
		CCD_CLK_UP;
		Dly_us();
	}
	CCD_Data.receive_flag = 1;
}

/******************************************************************************
函数功能：产生us级延时
入口参数：无
返回  值：无
*******************************************************************************/ 
void Dly_us(void)
{
	int ii;    
	for(ii=0;ii<10;ii++);
}

/******************************************************************************
函数功能：发送数组的值到串口
入口参数：mdoe:串口信息类别，0:发送ADC采集的原始值，1:发送数据二值化后的数组值(确保有进行过二值化处理)
返回  值：无
*******************************************************************************/ 
void Send_To_Serial_Port(unsigned char mode)
{
	unsigned char i;
	if (CCD_Data.receive_flag == 1)
	{
		UARTprintf("{");
		for(i=0;i<128;i++)
		{
			if (mode) UARTprintf("%d,",CCD_Data.binaryzation_data[i]);
			else UARTprintf("%d,",CCD_Data.receive_data[i]);
		}
		UARTprintf("}");
		UARTprintf("\r\n");UARTprintf("\r\n");UARTprintf("\r\n");
		
		CCD_Data.receive_flag = 0;
	}
}

/******************************************************************************
函数功能：将待发送的信息通过串口发送至上位机
入口参数：mode：0发送采集到的原始数据，1发送经处理过后的数据
返回  值：无
*******************************************************************************/ 
void Send_To_Debug(unsigned char mode)
{ 
	int i;
	if (CCD_Data.receive_flag == 1)
	{
		UARTprintf("*");
		UARTprintf("LD");
		for(i = 2; i < 134; i++)
		{
			if (mode)
			{
				UARTprintf("%c",binToHex_high(CCD_Data.process_data[i])); //以字符形式发送高4位对应的16进制
				UARTprintf("%c",binToHex_low(CCD_Data.process_data[i]));  //以字符形式发送低?位对应的16进制
			}
			else
			{
				UARTprintf("%c",binToHex_high(CCD_Data.receive_data[i])); //以字符形式发送高4位对应的16进制
				UARTprintf("%c",binToHex_low(CCD_Data.receive_data[i]));  //以字符形式发送低?位对应的16进制
			}
		}
		UARTprintf("00");   //通信协议要求
		UARTprintf("#");    //通信协议要求
		CCD_Data.receive_flag = 0;
	}
}

/******************************************************************************
函数功能：将二进制低8位转换16进制*
入口参数：u8
返回  值：char
*******************************************************************************/ 
char binToHex_low(unsigned char num)
{
	unsigned char low_four;
	low_four=num&0x0f;
	if(low_four==0)
		return('0');
	else if(low_four==1)
		return('1');
	else if(low_four==2)
		return('2');
	else if(low_four==3)
		return('3');
	else if(low_four==4)
		return('4');
	else if(low_four==5)
		return('5');
	else if(low_four==6)
		return('6');
	else if(low_four==7)
		return('7');
	else if(low_four==8)
		return('8');
	else if(low_four==9)
		return('9');
	else if(low_four==10)
		return('A');
	else if(low_four==11)
		return('B');
	else if(low_four==12)
		return('C');
	else if(low_four==13)
		return('D');
	else if(low_four==14)
		return('E');
	else if(low_four==15)
		return('F');
	else
		return 0;
}

/******************************************************************************
函数功能：将二进制高8位转换16进制*
入口参数：u8
返回  值：char
*******************************************************************************/ 	
char binToHex_high(unsigned char num)
{
	unsigned char high_four;
	high_four=(num>>4)&0x0f;
	if(high_four==0)
		return('0');
	else if(high_four==1)
		return('1');
	else if(high_four==2)
		return('2');
	else if(high_four==3)
		return('3');
	else if(high_four==4)
		return('4');
	else if(high_four==5)
		return('5');
	else if(high_four==6)
		return('6');
	else if(high_four==7)
		return('7');
	else if(high_four==8)
		return('8');
	else if(high_four==9)
		return('9');
	else if(high_four==10)
		return('A');
	else if(high_four==11)
		return('B');
	else if(high_four==12)
		return('C');
	else if(high_four==13)
		return('D');
	else if(high_four==14)
		return('E');
	else if(high_four==15)
		return('F');
	else
		return 0;
}

/******************************************************************************
函数功能：得到当前环境区分黑白色的阈值
入口参数：无
返回  值：无
*******************************************************************************/ 
void Get_CCD_Threshold_Value(void)
{
	unsigned char i;
	unsigned char min, max;
	
	Get_CCD_Value();
	SysCtlDelay(((SysCtlClockGet() / 3000) * 30) - 1);	//30ms延时
	
	Get_CCD_Value();
	SysCtlDelay(((SysCtlClockGet() / 3000) * 30) - 1);	//30ms延时
	
	max = CCD_Data.receive_data[0];
	min = CCD_Data.receive_data[0];
	for (i = 0; i < 128; i++)
	{
		if (CCD_Data.receive_data[i] > max)	max = CCD_Data.receive_data[i];
		else if (CCD_Data.receive_data[i] < min)	min = CCD_Data.receive_data[i];
	}
	CCD_Data.threshold_value = (max + min) / 2;
	//UARTprintf("CCD_Data.threshold_value = %d\r\n\r\n",CCD_Data.threshold_value);
}

/******************************************************************************
函数功能：仅使用线性ccd中间100位数据做判断，得到黑线的左边沿和右边沿位置（输入环境阈值）
入口参数：threshold_value:环境阈值(区分黑白背景)
返回  值：无
*******************************************************************************/ 
void Get_Edge_StaticThreshold(unsigned char threshold_value)
{
	unsigned char i;
	
	for (i = 15; i < 115; i++)
	{
		if (CCD_Data.receive_data[i] < threshold_value && CCD_Data.receive_data[i + 1] < threshold_value && CCD_Data.receive_data[i + 2] < threshold_value
			&& CCD_Data.receive_data[i + 3] < threshold_value && CCD_Data.receive_data[i + 4] < threshold_value)
		{
			CCD_Data.left_edge_location = i - 15;
			break;
		}
	}
	for (i = 114; i > 14; i--)
	{
		if (CCD_Data.receive_data[i] < threshold_value && CCD_Data.receive_data[i - 1] < threshold_value && CCD_Data.receive_data[i - 2] < threshold_value
			&& CCD_Data.receive_data[i - 3] < threshold_value && CCD_Data.receive_data[i - 4] < threshold_value)
		{
			CCD_Data.right_edge_location = i - 15;
			break;
		}
	}
}

/******************************************************************************
函数功能：仅使用线性ccd中间100位数据做判断，得到黑线的左边沿和右边沿位置（动态阈值）
入口参数：无
返回  值：无
*******************************************************************************/ 
void Get_Edge_DynamicThreshold(void)
{
	unsigned char i, j;
	unsigned char min, min_j, max, max_j;
	unsigned char last_data;
	unsigned char middle_value;
	
	//对测得的数据进行一阶低通滤波(让测得的曲线变平滑,视情况使用)并储存到process_data数组中，之后对数组的处理都基于数组process_data
	last_data = CCD_Data.receive_data[0];
	for (i = 0; i < 100; i ++)
	{
		CCD_Data.process_data[i] = CCD_Data.receive_data[i + 15] * 1.0f + last_data * 0.0f;
		last_data = CCD_Data.receive_data[i + 15];
	}
	
	//去掉5个最大值和5个最小值
	for (i = 0; i < 5; i++)
	{
		min = CCD_Data.process_data[0];
		max = CCD_Data.process_data[0];
		for (j = 0; j < 100; j++)
		{
			if (CCD_Data.process_data[j] > max)
			{
				max = CCD_Data.process_data[j];
				max_j = j;
			}
			else if (CCD_Data.process_data[j] < min)
			{
				min = CCD_Data.process_data[j];
				min_j = j;
			}
		}
		CCD_Data.process_data[max_j] = CCD_Data.process_data[max_j + 1];
		CCD_Data.process_data[min_j] = CCD_Data.process_data[min_j + 1];
	}
	
	//计算数据最大值与最小值的中值，作为阈值使用
	min = CCD_Data.process_data[0];
	max = CCD_Data.process_data[0];
	for (i = 0; i < 100; i++)
	{
		if (CCD_Data.process_data[i] > max)	max = CCD_Data.process_data[i];
		else if (CCD_Data.process_data[i] < min)	min = CCD_Data.process_data[i];
	}
	middle_value = (max + min) / 2;
	
	//数据二值化处理，二值化后数据存储到CCD_Data.binaryzation_data
	for (i = 0; i < 100; i++)
	{
		if (CCD_Data.process_data[i] > middle_value)	CCD_Data.binaryzation_data[i] = 1;
		else CCD_Data.binaryzation_data[i] = 0;
	}
	
	//去除数据毛刺
	for (i = 0; i < 95; i++)
	{
		if (CCD_Data.binaryzation_data[i] == 0)
		{
			if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i + 1] == 0 && CCD_Data.binaryzation_data[i + 2] == 0
				&& CCD_Data.binaryzation_data[i + 3] == 0);
			else
			{
				CCD_Data.binaryzation_data[i] = 1;
				CCD_Data.binaryzation_data[i + 1] = 1;
				CCD_Data.binaryzation_data[i + 2] = 1;
				CCD_Data.binaryzation_data[i + 3] = 1;
				i = i + 5;
			}
		}
	}
	
	//得到左侧跳变沿和右侧跳变沿
	for (i = 0; i < 100; i++)
	{
		if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i + 1] == 0 && CCD_Data.binaryzation_data[i + 2] == 0
			&& CCD_Data.binaryzation_data[i + 3] == 0 && CCD_Data.binaryzation_data[i + 4] == 0)
		{
			CCD_Data.left_edge_location = i;
			break;
		}
	}
	for (i = 99; i > 0; i--)
	{
		if (CCD_Data.binaryzation_data[i] == 0 && CCD_Data.binaryzation_data[i - 1] == 0 && CCD_Data.binaryzation_data[i - 2] == 0
			&& CCD_Data.binaryzation_data[i - 3] == 0 && CCD_Data.binaryzation_data[i - 4] == 0)
		{
			CCD_Data.right_edge_location = i;
			break;
		}
	}
}
//定义黑线数据结构体变量
Line_TypeDef Line_Data;

/******************************************************************************
函数功能：黑线数据初始化函数
入口参数：无
返回  值：无
*******************************************************************************/ 
void Line_Data_Init(void)
{
	Line_Data.last_line_location = 50;
	Line_Data.last_line_width = 20;
	Line_Data.last_line_right = 65;
	Line_Data.last_line_left = 35;
}

/******************************************************************************
函数功能：跳变沿处理函数，可根据实际需求自行调整内容
入口参数：mode黑线偏差值以左交界线/右交界线/中线为基准，0:中线，1:左线，2:右线
返回  值：无		得到黑线偏差值
*******************************************************************************/ 
void Edge_Dispose(unsigned char mode)
{
	static unsigned char state = 0;
	//左跳变沿与右跳变沿差值大于80，表示检测到十字或T字路口
	//if (CCD_Data.right_edge_location - CCD_Data.left_edge_location > 80)	Automobile_Data_Recorder.Car_State_Flag = Stop_And_Switch_State;
	//if (CCD_Data.right_edge_location > 85 && (CCD_Data.right_edge_location - CCD_Data.left_edge_location) > 40 && (CCD_Data.right_edge_location - CCD_Data.left_edge_location) < 80)	Automobile_Data_Recorder.Car_State_Flag = Stop_And_Switch_State;
	
	//左跳变沿向左突变，则沿用上次左线位置；右跳变沿向右突变，则延用上次悠闲位置；	防止赛道上的污渍干扰
	if (Line_Data.last_line_left - CCD_Data.left_edge_location > 20)	Line_Data.line_left = Line_Data.last_line_left;
	else Line_Data.line_left = CCD_Data.left_edge_location;
	if (CCD_Data.right_edge_location - Line_Data.last_line_right > 20)	Line_Data.line_right = Line_Data.last_line_right;
	else Line_Data.line_right = CCD_Data.right_edge_location;
	
	//线的位置以左交界线/右交界线/中线为基准做加减	用于巡左环线与右环线
	if (mode == 0)	Line_Data.line_location = (Line_Data.line_left + Line_Data.line_right) / 2;
	else if (mode == 1)	Line_Data.line_location = Line_Data.line_left + 10;
	else if (mode == 2)	Line_Data.line_location = Line_Data.line_right - 10;
	
	//线的位置突变，则延用上一次线的位置	滤除不正确的数据
	if (Get_Absolute_Value(Line_Data.line_location - Line_Data.last_line_location) > 30)	Line_Data.line_location = Line_Data.last_line_location;
	
	//线的宽度突变，则延用上一次线的位置	滤除不正确的数据
	Line_Data.line_width = Line_Data.line_right - Line_Data.line_left;
	if (Line_Data.line_width - Line_Data.last_line_width > 20)	Line_Data.line_location = Line_Data.last_line_location;
	
	//左跳变沿与右跳变沿同时扩宽，且线宽大于某值，则识别为检测到终点，LED灯改变状态
	if (Line_Data.last_line_left - CCD_Data.left_edge_location > 12 && CCD_Data.right_edge_location - Line_Data.last_line_right > 12 && Line_Data.line_width > 20)
	{
		state = !state;
		Grren_Light_Control(state);
	}
	
	//上次数据赋值
	Line_Data.last_line_location = Line_Data.line_location;
	Line_Data.last_line_width = Line_Data.line_width;
	Line_Data.last_line_left = Line_Data.line_left;
	Line_Data.last_line_right = Line_Data.line_right;
	
	//线的偏差值计算
	Line_Data.line_offset = Line_Data.line_location - 50;
}

//定义CCD的pid参数结构体变量和p，i，d三值结构体变量数组
PID_TypeDef CCD_Pid;
p_i_d_Value_TypeDef PID_Value_CCD[3] = {{45.0, 0.0, 0.0},	//预存三组pid参数，用于不同的巡线速度；
										{70.0, 0.0, 10.0},
										{150.0, 0.0, 0.0}};
/******************************************************************************
函数功能：线性CCD的pid循迹（巡线速度改变时，p，i，d参数的值也要改变）
入口参数：pwm:pwm比较值，控制电机巡线速度
返回  值：无，改变输出给电机的pwm比较值
*******************************************************************************/ 
void CCD_Track_Pid(uint16_t pwm)
{
	//Get_Edge_DynamicThreshold();
	Get_Edge_StaticThreshold(CCD_Data.threshold_value);
	Edge_Dispose(2);
	
	Pid_control(&CCD_Pid, Line_Data.line_offset, 0);
	Pid_OutLimit(&CCD_Pid, 5000);
	
	L1_Motor.motor_pwm = pwm - CCD_Pid.ActualValue;
	R1_Motor.motor_pwm = pwm + CCD_Pid.ActualValue;
//	Motor_InLimit( &L1_Motor, 9000);
//	Motor_InLimit( &R1_Motor, 9000);
	
//	Set_MotorPwm(&L1_Motor,L1_Motor.motor_pwm);
//	Set_MotorPwm(&R1_Motor,R1_Motor.motor_pwm);
}

//定义CCD做方向环的pid参数结构体变量和p，i，d三值结构体变量数组
PID_TypeDef CCD_Direction_Pid;
p_i_d_Value_TypeDef PID_Value_CCD_Direction[3] = {{2.0, 0.0, 0.18},	//预存三组pid参数，用于不同的巡线速度；
												  {2.3, 0.0, 0.5},
												  {0.0, 0.0, 0.0}};
/******************************************************************************
函数功能：线性CCD做循迹方向环（巡线速度改变时，p，i，d参数的值也要改变）
入口参数：speed:巡线速度
返回  值：无，改变电机速度环目标值
*******************************************************************************/ 
void CCD_Track_Direction_Control(float speed)
{
	Get_CCD_Value();
	Get_Edge_DynamicThreshold();
	//Get_Edge_StaticThreshold(CCD_Data.threshold_value);
	Edge_Dispose(1);

	Pid_control(&CCD_Direction_Pid, Line_Data.line_offset, 0);
	Pid_OutLimit(&CCD_Direction_Pid, 150);
	
	L1_Speed_Pid.SetPoint = speed - CCD_Direction_Pid.ActualValue;
	R1_Speed_Pid.SetPoint = speed + CCD_Direction_Pid.ActualValue;
}

