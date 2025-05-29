#include "keyboard.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "math.h"
#include "led.h"

/******************************************************************************
�������ܣ����󰴼���ʼ������4��IO������Ϊ�������4��IO������Ϊ����
��ڲ�������
����  ֵ����
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
�������ܣ����󰴼�ɨ�躯�����жϰ��µ����ĸ�����
��ڲ�����Mode���Ƿ�֧����������0:��֧�֣�1:֧��
����  ֵ����		//����whileѭ����ʹ��
*******************************************************************************/ 
u8 KeyBoard_Scan(u8 Mode)
{
	u16 temp;
	
	Key_Reset(1);  // ��һ��(��)���������ߣ������жϵ�һ��(��)�����Ƿ񱻰���
	if((GPIOG->IDR | 0X53FF) != 0x53FF)  // �ж�����IO���Ƿ��⵽�ߵ�ƽ
	{
		delay_ms(10);	// ��ʱ����
		if((GPIOG->IDR | 0X53FF) != 0x53FF)  // �ж�����IO���Ƿ��⵽�ߵ�ƽ
		{
			temp = (GPIOG->IDR | 0X53FF);
			switch(temp)  // �ж�����һ��(��)���������£����Ӧ������IO�ڱ�����
			{
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 1;  // һ��һ��
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 5;  // һ�ж���
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 9;  // һ������
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 13;  // һ������
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
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 2;  // ����һ��
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 6;  // ���ж���
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 10;  // ��������
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 14;  // ��������
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
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 3;  // ����һ��
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 7;  // ���ж���
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 11;  // ��������
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 15;  // ��������
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
				case 0XD3FF:	if (Mode == 0)	while (PGin(15) == 1);	return 4;  // ����һ��
				case 0X73FF:	if (Mode == 0)	while (PGin(13) == 1);	return 8;  // ���ж���
				case 0X5BFF:	if (Mode == 0)	while (PGin(11) == 1);	return 12;  // ��������
				case 0X57FF:	if (Mode == 0)	while (PGin(10) == 1);	return 16;  // ��������
			}
		}
	}
	return 0;
}

/******************************************************************************
�������ܣ����󰴼�IO������ߵ͵�ƽ�ı䣬��ĳһ��(��)��������˸��ߵ�ƽ��������͵�ƽ
��ڲ�����column����һ��(��)���������ߣ����ఴ������
����  ֵ����
*******************************************************************************/ 
void Key_Reset(u8 column)
{
	switch(column)
	{
		case 1:	PGout(8)=1,PFout(2)=0,PFout(1)=0,PFout(0)=0;	break;  // ��һ��(��)���������ߣ����ఴ������
		
		case 2:	PGout(8)=0,PFout(2)=1,PFout(1)=0,PFout(0)=0;	break;  // �ڶ���(��)���������ߣ����ఴ������
		
		case 3:	PGout(8)=0,PFout(2)=0,PFout(1)=1,PFout(0)=0;	break;  // ������(��)���������ߣ����ఴ������
		
		case 4:	PGout(8)=0,PFout(2)=0,PFout(1)=0,PFout(0)=1;	break;  // ������(��)���������ߣ����ఴ������
	}
}

/******************************************************************************
�������ܣ�������ֵ�밴�������Ӧ
��ڲ�����key:������ֵ
����  ֵ����������
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
�������ܣ����󰴼�����������ֵ
��ڲ�������
����  ֵ�����󰴼����������
*******************************************************************************/ 
int Get_Int_Numeric_Keyboard(void)
{
	u8 key_value = 0;  // ������ֵ����������ֵ
	u8 minus_flag = 0;  // ���ֵ������־λ
	int OutData = 0;  // ���ֵ
	while(key_value != 'E')  // ��⵽����Ent��������ɣ��������
	{
		key_value = KeyBoard_Scan(0);  // ����ɨ�裬�õ���ֵ
		key_value = Key_Transtition_Value(key_value);  // ������ֵת��Ϊ��������
		if (key_value == '0' || key_value == 'E')	continue;  // δ���°���������Ent�������أ�	
		if (key_value == 'T' || key_value == '.' )	continue;  // ����Tes����'.'�������أ�	Tesδ���壬�˺���Ϊȡ��������'.'��ʹ��
		
		if (key_value == 'R')  // ���°���Res���������
		{
			OutData = 0;  // ���ֵ��Ϊ��
			OLED_ShowString(60, 30, "     ",16);  // ��ʾ�����ֵ���
			OLED_Refresh_Gram();
			continue;
		}
		
		if (key_value == '-')  // ���°���'-'�������ֵ
		{
			if (OutData == 0)  // ������'-'ʱ�����ֵΪ�㣬���ʾ���븺ֵ
			{
				minus_flag = 1;  // ���ֵΪ����־λ
				OLED_ShowChar(60, 30, '-', 16, 1);  // ��ʾ������'-'��
				OLED_Refresh_Gram();
			}
			else  // ������'-'ʱ�����ֵ��Ϊ�㣬���ʾ�������
			{
				OLED_ShowString(60, 30, "     ",16);  // ��ʾ�����ֵ���
				OLED_Refresh_Gram();
				Led_Warning();  // LED����ƿ���
				OutData = 0;  // ���ֵ��0
			}
		}
		else
		{
			OutData = OutData * 10 + key_value;  // ���ֵ����
			OLED_ShowNum(70, 30, OutData, 3, 16);  // ���ֵ��ʾ����ʾ����
			OLED_Refresh_Gram();
		}
	}
	if (minus_flag == 1)	OutData = -OutData;  // ���ֵΪ��
	OLED_ShowString(60, 30, "     ",16);  // ��ʾ�����ֵ���
	OLED_Refresh_Gram();
	return OutData;
}

//�����ü��̻��С������������������
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
