#include "control.h"
#include "ccd.h"

/******************************************************************************
函数功能：逻辑控制函数，自定义
入口参数：无
返回  值：无
*******************************************************************************/ 
void Control(void)
{
	while(1)
	{
		if (CCD_Data.receive_flag == 1)
		{
			CCD_Track_Pid(3000);
			//Send_To_Debug(0);
			CCD_Data.receive_flag = 0;
		}
	}
}

