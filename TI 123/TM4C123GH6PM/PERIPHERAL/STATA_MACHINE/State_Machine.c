#include "State_Machine.h"

bool Fork_Check(void)
{
	if( Grayscale_Data.grayscale_digital == 0x1A )
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Closed_Check(void)
{
	if(Grayscale_Data.grayscale_digital == 0x09)
	{
		delay_ms(10);
		Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
		if(Grayscale_Data.grayscale_digital == 0x09)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool Final_Check(void)
{
	if(Grayscale_Data.grayscale_digital == 0x7F)
	{
		delay_ms(10);
		Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
		if(Grayscale_Data.grayscale_digital == 0x7F)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool Line_Check(void)
{
	if(Grayscale_Data.grayscale_digital == 0x08)
	{
		delay_ms(1);
		Grayscale_Data.grayscale_digital = Grayscale_Sensor_Digital;
		if(Grayscale_Data.grayscale_digital == 0x08)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
uint8_t step = 5;
void State_Machine_Main(void)
{
	switch(step)
	{
		case 1:
		{
			//motor_2(0,0);
			L1_Speed_Pid.SetPoint = 0;
			R1_Speed_Pid.SetPoint = 0;
		}break;
		case 2:
		{ 
			cheat = 10;
			sensor_weights[0] = -1;
			sensor_weights[1] = -1;
			while(Flag == 0)
			{
				Track_Direction_Distance_Control(85);
			}
			sensor_weights[0] = -3;
			sensor_weights[1] = -2 ;
			Flag = 0;
			step = 3;
		}break;
		case 3:
		{
			Track_Direction_Distance_Control(85);
		}break;
		case 4:
		{
			Pid_Init(&L1_Speed_Pid, &L1_PID_Value_Speed);
			Pid_Init(&R1_Speed_Pid, &R1_PID_Value_Speed);  
			step = 1;
		}break;
		case 5:
		{
			cheat = 10;
			sensor_weights[0] = -1;
			sensor_weights[1] = -1 ;
			while(Flag == 0)
			{
				Track_Direction_Control(270);
			}
			sensor_weights[0] = -3;
			sensor_weights[1] = -2 ;
			Flag = 0;
			step = 6;
		}break;
		case 6:
		{
			Track_Direction_Control(155);
		}break;
		case 7:
		{
			Track_Direction_Control(140);
			if(Final_Check())
			{
				step = 8;
			}
		}break;
		case 8:
		{
			Track_Direction_Control(140);
			if(Line_Check())
			{
				step = 9;
			}
			
		}break;
		case 9:
		{
			Track_Direction_Control(140);
			if(Final_Check())
			{
				step = 10;
			}
		}break;
		case 10:
		{
			Track_Direction_Control(140);
			sensor_weights[0] = -10;
			sensor_weights[1] = -9;
			cheat = 6;
			while(Flag == 0)
			{
				Track_Direction_Control(180);
			}
			sensor_weights[0] = -3;
			sensor_weights[1] = -2;
			
			Flag = 0;
			step = 11;
		}break;
		case 11:
		{
			Track_Direction_Control(150);
		}break;
		case 12:
		{
			Track_Direction_Control(130);
		}break;
		case 13:
		{
			Track_Direction_Control(140);
			if(Final_Check())
			{
				step = 14;
			}
		}break;
		case 14:
		{
			cheat = 1;
			while(Flag == 0)
			{
				L1_Speed_Pid.SetPoint = 40;
				R1_Speed_Pid.SetPoint = 40; 
			}
			Flag = 0;
			step = 15;
		}break;
		case 15:
		{
			Track_Direction_Control(180);
		}break;
		default:break;
	}
	
}

