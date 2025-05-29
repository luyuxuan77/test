#include "ti_msp_dl_config.h"
#include "bsp.h"


SYSCONFIG_WEAK void SYSCFG_DL_init(void)
{
    SYSCFG_DL_initPower();
    SYSCFG_DL_GPIO_init();
    /* Module-Specific Initializations*/
    SYSCFG_DL_SYSCTL_init();
}

SYSCONFIG_WEAK void SYSCFG_DL_initPower(void)
{
    DL_GPIO_reset(GPIOA);
    DL_GPIO_reset(GPIOB);
    DL_GPIO_enablePower(GPIOA);
    DL_GPIO_enablePower(GPIOB);
    delay_cycles(POWER_STARTUP_DELAY);
}

SYSCONFIG_WEAK void SYSCFG_DL_GPIO_init(void)
{


}

static const DL_SYSCTL_SYSPLLConfig gSYSPLLConfig = {
    .inputFreq              = DL_SYSCTL_SYSPLL_INPUT_FREQ_32_48_MHZ,
	.rDivClk2x              = 1,
	.rDivClk1               = 0,
	.rDivClk0               = 1,
	.enableCLK2x            = DL_SYSCTL_SYSPLL_CLK2X_DISABLE,
	.enableCLK1             = DL_SYSCTL_SYSPLL_CLK1_DISABLE,
	.enableCLK0             = DL_SYSCTL_SYSPLL_CLK0_ENABLE,
	.sysPLLMCLK             = DL_SYSCTL_SYSPLL_MCLK_CLK0,
	.sysPLLRef              = DL_SYSCTL_SYSPLL_REF_SYSOSC,
	.qDiv                   = 9,
	.pDiv                   = DL_SYSCTL_SYSPLL_PDIV_1
};

SYSCONFIG_WEAK void SYSCFG_DL_SYSCTL_init(void)
{

    DL_SYSCTL_setPowerPolicyRUN0SLEEP0();								//将待机模式电源策略设置为 STANDBY0
    DL_SYSCTL_setBORThreshold(DL_SYSCTL_BOR_THRESHOLD_LEVEL_1);		//设置掉电重置 (BOR) 阈值水平  BOR0 门限电平。这是允许的最小阈值。
	DL_SYSCTL_setFlashWaitState(DL_SYSCTL_FLASH_WAIT_STATE_2);

    DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);			// 设置系统振荡器 (SYSOSC) 的目标频率  系统振荡器 (SYSOSC) 使用基准频率 (32MHz)
	/* Set default configuration */
	DL_SYSCTL_disableHFXT();										//禁用 HFXT 高频开关
	DL_SYSCTL_disableSYSPLL();										// Disable the SYSPLL 禁用系统级差分锁
	
//	DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_1);				//设置超低功耗时钟 (ULPCLK) 的分频器   ULPCLK 为 MCLK
//    DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);		//设置主时钟 (MCLK) 分频器 (MDIV)  禁用 MCLK 分频器。仅在 MDIV 禁用时更改 SYSOSC 频率
	
	DL_SYSCTL_configSYSPLL((DL_SYSCTL_SYSPLLConfig *) &gSYSPLLConfig);
    DL_SYSCTL_setULPCLKDivider(DL_SYSCTL_ULPCLK_DIV_2);
    DL_SYSCTL_setMCLKSource(SYSOSC, HSCLK, DL_SYSCTL_HSCLK_SOURCE_SYSPLL);
}


