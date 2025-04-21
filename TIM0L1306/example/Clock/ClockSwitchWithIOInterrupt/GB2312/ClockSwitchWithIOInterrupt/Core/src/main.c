#include "ti_msp_dl_config.h"

uint8_t clk_flag = 1;     // �л�ʱ�ӱ�־
uint8_t clk_ch = 0;       // ʱ��ѡ��
uint32_t clk_hz = 32e+06; // ϵͳʱ��

void delay_ms(int x);

int main(void)
{
  SYSCFG_DL_init();

  // MFCLKʹ��ʱMDIV��Ƶ��Ч������syscfgȡ��MFCLKʹ��

  NVIC_EnableIRQ(KEY_INT_IRQN); // IO�ж�ʹ��

  while (1)
  {
    if (clk_flag) // ��while(1)���л�ʱ������Ϊ����л�ʱ��ʱ��������delay��������ı�delay������ʱ��
    {
      switch (clk_ch)
      {
      case 0: // 32M
        DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 32e+06;

        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE); // MDIV����Ƶ
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_BASE);      // ѡ��32MHz��ΪSYSOSC
        DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC();                  // ��MCLKʱ��Դ��LFCLK��ΪSYSOSC

        break;
      case 1: // 4M
        DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 4e+06;

        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_4M); // ѡ��4MHz��ΪSYSOSC
        DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC();
        break;
      case 2: // 1M
        DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 1e+06;

        // ע������뽫��ʱ�ӹ̶�Ϊ1MHz�������Ǵ�������ʾ���л�ʱ��
        // ���������жϺ�MCLK����4M����Ҫ��������Ϊ1M
        // DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE);
        // DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_4M);
        // DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_4);
        // ԭ��������첽����ʱ�����󣬸���Ȥ��ͬѧ�����Լ����Թرգ��Ҳ��ᣩ
        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE); // MDIV����Ƶʱ�������÷�Ƶϵ��
        DL_SYSCTL_setSYSOSCFreq(DL_SYSCTL_SYSOSC_FREQ_4M);        // SYSOSCΪ4MHzʱ����ͨ��MDIV��Ƶ
        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_4);       // MDIV��Ƶϵ����Ϊ4
        DL_SYSCTL_switchMCLKfromLFCLKtoSYSOSC();
        break;
      case 3: // 32.768K
        DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
        DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);

        clk_hz = 32768;

        DL_SYSCTL_setMCLKDivider(DL_SYSCTL_MCLK_DIVIDER_DISABLE); // MDIV����Ƶʱ�����л�MCLKʱ��Դ
        DL_SYSCTL_switchMCLKfromSYSOSCtoLFCLK(false);             // ��MCLKʱ��Դ��SYSOSC��ΪLFCLK
        break;
      default:
        break;
      }
      clk_flag = 0;
    }

    // 1Hz��˸
    delay_ms(500);
    DL_GPIO_togglePins(LED_PORT, LED_USER_PIN);
  }
}

void GROUP1_IRQHandler(void)
{
  switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1))
  {
  case KEY_INT_IIDX:
    delay_ms(10); // ���������ӳ٣�һ��Ϊ10ms
    // IO�ж�Ҳ��Ҫ������������Ϊ����������ڣ���ô�ɿ����������Ķ���Ҳ�����½���
    // ��ζ�Ű��º��ɿ����л��ᴥ���жϽ����жϷ�����
    // �ӳ�֮�����ȡ��ֵ����Ϊ0��˵������ж��ǰ��´����ģ���Ϊ1��˵������ж����ɿ�������
    // ���İ��ϵİ���������С�����Բ�����Ҳ������ʹ�ã�������̺ͱ�Ĥ���̾�δ���кܺõ�Ч��������Ƽ���������
    if (!DL_GPIO_readPins(KEY_PORT, KEY_KEY0_PIN))
    {
      clk_ch = (clk_ch + 1) % 4; // �л�ʱ��ѡ��
      clk_flag = 1;
    }
    break;
  default:
    break;
  }
}

void delay_ms(int x)
{
  delay_cycles(1.0 * clk_hz / 1000 * x); // �޸�ϵͳʱ��ʱ�޸�clk_hz��ʹdelay_msʼ���ӳ�1ms
}