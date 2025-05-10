#include "encoder.h"
#include "motor.h"

/**************************************************************************
函数功能：正交编码器0初始化
入口参数：无
返回  值：无
**************************************************************************/
void QEI0_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);  	//使能 QEI0 外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	//使能 GPIOD 外设
	
	//解锁PD7引脚，要放在外设使用前，否则无法使用
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0x00;
	
	GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6|GPIO_PIN_7);	//配置QEI外围设备使用的引脚 PD6|PD7
	GPIOPinConfigure(GPIO_PD6_PHA0);	//配置GPIO引脚的备用功能
	GPIOPinConfigure(GPIO_PD7_PHB0);	//配置GPIO引脚的备用功能
	
	QEIConfigure(QEI0_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP), QEI_MAX_POSITION);	//编码器参数设置
	QEIEnable(QEI0_BASE);	//使能正交编码器
	QEIPositionSet(QEI0_BASE, 0);	//设置编码器初始值
}

/**************************************************************************
函数功能：正交编码器1初始化
入口参数：无
返回  值：无
**************************************************************************/
void QEI1_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);  	//使能 QEI0 外设
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	//使能 GPIOD 外设
	
	GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5|GPIO_PIN_6);	//配置QEI外围设备使用的引脚 PC5|PC6
	GPIOPinConfigure(GPIO_PC5_PHA1);	//配置GPIO引脚的备用功能
	GPIOPinConfigure(GPIO_PC6_PHB1);	//配置GPIO引脚的备用功能
	
	QEIConfigure(QEI1_BASE,(QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE | QEI_CONFIG_SWAP), QEI_MAX_POSITION);	//编码器参数设置
	QEIEnable(QEI1_BASE);			//使能正交编码器
	QEIPositionSet(QEI1_BASE, 0);	//设置编码器初始值
}

/**************************************************************************
函数功能：得到编码器计数值（函数内容包含软硬件连接（硬连接），不可轻易更改）
入口参数：num：编码器编号	L：左轮编码器	R:右轮编码器
返回  值：电机编码器绝对计数值
**************************************************************************/
int32_t Get_QEI_Count(unsigned char num)
{
	int32_t count;
	
	if (num == L)	count = QEIPositionGet(QEI0_BASE);	//编码器位置获取
	else	count = QEIPositionGet(QEI1_BASE);	//编码器位置获取

#if	L_Change_Diretion
	if (num == L)	count = -count;
#endif

#if R_Change_Diretion
	if (num == R)	count = -count;
#endif
	
	return count;
}

/**************************************************************************
函数功能：得到电机速度
入口参数：num：编码器编号(L/R)，time：电机测速间隔(ms)
返回  值：电机转动速度(转每分)	//测速最好仅在一个定时器内测电机速度，不然会导致测速间隔不准确
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
	
	/*速度计算： 速度(r/min) =  ((count_chage / ROTO_RATIO / REDUCTION_RATIO) / (time / 1000)) * 60 */
	Speed = (float)(60000 * count_change) / (float)(time * ROTO_RATIO * REDUCTION_RATIO);
	
	return Speed;
}
