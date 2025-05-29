#include "bsp.h"


void motor_init(void)
{
	tima0_pwm_init();
	
	// 初始化输出引脚为禁用引脚输出反相、使能下拉、禁用高驱动强度、禁用高阻态
    DL_GPIO_initDigitalOutputFeatures(S_LD_N_IOMUX, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                                    DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_setPins(S_PORT, S_LD_N_PIN);  // 初始状态为拉高
    DL_GPIO_enableOutput(S_PORT, S_LD_N_PIN);  // 使能输出
	DL_GPIO_writePinsVal(S_PORT, S_LD_N_PIN,0);
	
	// 初始化输出引脚为禁用引脚输出反相、使能下拉、禁用高驱动强度、禁用高阻态
    DL_GPIO_initDigitalOutputFeatures(S_RU_N_IOMUX, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                                    DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_setPins(S_PORT, S_RU_N_PIN);  // 初始状态为拉高
    DL_GPIO_enableOutput(S_PORT, S_RU_N_PIN);  // 使能输出
	DL_GPIO_writePinsVal(S_PORT, S_RU_N_PIN,0);

	// 初始化输出引脚为禁用引脚输出反相、使能下拉、禁用高驱动强度、禁用高阻态
    DL_GPIO_initDigitalOutputFeatures(S_LU_N_IOMUX, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                                    DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_setPins(S_PORT, S_LU_N_PIN);  // 初始状态为拉高
    DL_GPIO_enableOutput(S_PORT, S_LU_N_PIN);  // 使能输出
	DL_GPIO_writePinsVal(S_PORT, S_LU_N_PIN,0);
	
	// 初始化输出引脚为禁用引脚输出反相、使能下拉、禁用高驱动强度、禁用高阻态
    DL_GPIO_initDigitalOutputFeatures(S_RD_N_IOMUX, DL_GPIO_INVERSION_DISABLE, DL_GPIO_RESISTOR_PULL_DOWN,
                                                    DL_GPIO_DRIVE_STRENGTH_LOW, DL_GPIO_HIZ_DISABLE);
    DL_GPIO_setPins(S_PORT, S_RD_N_PIN);  // 初始状态为拉高
    DL_GPIO_enableOutput(S_PORT, S_RD_N_PIN);  // 使能输出
	DL_GPIO_writePinsVal(S_PORT, S_RD_N_PIN,0);
}
Motor_Data_TypeDef	Motor_Data[4] = {{ 0, 0, 0, 0, 0, 0 },
									 { 1, 0, 0, 0, 0, 0 },
									 { 2, 0, 0, 0, 0, 0 },
									 { 3, 0, 0, 0, 0, 0 },};


void motor_4(int LU,int RU,int LD,int RD)
{
    if(LU>0)
    {
        DL_GPIO_writePinsVal(S_PORT, S_LU_N_PIN,S_LU_N_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, 3200-LU, DL_TIMER_CC_0_INDEX);
    }
    else
    {
        DL_GPIO_writePinsVal(S_PORT, S_LU_N_PIN,0);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, -LU, DL_TIMER_CC_0_INDEX);
    }
    
    if(LD>0)
    {
        DL_GPIO_writePinsVal(S_PORT, S_LD_N_PIN,S_LD_N_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, LD, DL_TIMER_CC_3_INDEX);
    }
    else
    {
        DL_GPIO_writePinsVal(S_PORT, S_LD_N_PIN,0);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST,3200+LD, DL_TIMER_CC_3_INDEX);
    }
    
    if(RU>0)
    {
        DL_GPIO_writePinsVal(S_PORT, S_RU_N_PIN,0);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, RU, DL_TIMER_CC_1_INDEX);
    }
    else
    {
        DL_GPIO_writePinsVal(S_PORT, S_RU_N_PIN,S_RU_N_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, 3200+RU, DL_TIMER_CC_1_INDEX);
    }
    
    if(RD>0)
    {
        DL_GPIO_writePinsVal(S_PORT, S_RD_N_PIN,0);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, 3200-RD, DL_TIMER_CC_2_INDEX);
    }
    else
    {
        DL_GPIO_writePinsVal(S_PORT, S_RD_N_PIN,S_RD_N_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, -RD, DL_TIMER_CC_2_INDEX);
    }
}











