#include "keyboard.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "math.h"
#include "led.h"

/******************************************************************************
函数功能：矩阵按键初始化，将4个IO口配置为输出，将4个IO口配置为输入
入口参数：无
返回  值：无
*******************************************************************************/ 
void KeyBoard_Init(void)
{
	RCC->AHB1ENR |= 1<<5;
	GPIO_Set( GPIOF, PIN0|PIN1|PIN2, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_NONE );
	RCC->AHB1ENR |= 1<<6;
	GPIO_Set( GPIOG, PIN8, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M, GPIO_PUPD_NONE );
	GPIO_Set( GPIOG, PIN10|PIN11|PIN13|PIN15, GPIO_MODE_IN, 0, 0, GPIO_PUPD_PD );
}

/******************************************************************************
函数功能：矩阵按键扫描函数，判断按下的是哪个按键
入口参数：Mode：是否支持连续按，0:不支持，1:支持
返回  值：无		//放在while循环内使用
*******************************************************************************/ 
u8 KeyBoard_Scan(u8 Mode)
{
	u16 temp;
	
	Key_Reset(1);  // 第一行(列)按键左侧给高，用来判断第一行(列)按键是否被按下
	if((GPIOG->IDR | 0X53FF) != 0x53FF)  // 判断输入IO口是否检测到高电平
	{
		delay_ms(10);	// 延时消抖
		if((GPIOG->IDR | 0X53FF) != 0x53FF)  // 判断输入IO口是否检测到高电平
		{
			temp = (GPIOG->IDR | 0X53FF);
			switch(temp)  // 判断是哪一列(行)按键被按下，其对应的输入IO口被拉高
			{
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 1;  // 一列一行
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 5;  // 一列二行
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 9;  // 一列三行
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 13;  // 一列四行
			}
		}
	}

	Key_Reset(2);
	if((GPIOG->IDR | 0X53FF)!=0x53FF)
	{
		delay_ms(10);
		if((GPIOG->IDR | 0X53FF)!=0x53FF)
		{
			temp=(GPIOG->IDR | 0X53FF);
			switch(temp)
			{
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 2;  // 二列一行
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 6;  // 二列二行
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 10;  // 二列三行
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 14;  // 二列四行
			}
		}
	}
	
	Key_Reset(3);
	if((GPIOG->IDR | 0X53FF)!=0x53FF)
	{
		delay_ms(10);
		if((GPIOG->IDR | 0X53FF)!=0x53FF)
		{
			temp=(GPIOG->IDR | 0X53FF);
			switch(temp)
			{
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 3;  // 三列一行
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 7;  // 三列二行
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 11;  // 三列三行
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 15;  // 三列四行
			}
		}
	}
	
	Key_Reset(4);
	if((GPIOG->IDR | 0X53FF)!=0x53FF)
	{
		delay_ms(10);
		if((GPIOG->IDR | 0X53FF)!=0x53FF)
		{
			temp=(GPIOG->IDR | 0X53FF);
			switch(temp)
			{
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 4;  // 四列一行
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 8;  // 四列二行
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 12;  // 四列三行
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 16;  // 四列四行
			}
		}
	}
	return 0;
}

/******************************************************************************
函数功能：矩阵按键IO口输出高低电平改变，给某一行(列)按键的左端给高电平，其余给低电平
入口参数：column：哪一行(列)按键左侧给高，其余按键给低
返回  值：无
*******************************************************************************/ 
void Key_Reset(u8 column)
{
	switch(column)
	{
		case 1:	PGout(8)=1,PFout(2)=0,PFout(1)=0,PFout(0)=0;	break;  // 第一行(列)按键左侧给高，其余按键给低
		
		case 2:	PGout(8)=0,PFout(2)=1,PFout(1)=0,PFout(0)=0;	break;  // 第二行(列)按键左侧给高，其余按键给低
		
		case 3:	PGout(8)=0,PFout(2)=0,PFout(1)=1,PFout(0)=0;	break;  // 第三行(列)按键左侧给高，其余按键给低
		
		case 4:	PGout(8)=0,PFout(2)=0,PFout(1)=0,PFout(0)=1;	break;  // 第四行(列)按键左侧给高，其余按键给低
	}
}

/******************************************************************************
函数功能：按键键值与按键含义对应
入口参数：key:按键键值
返回  值：按键含义
*******************************************************************************/ 
u8 Key_Transtition_Value(u8 key)
{
	if (key == 1)	return 1;
	else if (key == 2)	return 2;
	else if (key == 3)	return 3;
	else if (key == 4)	return 'T';
	else if (key == 5)	return 4;
	else if (key == 6)	return 5;
	else if (key == 7)	return 6;
	else if (key == 8)	return 'D';
	else if (key == 9)	return 7;
	else if (key == 10)	return 8;
	else if (key == 11)	return 9;
	else if (key == 12)	return 'R';
	else if (key == 13)	return '-';
	else if (key == 14)	return 0;
	else if (key == 15)	return '.';
	else if (key == 16)	return 'E';
	return '0';
}

/******************************************************************************
函数功能：矩阵按键输入整数数值
入口参数：无
返回  值：矩阵按键输入的整数
*******************************************************************************/ 
int Get_Int_Numeric_Keyboard(void)
{
	u8 key_value = 0;  // 按键键值，案件含义值
	u8 minus_flag = 0;  // 输出值正负标志位
	int OutData = 0;  // 输出值
	while(key_value != 'E')  // 检测到按下Ent，输入完成，输出整数
	{
		key_value = KeyBoard_Scan(0);  // 按键扫描，得到键值
		key_value = Key_Transtition_Value(key_value);  // 按键键值转换为按键含义
		if (key_value == '0' || key_value == 'E')	continue;  // 未按下按键、按下Ent键，返回；	
		if (key_value == 'T' || key_value == '.' )	continue;  // 按下Tes键与'.'键，返回；	Tes未定义，此函数为取整数函数'.'不使用
		
		if (key_value == 'R')  // 按下按键Res，清零输出
		{
			OutData = 0;  // 输出值变为零
			OLED_ShowString(60, 30, "     ",16);  // 显示屏输出值清空
			OLED_Refresh_Gram();
			continue;
		}
		
		if (key_value == '-')  // 按下按键'-'，输出负值
		{
			if (OutData == 0)  // 若按下'-'时，输出值为零，则表示输入负值
			{
				minus_flag = 1;  // 输出值为负标志位
				OLED_ShowChar(60, 30, '-', 16, 1);  // 显示屏打上'-'号
				OLED_Refresh_Gram();
			}
			else  // 若按下'-'时，输出值不为零，则表示输入错误
			{
				OLED_ShowString(60, 30, "     ",16);  // 显示屏输出值清空
				OLED_Refresh_Gram();
				Led_Warning();  // LED警告灯开启
				OutData = 0;  // 输出值赋0
			}
		}
		else
		{
			OutData = OutData * 10 + key_value;  // 输出值计算
			OLED_ShowNum(70, 30, OutData, 3, 16);  // 输出值显示在显示屏上
			OLED_Refresh_Gram();
		}
	}
	if (minus_flag == 1)	OutData = -OutData;  // 输出值为负
	OLED_ShowString(60, 30, "     ",16);  // 显示屏输出值清空
	OLED_Refresh_Gram();
	return OutData;
}

//可以用键盘获得小数，垃圾函数，别用
float Get_KeyBoard_Value(void)
{
	u8 i = 0;
	u8 key_value = 0;
	u8 minus_flag = 0, decimals_flag = 0;
	u8 decimals_digits = 0;
	float OutData = 0.0;
	
	while(key_value != 'E')
	{
		key_value = KeyBoard_Scan(0);
		key_value = Key_Transtition_Value(key_value);
		
		switch	(key_value)
		{
			case '0':	continue;
			
			case 'E':	continue;
			
			case 'T':	continue;
			
			case 'R':	OutData = 0;	continue;
			
			case 'D':
			{
				if (decimals_flag == 1)
				{
					if (decimals_digits == 0)	decimals_flag = 0;
					else
					{
						decimals_digits--;
						OutData = (int)OutData / 10;
					}
				}
				else	OutData = (int)OutData / 10;
			}
			continue;
			
			case '.':
			{
				if (decimals_flag == 0)	decimals_flag = 1;
				else
				{
					Led_Warning();
					decimals_flag = 0;
					OutData = 0;
				}
			}
			continue;
			
			case '-':
			{
				if (OutData == 0)	minus_flag = 1;
				else 
				{
					Led_Warning();
					OutData = 0;
				}
			}
			continue;
		}
		OutData = OutData * 10 + key_value;
		if (decimals_flag == 1)	decimals_digits++;
	}
	if (decimals_flag == 1)
	{
		for (i = 0; i < decimals_digits; i++)	OutData = OutData / 10.0f;
	}
	if (minus_flag == 1)	OutData = -OutData;
	return OutData;

}
