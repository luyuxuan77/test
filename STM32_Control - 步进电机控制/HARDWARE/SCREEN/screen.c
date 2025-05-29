#include "screen.h"
#include "delay.h"
#include "touch.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "led.h"

//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLUE;//设置字体为蓝色 
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;//设置画笔蓝色 
}

//电阻触摸屏测试函数
void rtp_test(void)
{
	u8 key;
	u8 i=0;
	
	POINT_COLOR=RED;//设置字体为红色 
   	if(tp_dev.touchtype!=0XFF)LCD_ShowString(30,130,200,16,16,"Press KEY0 to Adjust");//电阻屏才显示
	delay_ms(1500);
 	Load_Drow_Dialog();
	
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//清除
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//画图	  			   
			}
		}else delay_ms(10);	//没有按键按下的时候 	    
		if(key==KEY0_PRES)	//KEY0按下,则执行校准程序
		{
			LCD_Clear(WHITE);	//清屏
		    TP_Adjust();  		//屏幕校准 
			TP_Save_Adjdata();	 
			Load_Drow_Dialog();
		}
		i++;
		if(i%20==0)LED1=!LED1;
	}
}


const char* Keyboard_Annotation[4][4] = {{"1","2","3","   "},
										 {"4","5","6","Del"},
										 {"7","8","9","Res"},
										 {"-","0",".","Ent"}};

void Numeric_Keyboard(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u8 i,j;
	LCD_Fill(x1, y1, x2, y2, WHITE);
	POINT_COLOR = BLACK;
	for (i = 0; i < 5; i++)
	{
		LCD_DrawLine((((x2 - x1) / 4) * i) + x1, y1, (((x2 - x1) / 4) * i) + x1, y2);
		LCD_DrawLine(x1, (((y2 - y1) / 4) * i) + y1, x2, (((y2 - y1) / 4) * i) + y1);
	}
	POINT_COLOR = RED;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j ++)
		{
			LCD_ShowString((((x2 - x1) / 4) * i) + ((x2 - x1) / 12) + x1, (((y2 - y1) / 4) * j) + ((y2 - y1) / 12) + y1, 24, 16, 16, (u8*)Keyboard_Annotation[j][i]);
		}
	}

}

char LCD_KeyBoard_Scan(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u8 i, j;
	char key_value;
	tp_dev.scan(0);
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{
		if(tp_dev.x[0]<lcddev.width && tp_dev.y[0]<lcddev.height)
		{
			for (i = 0; i < 5; i++)
			{
				for (j = 0; j < 5; j++)
				{
					if ((tp_dev.x[0] > (((x2 - x1) / 4) * i)) && (tp_dev.x[0] < (((x2 - x1) / 4) * (i + 1))) && (tp_dev.y[0] > (((y2 - y1) / 4) * j)) && (tp_dev.y[0] < (((y2 - y1) / 4) * (j + 1))))
					{
						key_value = *Keyboard_Annotation[j][i];
						LCD_Fill((((x2 - x1) / 4) * i),(((y2 - y1) / 4) * j),(((x2 - x1) / 4) * (i + 1)),(((y2 - y1) / 4) * (j + 1)), BLACK);
						LCD_ShowString((((x2 - x1) / 4) * i) + ((x2 - x1) / 12) + x1, (((y2 - y1) / 4) * j) + ((y2 - y1) / 12) + y1, 24, 16, 16, (u8*)Keyboard_Annotation[j][i]);
						while (tp_dev.sta&TP_PRES_DOWN)	tp_dev.scan(0);
						LCD_Fill((((x2 - x1) / 4) * i) + 1,(((y2 - y1) / 4) * j) + 1,(((x2 - x1) / 4) * (i + 1)) - 1,(((y2 - y1) / 4) * (j + 1)) - 1, WHITE);
						LCD_ShowString((((x2 - x1) / 4) * i) + ((x2 - x1) / 12) + x1, (((y2 - y1) / 4) * j) + ((y2 - y1) / 12) + y1, 24, 16, 16, (u8*)Keyboard_Annotation[j][i]);
						return key_value;
					} 	
				}
			}		   
		}
	}
	
	return 0;
}

float Get_LCD_KeyBoard_Value(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u8 i;
	u8 value_lenght = 0;
	char key_value;
	char OutValue_Array[11] = {0};
	float OutValue;
	
	Numeric_Keyboard(x1, y1, x2, y2);
	while(OutValue_Array[10] != '1')
	{
		key_value = LCD_KeyBoard_Scan(x1, y1, x2, y2);
		
		if (key_value == 0) continue;
		else if (key_value == 'E')
		{
			OutValue = Get_Array_Value(OutValue_Array);
			if (OutValue_Array[10] == '0')
			{
				for (i = 0; i < 10; i++)
				{
					OutValue_Array[i] = 0;
				}
				value_lenght = 0;
				LCD_Fill((((x2 - x1) / 4) * 3) + 1,(((y2 - y1) / 4) * 0) + 1,(((x2 - x1) / 4) * (3 + 1)) - 1,(((y2 - y1) / 4) * (0 + 1)) - 1, WHITE);
				LCD_ShowString(((x2 - x1) / 4) * 3 + 8,((y2 - y1) / 4) * 0 + 8,(x2 - x1) / 4 - 16, (y2 - y1) / 8, 16, "error");
				delay_ms(2000);
				LCD_Fill((((x2 - x1) / 4) * 3) + 1,(((y2 - y1) / 4) * 0) + 1,(((x2 - x1) / 4) * (3 + 1)) - 1,(((y2 - y1) / 4) * (0 + 1)) - 1, WHITE);
			}
			continue;
		}
		else if (key_value == 'R')
		{
			for (i = 0; i < 10; i++)
			{
				OutValue_Array[i] = 0;
			}
			value_lenght = 0;
			LCD_Fill((((x2 - x1) / 4) * 3) + 1,(((y2 - y1) / 4) * 0) + 1,(((x2 - x1) / 4) * (3 + 1)) - 1,(((y2 - y1) / 4) * (0 + 1)) - 1, WHITE);
		}
		else if (key_value == 'D')
		{
			OutValue_Array[--value_lenght] = 0;
			LCD_Fill((((x2 - x1) / 4) * 3) + 1,(((y2 - y1) / 4) * 0) + 1,(((x2 - x1) / 4) * (3 + 1)) - 1,(((y2 - y1) / 4) * (0 + 1)) - 1, WHITE);
			LCD_ShowString(((x2 - x1) / 4) * 3 + 8,((y2 - y1) / 4) * 0 + 8,(x2 - x1) / 4 - 16, (y2 - y1) / 8, 16, (u8*)OutValue_Array);
		}
		else
		{
			OutValue_Array[value_lenght] = key_value;
			value_lenght++;
			LCD_ShowString(((x2 - x1) / 4) * 3 + 8,((y2 - y1) / 4) * 0 + 8,(x2 - x1) / 4 - 16, (y2 - y1) / 8, 16, (u8*)OutValue_Array);
		}
	}
	return OutValue;
}

char* Array_Data_Conversion(char* value)
{
	u8 i;
	for (i = 0; i < 10; i++)
	{
		if (value[i] >= '0' && value[i] <= '9')
		{
			value[i] -= '0';
		}
		else if (value[i] == 0)	value[i] = '0';
	}
	return	value;
}

float Get_Array_Value(char* value)
{
	u8 i;
	u8 minus_flag = 0;
	u16 minus_digit = 1;
	u8 decimals_flag = 0;
	float OutData = 0;
	float OutData_minus = 0;
	
	value = Array_Data_Conversion(value);
	
	if (value[0] == '-')	minus_flag = 1;
	if (minus_flag == 1) i = 1;
	else	i = 0;
	
	for ( ; i < 10; i++)
	{
		if (value[i] == '0')
		{
			OutData = OutData + (OutData_minus / (float)minus_digit);
			if (minus_flag == 1)	OutData = -OutData;
			value[10] = '1';
			return OutData;
		}
		else if (value[i] == '-')
		{
			value[10] = '0';
			return 0;
		}
		else if (value[i] == '.')
		{
			 if (decimals_flag == 0)
			 {
				decimals_flag = 1;
				continue;
			 }
			 else
			 {
				 
				value[10] = '0';
				 return 0;
			 }
		}
		
		if (decimals_flag == 0)	OutData = (OutData * 10) + value[i];
		else
		{
			OutData_minus = (OutData_minus * 10) + value[i];
			minus_digit = minus_digit * 10;
		}
	}
	
	OutData = OutData + (OutData_minus / (float)minus_digit);
	if (minus_flag == 1)	OutData = -OutData;
	value[10] = '1';
	return OutData;
}






