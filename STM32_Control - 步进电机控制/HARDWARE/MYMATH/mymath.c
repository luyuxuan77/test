#include "mymath.h"

/**************************************************************************
�������ܣ��˲������������ʮ�ε����ݲ���ֵ��������ƽ��ֵ�����˲�����(һ�׵�ͨ�˲�)
��ڲ�������
����  ֵ����
**************************************************************************/
void Data_Filter(float Data_Measurement[10], float* OutData)
{
	u8 x, y;
	float temp, Data_Average = 0;

	/*ð�ݳ��򣬽�������ݴӴ�С����*/
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
	
	/*ȥ����������������С������������ƽ����*/
	for ( x = 2; x < 8; x++)
	{
		Data_Average += Data_Measurement[x];
	}
	Data_Average /= 6;
	
	/*һ�׵�ͨ�˲��㷨�����˴ε�ֵ���ϴε�ֵ����һ��ռ����ӵõ�����ֵ*/
	*OutData = Data_Average * 0.8f + (*OutData)	* 0.2f;
}

/**************************************************************************
�������ܣ��������ݶ�ֵ��������ֵΪ���ֵ����Сֵ����ֵ
��ڲ�����*dataĿ�����飬*out_data��ֵ�����ݴ������飬len���鳤��
����  ֵ����
**************************************************************************/
void Data_Binaryzation(u8* data, u8* out_data, u8 len)
{
	u8 i = 0;
	u8 min = 0, max = 0;
	u8 threshold_value;
	
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
