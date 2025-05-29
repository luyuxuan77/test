#ifndef CCD_H
#define CCD_H
#include "sys.h"
#include "pid.h"

//����CCDģ�����ݽṹ��
typedef struct
{
	u8 receive_data[128];  // �ɼ�����CCDԭʼ����
	u8 process_data[128];  // ���������ݴ��������
	u8 binaryzation_data[128];  // ��ֵ������������
	u8 receive_flag;  // �ɼ���ɱ�־
	u8 threshold_value;  // ������ֵ
	u8 left_edge_location;  // ��������λ��
	u8 right_edge_location;  // ��������λ��
}CCD_TypeDef;
extern CCD_TypeDef CCD_Data;

//Ѳ�ߣ�����������ݽṹ��
typedef struct
{
	u8 line_left;  // �ߵ��������
	u8 line_right;  // �ߵ��ұ�����
	u8 line_width;  // �ߵĿ��
	u8 line_location;  // �ߵ�λ��
	int line_offset;  // �ߵ�ƫ��
}Line_TypeDef;
extern Line_TypeDef Line_Data;

//����CCDѲ��Pid��ؽṹ��
extern PID_TypeDef CCD_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD[3];

//����CCD��������ؽṹ��
extern PID_TypeDef CCD_Direction_Pid;
extern p_i_d_Value_TypeDef PID_Value_CCD_Direction[3];

//����CCD���Ŷ���
#define CCD_CLK	PFout(14)  // CCDʱ���ź�����
#define CCD_SI	PFout(15)  // CCD�����ź�����
#define	CCD_AO_CH	15  // �Ҷ�ֵ������ţ����ΪADCֵ����ΪADC3��ͨ����

void CCD_Init(void);
void Get_CCD_Value(void);
void Dly_us(void);
void Send_To_Serial_Port(u8 mode);
void Send_To_Debug(u8 mode);
char binToHex_low(u8 num);
char binToHex_high(u8 num);

void Get_CCD_Threshold_Value(void);
void Get_Edge_StaticThreshold(u8 threshold_value);
void Get_Edge_DynamicThreshold(void);
void Edge_Dispose(u8 mode);
void CCD_Track_Pid(u16 pwm);
void CCD_Track_Direction_Control(float speed);
#endif

