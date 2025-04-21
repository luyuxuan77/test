/*
 * ������֤��ADC��ͨ��ת��ʹ���ڲ�2.5V�ο���ѹʾ��
 * ������ѭ������ADCת����OLED��Ļ��ʾ����ֵ
 * ��ת�װ�VR1�������������Կ�����ֵ�仯
 * Author: zjs
 */
#include "ti_msp_dl_config.h"
#include "oled.h"
#include "key.h"

/* ���adc�Ƿ����ת�� */
volatile bool gCheckADC;

int main(void)
{
    /* adcת����� */
    uint16_t adcResult;         // adcת�������12bit����������
    float adcResult_voltage;    // adcת�������Ӧ�ĵ�ѹֵ����λ:mV��

    /* ϵͳ��ʼ�� */
    SYSCFG_DL_init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0, 0, "ADC val:");
    OLED_ShowString(0, 2, "voltage:");
    OLED_ShowString(104, 2, "mV");
    /* ���� adc �ж� */
    NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);

    /* ������ʼ�� */
    gCheckADC = false;

    /* ȷ���ڲ��ο���ѹ��adcת��ǰ��������� */
    while (DL_VREF_CTL1_READY_NOTRDY == DL_VREF_getStatus(VREF))
        ;

    while (1)
    {
        /* ����adcת�� */
        DL_ADC12_startConversion(ADC12_0_INST);

        /* �ȴ�adcת����� */
        while (false == gCheckADC)
        {
            /* ����͹���ģʽ wait for event */
            __WFE();
        }

        /* ��adc����ֵ���� */
        adcResult = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);

        /* ���ݹ�ʽ�����Ӧ�ĵ�ѹֵ���ο���ѹ2.5V��sysconfig�����ã� */
        adcResult_voltage = adcResult * 2500.0 / (4096.0 - 1) - 0.5 * 2500.0 / 4096.0;

        /* ��OLED����ʾ����ֵ */
        OLED_ShowNum(72, 0, adcResult, 4, 16);
        OLED_ShowNum(72, 2, (uint16_t)adcResult_voltage, 4, 16);

        /* ���ƴ��0.2s����һ�� */
        delay_cycles(32000000 * 0.2);

        /* ׼����һ�β��� */
        gCheckADC = false;
        DL_ADC12_enableConversions(ADC12_0_INST);
    }
}

/* adc�жϷ����� */
void ADC12_0_INST_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST))
    {
    /* adcת����ɺ�����ж� */
    case DL_ADC12_IIDX_MEM0_RESULT_LOADED:
        gCheckADC = true;
        break;
    default:
        break;
    }
}
