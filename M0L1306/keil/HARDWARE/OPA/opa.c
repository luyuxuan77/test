#include "opa.h"
#include "bsp.h"

/* COMP_0 Initialization */
static const DL_COMP_Config gCOMP_0Config = {
    .channelEnable = DL_COMP_ENABLE_CHANNEL_POS_NEG,
    .mode          = DL_COMP_MODE_FAST,
    .negChannel    = DL_COMP_IMSEL_CHANNEL_0,
    .posChannel    = DL_COMP_IPSEL_CHANNEL_0,
    .hysteresis    = DL_COMP_HYSTERESIS_NONE,
    .polarity      = DL_COMP_POLARITY_NON_INV
};
static const DL_COMP_RefVoltageConfig gCOMP_0VRefConfig = {
    .mode           = DL_COMP_REF_MODE_STATIC,
    .source         = DL_COMP_REF_SOURCE_NONE,
    .terminalSelect = DL_COMP_REF_TERMINAL_SELECT_POS,
    .controlSelect  = DL_COMP_DAC_CONTROL_COMP_OUT,
    .inputSelect    = DL_COMP_DAC_INPUT_DACCODE0
};
static const DL_OPA_Config gOPA_0Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};

static const DL_OPA_Config gOPA_1Config0 = {
    .pselChannel    = DL_OPA_PSEL_IN0_POS,
    .nselChannel    = DL_OPA_NSEL_IN0_NEG,
    .mselChannel    = DL_OPA_MSEL_OPEN,
    .gain           = DL_OPA_GAIN_N0_P1,
    .outputPinState = DL_OPA_OUTPUT_PIN_ENABLED,
    .choppingMode   = DL_OPA_CHOPPING_MODE_DISABLE,
};
static const DL_GPAMP_Config gGPAMP_0Config = {
    .pselChannel    = DL_GPAMP_PSEL_IN_POS,
    .nselChannel    = DL_GPAMP_NSEL_IN_NEG,
    .outputPinState = DL_GPAMP_OUTPUT_PIN_STATE_ENABLED,
    .choppingMode   = DL_GPAMP_CHOPPING_MODE_DISABLED,
    .choppingFreq   = DL_GPAMP_CHOPPING_FREQ_16KHZ,
};

SYSCONFIG_WEAK void SYSCFG_DL_GPAMP_0_init(void)
{
    DL_GPAMP_init((DL_GPAMP_Config *) &gGPAMP_0Config);
    DL_GPAMP_enable();
}

void opa_init(void)
{
     DL_GPIO_reset(GPIOA);
     DL_OPA_reset(OPA_0_INST);
     DL_OPA_reset(OPA_1_INST);
     DL_COMP_reset(COMP_0_INST);


     DL_GPIO_enablePower(GPIOA);
     DL_OPA_enablePower(OPA_0_INST);
     DL_OPA_enablePower(OPA_1_INST);
     DL_COMP_enablePower(COMP_0_INST);

     delay_cycles(POWER_STARTUP_DELAY);
     
	 DL_GPIO_setAnalogInternalResistor(GPIO_OPA_0_IOMUX_OUT, DL_GPIO_RESISTOR_NONE);
	 DL_GPIO_setAnalogInternalResistor(GPIO_OPA_0_IOMUX_IN0POS, DL_GPIO_RESISTOR_NONE);
	 DL_GPIO_setAnalogInternalResistor(GPIO_OPA_0_IOMUX_IN0NEG, DL_GPIO_RESISTOR_NONE);
	 DL_GPIO_setAnalogInternalResistor(GPIO_OPA_1_IOMUX_IN0POS, DL_GPIO_RESISTOR_NONE);
	 DL_GPIO_setAnalogInternalResistor(GPIO_OPA_1_IOMUX_OUT, DL_GPIO_RESISTOR_NONE);
	 DL_GPIO_setAnalogInternalResistor(GPIO_OPA_1_IOMUX_IN0NEG, DL_GPIO_RESISTOR_NONE);
//	 DL_GPIO_setAnalogInternalResistor(GPIO_GPAMP_0_IOMUX_INPOS, DL_GPIO_RESISTOR_PULL_DOWN);
//	 DL_GPIO_setAnalogInternalResistor(GPIO_GPAMP_0_IOMUX_INNEG, DL_GPIO_RESISTOR_PULL_DOWN);
//	 DL_GPIO_setAnalogInternalResistor(GPIO_GPAMP_0_IOMUX_OUT, DL_GPIO_RESISTOR_PULL_DOWN);

    DL_GPIO_initPeripheralOutputFunction(
        GPIO_COMP_0_IOMUX_OUT, GPIO_COMP_0_IOMUX_OUT_FUNC);
    
//	DL_GPIO_setAnalogInternalResistor(GPIO_GPAMP_0_IOMUX_INPOS, DL_GPIO_RESISTOR_PULL_DOWN);
//	DL_GPIO_setAnalogInternalResistor(GPIO_GPAMP_0_IOMUX_OUT, DL_GPIO_RESISTOR_NONE);
//	DL_GPIO_setAnalogInternalResistor(GPIO_GPAMP_0_IOMUX_INNEG, DL_GPIO_RESISTOR_NONE);
    DL_OPA_init(OPA_0_INST, (DL_OPA_Config *) &gOPA_0Config0);
    DL_OPA_init(OPA_1_INST, (DL_OPA_Config *) &gOPA_1Config0);

    DL_COMP_init(COMP_0_INST, (DL_COMP_Config *) &gCOMP_0Config);
    DL_COMP_refVoltageInit(COMP_0_INST, (DL_COMP_RefVoltageConfig *) &gCOMP_0VRefConfig);
    DL_COMP_enableShortInputTerminals(COMP_0_INST);

    
    DL_OPA_enable(OPA_0_INST);
    DL_OPA_enable(OPA_1_INST);
    DL_COMP_enable(COMP_0_INST);


}