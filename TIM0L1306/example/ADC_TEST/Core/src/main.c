/*
 * （待验证）ADC单通道转换使用内部2.5V参考电压示例
 * 主函数循环启动ADC转换，OLED屏幕显示采样值
 * 旋转底板VR1滑动变阻器可以看到数值变化
 * Author: zjs
 */
#include "ti_msp_dl_config.h"
#include "oled.h"
#include "key.h"

/* 检查adc是否完成转换 */
volatile bool gCheckADC;

int main(void)
{
    /* adc转换结果 */
    uint16_t adcResult;         // adc转换结果（12bit，数字量）
    float adcResult_voltage;    // adc转换结果对应的电压值（单位:mV）

    /* 系统初始化 */
    SYSCFG_DL_init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, "ADC val:");
    OLED_ShowString(0, 2, "voltage:");
    OLED_ShowString(104, 2, "mV");
    /* 开启 adc 中断 */
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    /* 变量初始化 */
    gCheckADC = false;

    /* 确保内部参考电压在adc转换前已完成配置 */
    while (DL_VREF_CTL1_READY_NOTRDY == DL_VREF_getStatus(VREF))
        ;

    while (1)
    {
        /* 启动adc转换 */
        DL_ADC12_startConversion(ADC12_0_INST);

        /* 等待adc转换完成 */
        while (false == gCheckADC)
        {
            /* 进入低功耗模式 wait for event */
            __WFE();
        }

        /* 将adc采样值读出 */
        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

        /* 根据公式计算对应的电压值，参考电压2.5V（sysconfig里设置） */
        adcResult_voltage = adcResult * 2500.0 / (4096.0 - 1) - 0.5 * 2500.0 / 4096.0;

        /* 在OLED上显示采样值 */
        OLED_ShowNum(72, 0, adcResult, 4, 16);
        OLED_ShowNum(72, 2, (uint16_t)adcResult_voltage, 4, 16);

        /* 控制大概0.2s更新一次 */
        delay_cycles(32000000 * 0.2);

        /* 准备下一次采样 */
        gCheckADC = false;
        DL_ADC12_enableConversions(ADC12_0_INST);
    }
}

/* adc中断服务函数 */
void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST))
    {
    /* adc转换完成后进入中断 */
    case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
        gCheckADC = true;
        break;
    default:
        break;
    }
}
