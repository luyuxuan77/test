#include "key.h"
#include "delay.h"

/**************************************************************************
函数功能：按键初始化函数，PE3+PE4+PA0做普通IO口输入
入口参数：无
返回  值：无
**************************************************************************/
void Key_Init(void)
{
	RCC->AHB1ENR |= 1<<0;  // 使能PORTA时钟 
	RCC->AHB1ENR |= 1<<4;  // 使能PORTE时钟
	GPIO_Set(GPIOA,PIN0|PIN4|PIN5,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);
//	GPIO_Set(GPIOE, PIN4, GPIO_MODE_IN, 0, 0, GPIO_PUPD_PU);  // PE3,PE4设置上拉输入
}

/**************************************************************************
函数功能：按键扫描函数，按键消抖与选择按键能否连续按
入口参数：mode按键模式：0不支持连续按，1支持连续按
返回  值：无
**************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1;  // 按键按松开标志
	if(mode)	key_up = 1;  // 是否支持连续按
	if(key_up == 1 && (KEY0 == 0 || KEY1 == 0))
	{
		delay_ms(10);  // 消抖
		key_up = 0;
		if(KEY0 == 0)	return KEY0_PRES;  // 按键0按下
		else if(KEY1 == 0)	return KEY1_PRES;  // 按键1按下
	}else if(key_up == 0 && KEY0 == 1 && KEY1 == 1)	key_up = 1;
 	return 0;  // 无按键按下
}
