#include "key.h"

///**************************************************************************
//�������ܣ�������ʼ��������PF4(SW1),PF0(SW2)����ͨIO������
//��ڲ�������
//����  ֵ����
//**************************************************************************/
//void KEY_Init(void)
//{
//	//���� GPIOF ����
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//	
//	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x00;
////	
//	//����PF4Ϊ��������ģʽ		����SW1
//    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

//	//����PF0Ϊ��������ģʽ		����SW2
//    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
//}

///**************************************************************************
//�������ܣ�����ɨ�躯��������������ѡ�񰴼��ܷ�������
//��ڲ�����mode����ģʽ��0��֧����������1֧��������
//����  ֵ����
//**************************************************************************/
//unsigned char KEY_Scan(unsigned char mode)
//{
//	static unsigned char key_up = 1;	//�������ɿ���־
//	if(mode)	key_up = 1; 	//�Ƿ�֧��������
//	if(key_up == 1 && (SW1_STATE == 0 || SW2_STATE == 0))
//	{
//		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms��ʱ
//		key_up = 0;
//		if(SW1_STATE == 0)	return SW1_PRESS;		//����0����
//		else if(SW2_STATE == 0)	return SW2_PRESS;	//����1����
//	}else if(key_up == 0 && SW1_STATE == 1 && SW2_STATE == 1)	key_up = 1;
// 	return 0;	//�ް�������
//}
/*
    PF0��PD7���������ڽ����󷽿�ָ�����Ź���

*/
void GPIO_Unlock(uint32_t ui32Port, uint8_t ui8Pins)
{
    // Unlock the commit register
  HWREG(ui32Port + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
    // Write 0x01 to the corresponding bit in commit register
  HWREG(ui32Port + GPIO_O_CR) |= 0x01 ;//<<ui8Pins
  //��������
    HWREG(ui32Port + GPIO_O_LOCK) = 0;
}

int Key=0,cnt=0;
void PF_interrupt(void)
{
	uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE,true);//��ȡ�жϱ�־λ  
	if((s&GPIO_PIN_0) == GPIO_PIN_0)//
	{
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms��ʱ
		while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){}//�ȴ������ɿ�
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms��ʱ
		Key=cnt;
//		if(flag==0)
//		{
//			flag=1;
////			printf("Key = %d    flag = %d\n",Key,flag);
//		}
//		else if(flag==1)
//		{
//			Key=0;
//			flag=0;
//			cnt=0;
////			printf("Key = %d    flag = %d    cnt = %d\n",Key,flag,cnt);
//		}
	}
	else if((s&GPIO_PIN_4) == GPIO_PIN_4)//
	{
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms��ʱ
		while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){}//�ȴ������ɿ�
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms��ʱ
		cnt++;
//		printf("cnt = %d ",cnt);
	}
	
	GPIOIntClear(GPIO_PORTF_BASE, s);//����жϱ�־λ
}




void KEY_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//ʹ��GPIOF
    GPIO_Unlock(GPIO_PORTF_BASE,0);//����PF0
	GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4, GPIO_DIR_MODE_IN);//���� 
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//����
    
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);//�½���
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);//�½���
	GPIOIntRegister(GPIO_PORTF_BASE,PF_interrupt);  //�����жϴ������ĵ�ַ  �жϺ��������Զ���
    
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_0);//������PF0����
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);//������PF4����
	IntEnable(INT_GPIOF);//������F�˿ڵĿ���
	IntMasterEnable();//�������ܿ���        
}
