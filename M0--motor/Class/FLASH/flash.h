#ifndef _FLASH_H_
#define _FLASH_H_
#include <ti/driverlib/driverlib.h>

void flash_init(void);

uint16_t falsh_u16_read(uint16_t *data,uint64_t address);
uint16_t flash_u16_write(uint16_t data,uint64_t address);


#endif