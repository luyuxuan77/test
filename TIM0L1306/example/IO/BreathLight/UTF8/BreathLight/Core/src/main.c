#include "ti_msp_dl_config.h"

#define CLK_HZ 32e+06 // 系统时钟

int sin_tab[256] = {128, 131, 134, 137, 141, 144, 147, 150, 153, 156, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 188, 191, 194, 196, 199, 202, 204, 207, 209, 212, 214, 216, 219, 221, 223, 225, 227, 229, 231, 233, 234, 236, 238, 239, 241, 242, 244, 245, 246, 247, 249, 250, 250, 251, 252, 253, 254, 254, 255, 255, 255, 256, 256, 256, 256, 256, 256, 256, 255, 255, 255, 254, 254, 253, 252, 251, 250, 250, 249, 247, 246, 245, 244, 242, 241, 239, 238, 236, 234, 233, 231, 229, 227, 225, 223, 221, 219, 216, 214, 212, 209, 207, 204, 202, 199, 196, 194, 191, 188, 186, 183, 180, 177, 174, 171, 168, 165, 162, 159, 156, 153, 150, 147, 144, 141, 137, 134, 131, 128, 125, 122, 119, 115, 112, 109, 106, 103, 100, 97, 94, 91, 88, 85, 82, 79, 76, 73, 70, 68, 65, 62, 60, 57, 54, 52, 49, 47, 44, 42, 40, 37, 35, 33, 31, 29, 27, 25, 23, 22, 20, 18, 17, 15, 14, 12, 11, 10, 9, 7, 6, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 6, 6, 7, 9, 10, 11, 12, 14, 15, 17, 18, 20, 22, 23, 25, 27, 29, 31, 33, 35, 37, 40, 42, 44, 47, 49, 52, 54, 57, 60, 62, 65, 68, 70, 73, 76, 79, 82, 85, 88, 91, 94, 97, 100, 103, 106, 109, 112, 115, 119, 122, 125};

void delay_ms(int x);

int main(void)
{
	SYSCFG_DL_init();

	// Timer和PWM的计数均在syscfg中已启动
	// Timer为256Hz；PWM为15625Hz，一周期计数值为256
	// 实测发现Edge-aligned Down Counting中设置比较值为256，得到的占空比为100%，而不是syscfg中计算出的0%
	// 这样需要对比较值进行重新排序：256,0,1,2,...,254,255
	// 因此使用Edge-aligned Up Counting，并Invert Channel使占空比为100%对应灯最亮

	// 测试比较值为255,0,256下的亮度
	DL_TimerG_setCaptureCompareValue(PWM_0_INST, 255, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	delay_ms(500);
	DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	delay_ms(500);
	DL_TimerG_setCaptureCompareValue(PWM_0_INST, 256, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	delay_ms(500);

	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN); // 定时器中断使能，256Hz

	while (1)
	{
	}
}
void TIMER_0_INST_IRQHandler(void)
{
	static uint8_t i = 0;
	i = (i + 1) % 256; // 只设置i为0-255，不设置256，因为定时器频率为256Hz。

	// 占空比三角波变化
	static uint8_t direction_up = 0; // 变亮1s，变暗1s，呼吸灯周期为2s
	if (i == 0)
	{
		direction_up = !direction_up;
	}

	if (direction_up) // 变亮，从0-255
	{
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, i, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	}
	else // 变暗，从256-1
	{
		DL_TimerG_setCaptureCompareValue(PWM_0_INST, 256 - i, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
	}

	// 占空比正弦波变化
	// 正弦波表长度为256，呼吸灯周期为1s
	// DL_TimerG_setCaptureCompareValue(PWM_0_INST, sin_tab[i], DL_TIMERG_CAPTURE_COMPARE_0_INDEX);
}
void delay_ms(int x)
{
	delay_cycles(CLK_HZ / 1000 * x);
}