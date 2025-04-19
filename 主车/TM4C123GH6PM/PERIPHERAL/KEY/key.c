#include <stdint.h>
#include <stdbool.h>
#include "tiva_sys.h"
#include "tiva_lib.h"
#include "uart1.h"
#include "my_timer.h"
#include "control.h"
#include "motor.h"
#include "uart0.h"
#include "ccd.h"
#include "rgb.h"
#include "key.h"
#include "encoder.h" 
#include "track.h"
#include "State_Machine.h"
#include "uart7.h"
#include "distance.h"
#include "oled.h"
#include "beep.h"




/* �Զ����������������ʵ����Ҫ�� */
void GPIO_Unlock(uint32_t ui32Port, uint8_t ui8Pins)
{
    HWREG(ui32Port + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
    HWREG(ui32Port + GPIO_O_CR) |= ui8Pins;
    HWREG(ui32Port + GPIO_O_LOCK) = 0;
}

/* ȫ�ֱ��� */
uint8_t itemFlag = 0;
volatile int lap_count = 0;         // ��¼����ɵ�Ȧ��
volatile int checkpoint_count = 0;  // ��¼��ǰȦ�ھ����ļ�������
volatile int prev_flag = 0;         // ��¼�Ƿ�վ������㣬��ֹ�ظ�����
volatile int key_flag = 0;  // ������־λ
volatile int flag = 1;  // ������־λ

volatile int key1 = 0;  // ������־λ
volatile int key2 = 0;  // ������־λ


/* �жϷ����� */
void PortF_Interrupt(void)
{
    uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE, true);

    if ((s & GPIO_PIN_4) && key_flag == 0)  // PF4�����������������
    {
        key_flag = 1;  // ��λ����ֹ�����������
        SysCtlDelay(SysCtlClockGet() / 1000);    // ������ʱ
        while (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)) {} // �ȴ��ͷ�
        SysCtlDelay(SysCtlClockGet() / 1000);
//        printf("PF4 Pressed\n");
		key1 = 1;
        key_flag = 0;  // �ͷű�־
    }

    // ���PF4�ж�
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
}

void PortE_Interrupt(void)
{
    uint32_t s = GPIOIntStatus(GPIO_PORTE_BASE, true);

    if ((s & GPIO_PIN_3) && key_flag == 0)  // PE3�����������������
    {
        key_flag = 1;  // ��λ����ֹ�����������
        SysCtlDelay(SysCtlClockGet() / 2000);    // ������ʱ
        while (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3)) {} // �ȴ��ͷ�
        SysCtlDelay(SysCtlClockGet() / 2000);
//        printf("PE3 Pressed\n");
		key2 = 1;
        key_flag = 0;  // �ͷű�־
    }

    // ���PE3�ж�
    GPIOIntClear(GPIO_PORTE_BASE, GPIO_PIN_3);
}

/* ������ʼ�� */
void KEY_Init(void)
{
    // ʹ��GPIOF��GPIOEʱ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // ����PF4
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, 
                    GPIO_PIN_4,
                    GPIO_STRENGTH_2MA, 
                    GPIO_PIN_TYPE_STD_WPU);

    // ����PE3
    GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTE_BASE, 
                    GPIO_PIN_3,
                    GPIO_STRENGTH_2MA, 
                    GPIO_PIN_TYPE_STD_WPU);

    // ����PF4�ж�
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, PortF_Interrupt);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);

    // ����PE3�ж�
    GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTE_BASE, PortE_Interrupt);
    GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_3);

    // ʹ��ȫ���ж�
    IntEnable(INT_GPIOF);
    IntEnable(INT_GPIOE);
    IntMasterEnable();
}

void Menu_Process(void)
{
    if (key1 == 1)
    {
        key1 = 0;
        itemFlag++;
        delay_ms(5);
    }

    if (key2 == 1)
    {  	
		printf("%d",itemFlag); 
        delay_ms(200); 
		Grren_Light_Control(1);
        key2 = 0;
        BEEP_BeepNTimes(itemFlag);
		Grren_Light_Control(0);
        SysCtlDelay(SysCtlClockGet() / 200); // ������ʱ
//        if (itemFlag > 0 && itemFlag <= 9)  // ȷ����Ŀѡ��
//        {
//            itemFlag = 0;
//        }
		
		switch (itemFlag)
		{
			case 1:
				task1();
				break;

			case 2:
				task2();
				break;

			case 3:
				task3();
				break;

			case 4:
				task4();
				break;

	//        case 5:
	//            task5();
	//            break;

	//        case 6:
	//            task6();
	//            break;

	//        case 7:
	//            task7();
	//            break;

	//        case 8:
	//            task8();
	//            break;

	//        case 9:
	//            task9();
	//            break;
		}
    }
    else // ���û�а��� key2���� LED ������˸
	{
	LED_FlashNTimes(itemFlag);
	delay_ms(800);
	}

}

int task1(void)
{
	checkpoint_count=0;
	while (checkpoint_count < 1) 
	{  // ֻ�� 1Ȧ
		Track_Direction_Control(85);  // 0.3m/s
		// ����Ƿ񵽴���㣨��������ж��߼���
	// ����Ƿ񵽴���㣨����������������7�����Ŷ�Ϊ1ʱ������������
		if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
		{
		Buzzer_On();        // ����������
			delay_ms(40);
		prev_flag = 1;      // ��¼����״̬�������ظ�����
			prev_flag = 0;
		Buzzer_Off(); // �رշ�����
		checkpoint_count++; // ��¼��ǰȦ�ļ�����
		}
    }
    // ��ʻ 1 Ȧ��ֹͣС��
			motor_4_speed(0,0,0,0);
	itemFlag = 0;
	printf("9");
	return 0;
}

int task2(void)
{
	checkpoint_count=0;
	while (checkpoint_count < 2) 
	{  // ֻ�� 1Ȧ
	Track_Direction_Control1(130);  // 0.5m/s
	// ����Ƿ񵽴���㣨��������ж��߼���
// ����Ƿ񵽴���㣨����������������7�����Ŷ�Ϊ1ʱ������������
	if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
	{
    Buzzer_On();        // ����������
		delay_ms(40);
    prev_flag = 1;      // ��¼����״̬�������ظ�����
	    prev_flag = 0;
    Buzzer_Off(); // �رշ�����
    checkpoint_count++; // ��¼��ǰȦ�ļ�����
		delay_ms(5);
	} 
    }
    // ��ʻ 1 Ȧ��ֹͣС��
    motor_4_speed(0, 0, 0, 0);
	itemFlag = 0;
		printf("9");
	return 0;
}
int task3(void)
{
	checkpoint_count=0;
	while (checkpoint_count < 3) 
	{
		if(checkpoint_count==0)
		{
			Track_Direction_Control2(140);
		}
		if(checkpoint_count==1)
		{
			Track_Direction_Control2(137);
		}
		else if (checkpoint_count==2)
		{
			Track_Direction_Control3(155);
		}
	// ����Ƿ񵽴���㣨��������ж��߼���
// ����Ƿ񵽴���㣨����������������7�����Ŷ�Ϊ1ʱ������������
	if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
	{
    Buzzer_On();        // ����������
		delay_ms(40);
		prev_flag = 1;      // ��¼����״̬�������ظ�����
	    prev_flag = 0;
    Buzzer_Off(); // �رշ�����
    checkpoint_count++; // ��¼��ǰȦ�ļ�����
	}
    }
    // ��ʻ 1 Ȧ��ֹͣС��
	itemFlag = 0;
	printf("9");
	motor_4_speed(0,0,0,0);
	return 0;
}
//int task3(void)
//{
//	checkpoint_count=0;
//	while (checkpoint_count < 3) 
//	{  // ֻ�� 1Ȧ
//	Track_Direction_Control1(140);  // 0.5m/s
//	// ����Ƿ񵽴���㣨��������ж��߼���
//// ����Ƿ񵽴���㣨����������������7�����Ŷ�Ϊ1ʱ������������
//	if (PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2 && (prev_flag == 0)) 
//	{
//    Buzzer_On();        // ����������
//		delay_ms(30);
//    prev_flag = 1;      // ��¼����״̬�������ظ�����
//	    prev_flag = 0;
//    Buzzer_Off(); // �رշ�����
//    checkpoint_count++; // ��¼��ǰȦ�ļ�����
//	}
//	if(checkpoint_count==1)
//	{
//		printf("7");
//	}
//	if((PD0 && PB3) &&(flag == 1)&&(checkpoint_count==2))
//	{       
//		motor_4_speed(100, 100, 220, 220);
//		delay_ms(250);
//			flag=0;
//		printf("8");
//	}
//	
//    }
//    // ��ʻ 1 Ȧ��ֹͣС��
//	itemFlag = 0;
//	printf("9");
//	motor_4_speed(0,0,0,0);
//	return 0;
//}
int task4(void)
{
	while(!(PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2))
	{
		Track_Direction_Control2(210);
	}
	printf("5");
	motor_4_speed(0,0,0,0);
		Buzzer_On();  // ��
		delay_ms(500);
		Buzzer_Off(); // ͣ
		delay_ms(500);
		Buzzer_On();  // ��
		delay_ms(500);
		Buzzer_Off(); // ͣ
		delay_ms(500);
		Buzzer_On();  // ��
		delay_ms(500);
		Buzzer_Off(); // ͣ
		delay_ms(500);
		Buzzer_On();  // ��
		delay_ms(500);
		Buzzer_Off(); // ͣ
		delay_ms(500); 
		Buzzer_On();  // ��
		delay_ms(500);
		Buzzer_Off(); // ͣ
		delay_ms(500);
	while(!(PB2 && PB3 && PD0 && PD1 && PD2 && PD3 && PE2))
	{
		Track_Direction_Control2(210);
	}
	printf("9");
	motor_4_speed(0,0,0,0);
	return 0;
}
