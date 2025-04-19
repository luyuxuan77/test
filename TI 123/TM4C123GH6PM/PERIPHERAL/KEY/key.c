#include "key.h"

///**************************************************************************
//函数功能：按键初始化函数，PF4(SW1),PF0(SW2)做普通IO口输入
//入口参数：无
//返回  值：无
//**************************************************************************/
//void KEY_Init(void)
//{
//	//开启 GPIOF 外设
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//	
//	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;
//	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x00;
////	
//	//设置PF4为上拉输入模式		按键SW1
//    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

//	//设置PF0为上拉输入模式		按键SW2
//    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
//    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
//}

///**************************************************************************
//函数功能：按键扫描函数，按键消抖与选择按键能否连续按
//入口参数：mode按键模式：0不支持连续按，1支持连续按
//返回  值：无
//**************************************************************************/
//unsigned char KEY_Scan(unsigned char mode)
//{
//	static unsigned char key_up = 1;	//按键按松开标志
//	if(mode)	key_up = 1; 	//是否支持连续按
//	if(key_up == 1 && (SW1_STATE == 0 || SW2_STATE == 0))
//	{
//		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms延时
//		key_up = 0;
//		if(SW1_STATE == 0)	return SW1_PRESS;		//按键0按下
//		else if(SW2_STATE == 0)	return SW2_PRESS;	//按键1按下
//	}else if(key_up == 0 && SW1_STATE == 1 && SW2_STATE == 1)	key_up = 1;
// 	return 0;	//无按键按下
//}
/*
    PF0和PD7脚有锁，在解锁后方可指定引脚功能

*/
void GPIO_Unlock(uint32_t ui32Port, uint8_t ui8Pins)
{
    // Unlock the commit register
  HWREG(ui32Port + GPIO_O_LOCK) = GPIO_LOCK_KEY; 
    // Write 0x01 to the corresponding bit in commit register
  HWREG(ui32Port + GPIO_O_CR) |= 0x01 ;//<<ui8Pins
  //重新锁定
    HWREG(ui32Port + GPIO_O_LOCK) = 0;
}

int Key=0,cnt=0;
void PF_interrupt(void)
{
	uint32_t s = GPIOIntStatus(GPIO_PORTF_BASE,true);//读取中断标志位  
	if((s&GPIO_PIN_0) == GPIO_PIN_0)//
	{
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms延时
		while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){}//等待按键松开
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms延时
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
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms延时
		while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)){}//等待按键松开
		SysCtlDelay(((SysCtlClockGet() / 3000) * 10) - 1);//10ms延时
		cnt++;
//		printf("cnt = %d ",cnt);
	}
	
	GPIOIntClear(GPIO_PORTF_BASE, s);//清除中断标志位
}




void KEY_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//使能GPIOF
    GPIO_Unlock(GPIO_PORTF_BASE,0);//解锁PF0
	GPIODirModeSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4, GPIO_DIR_MODE_IN);//输入 
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//上拉
    
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);//下降沿
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE);//下降沿
	GPIOIntRegister(GPIO_PORTF_BASE,PF_interrupt);  //设置中断处理程序的地址  中断函数名字自定义
    
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_0);//开启了PF0开关
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_INT_PIN_4);//开启了PF4开关
	IntEnable(INT_GPIOF);//开启了F端口的开关
	IntMasterEnable();//开启了总开关        
}
