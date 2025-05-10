#include "mymath.h"

/**************************************************************************
函数功能：滤波函数，储存近十次的数据测量值，求数据平均值并做滤波处理(一阶低通滤波)
入口参数：无
返回  值：无
**************************************************************************/
void Data_Filter(float Data_Measurement[10], float* OutData)
{
	unsigned char x, y;
	float temp, Data_Average = 0;

	/*冒泡程序，将测得数据从大到小排序*/
	for ( x = 10; x > 0; x-- )
	{
		for ( y = 0; y < x; y++ )
		{
			if (Data_Measurement[y] > Data_Measurement[x - 1])
			{
				temp = Data_Measurement[x - 1];
				Data_Measurement[x - 1] = Data_Measurement[y];
				Data_Measurement[y] = temp;
			}
		}
	}                     
	
	/*去掉最大的两个数和最小的两个数，求平均数*/
	for ( x = 2; x < 8; x++)
	{
		Data_Average += Data_Measurement[x];
	}
	Data_Average /= 6;
	
	/*一阶低通滤波算法，将此次的值与上次的值按照一定占比相加得到最终值*/
	*OutData = Data_Average * 0.8f + (*OutData)	* 0.2f;
}

/**************************************************************************
函数功能：数组数据二值化处理，阈值为最大值与最小值的中值
入口参数：*data目标数组，*out_data二值化数据储存数组，len数组长度
返回  值：无
**************************************************************************/
void Data_Binaryzation(unsigned char* data, unsigned char* out_data, unsigned char len)
{
	unsigned char i = 0;
	unsigned char min = 0, max = 0;
	unsigned char threshold_value;
	
	for (i = 0; i < len; i++)
	{
		if (data[i] > max)	max = data[i];
		if (data[i] < min)	min = data[i];
	}
	threshold_value = (max + min) / 2;
	for (i = 0; i < len; i++)
	{
		if (data[i] > threshold_value)	out_data[i] = 1;
		else if (data[i] < threshold_value)	out_data[i] = 0;
	}
}

/**************************************************************************
函数功能：取绝对值函数
入口参数：value：输入值
返回  值：无
**************************************************************************/
int Get_Absolute_Value(int value)
{
	if (value < 0)	value = -value;
	return value;
}
