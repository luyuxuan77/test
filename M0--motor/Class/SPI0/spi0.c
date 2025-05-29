#include "SPI0/spi0.h"

DL_SPI_backupConfig gSPI_0Backup;
/*
 * User should take care to save and restore register configuration in application.
 * See Retention Configuration section for more details.
 */
bool SYSCFG_DL_saveConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_SPI_saveConfiguration(SPI_0_INST, &gSPI_0Backup);

    return retStatus;
}

bool SYSCFG_DL_restoreConfiguration(void)
{
    bool retStatus = true;

	retStatus &= DL_SPI_restoreConfiguration(SPI_0_INST, &gSPI_0Backup);

    return retStatus;
}
static const DL_SPI_Config gSPI_0_config = {
    .mode        = DL_SPI_MODE_CONTROLLER,
    .frameFormat = DL_SPI_FRAME_FORMAT_MOTO4_POL0_PHA0,
    .parity      = DL_SPI_PARITY_NONE,
    .dataSize    = DL_SPI_DATA_SIZE_8,
    .bitOrder    = DL_SPI_BIT_ORDER_MSB_FIRST,
    .chipSelectPin = DL_SPI_CHIP_SELECT_0,
};

static const DL_SPI_ClockConfig gSPI_0_clockConfig = {
    .clockSel    = DL_SPI_CLOCK_BUSCLK,
    .divideRatio = DL_SPI_CLOCK_DIVIDE_RATIO_1
};


void spi0_init(void)
{
	DL_SPI_reset(SPI_0_INST);
    DL_SPI_enablePower(SPI_0_INST);
	DL_GPIO_initPeripheralOutputFunction(
        GPIO_SPI_0_IOMUX_SCLK, GPIO_SPI_0_IOMUX_SCLK_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_SPI_0_IOMUX_PICO, GPIO_SPI_0_IOMUX_PICO_FUNC);
    DL_GPIO_initPeripheralInputFunction(
        GPIO_SPI_0_IOMUX_POCI, GPIO_SPI_0_IOMUX_POCI_FUNC);
    DL_GPIO_initPeripheralOutputFunction(
        GPIO_SPI_0_IOMUX_CS0, GPIO_SPI_0_IOMUX_CS0_FUNC);
	
		DL_GPIO_initDigitalOutput(OLED_OLED_DC_IOMUX);
		DL_GPIO_clearPins(OLED_PORT, OLED_OLED_DC_PIN);
		DL_GPIO_enableOutput(OLED_PORT, OLED_OLED_DC_PIN);
		DL_GPIO_setPins(OLED_PORT, OLED_OLED_DC_PIN);
	
		DL_GPIO_initDigitalOutput(GPIO_RES_PIN_0_IOMUX);
    DL_GPIO_clearPins(GPIO_RES_PORT, GPIO_RES_PIN_0_PIN);
    DL_GPIO_enableOutput(GPIO_RES_PORT, GPIO_RES_PIN_0_PIN);
		DL_GPIO_setPins(GPIO_RES_PORT, GPIO_RES_PIN_0_PIN);
	
	    /* Ensure backup structures have no valid state */
	DL_SPI_setClockConfig(SPI_0_INST, (DL_SPI_ClockConfig *) &gSPI_0_clockConfig);

    DL_SPI_init(SPI_0_INST, (DL_SPI_Config *) &gSPI_0_config);

    /* Configure Controller mode */
    /*
     * Set the bit rate clock divider to generate the serial output clock
     *     outputBitRate = (spiInputClock) / ((1 + SCR) * 2)
     *     8000000 = (32000000)/((1 + 1) * 2)
     */
    DL_SPI_setBitRateSerialClockDivider(SPI_0_INST, 4);
    /* Set RX and TX FIFO threshold levels */
    DL_SPI_setFIFOThreshold(SPI_0_INST, DL_SPI_RX_FIFO_LEVEL_1_2_FULL, DL_SPI_TX_FIFO_LEVEL_1_2_EMPTY);

    /* Enable module */
    DL_SPI_enable(SPI_0_INST);
	
	gSPI_0Backup.backupRdy 	= false;
}
