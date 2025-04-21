#include "ti_msp_dl_config.h"

uint8_t clk_flag = 1;     // 切换时钟标志
uint8_t clk_ch = 0;       // 时钟选择
uint32_t clk_hz = 32e+06; // 系统时钟

void delay_ms(int x);

int main(void)
{
  SYSCFG_DL_init();

  // MFCLK使能时MDIV分频无效，已在syscfg取消MFCLK使能

  NVIC_EnableIRQ(KEY_INT_IRQN); // IO中断使能

  while (1)
  {
    if (clk_flag) // 在while(1)中切换时钟是因为如果切换时钟时正运行着delay函数，会改变delay函数的时间
    {
      switch (clk_ch)
      {
      case 0: // 32M
        DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 32e+06;

        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE); // MDIV不分频
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);      // 选择32MHz作为SYSOSC
        DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC();                  // 将MCLK时钟源由LFCLK变为SYSOSC

        break;
      case 1: // 4M
        DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 4e+06;

        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_4M); // 选择4MHz作为SYSOSC
        DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC();
        break;
      case 2: // 1M
        DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 1e+06;

        // 注：如果想将主时钟固定为1MHz，而不是此例程演示的切换时钟
        // 触发按键中断后MCLK会变回4M，需要重新配置为1M
        // DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
        // DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_4M);
        // DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_4);
        // 原因可能是异步快速时钟请求，感兴趣的同学可以自己尝试关闭（我不会）
        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE); // MDIV不分频时才能设置分频系数
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_4M);        // SYSOSC为4MHz时才能通过MDIV分频
        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_4);       // MDIV分频系数设为4
        DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC();
        break;
      case 3: // 32.768K
        DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 32768;

        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE); // MDIV不分频时才能切换MCLK时钟源
        DL_SYSCTL_switchMCLKfromSYSOSCtoLFCLK(false);             // 将MCLK时钟源由SYSOSC变为LFCLK
        break;
      default:
        break;
      }
      clk_flag = 0;
    }

    // 1Hz闪烁
    delay_ms(500);
    DL_GPIO_togglePins(LED_PORT, LED_USER_PIN);
  }
}

void GROUP1_IRQHandler(void)
{
  switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
  {
  case KEY_INT_IIDX:
    delay_ms(10); // 按键消抖延迟，一般为10ms
    // IO中断也需要按键消抖，因为如果抖动存在，那么松开按键产生的抖动也存在下降沿
    // 意味着按下和松开都有机会触发中断进入中断服务函数
    // 延迟之后检测读取的值，若为0，说明这次中断是按下触发的，若为1，说明这次中断是松开触发的
    // 核心板上的按键抖动较小，所以不消抖也能正常使用，矩阵键盘和薄膜键盘就未必有很好的效果，因此推荐进行消抖
    if (!DL_GPIO_readPins(KEY_PORT, KEY_KEY0_PIN))
    {
      clk_ch = (clk_ch + 1) % 4; // 切换时钟选择
      clk_flag = 1;
    }
    break;
  default:
    break;
  }
}

void delay_ms(int x)
{
  delay_cycles(1.0 * clk_hz / 1000 * x); // 修改系统时钟时修改clk_hz，使delay_ms始终延迟1ms
}