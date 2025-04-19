#include "bsp.h"

/* Address in main memory to write to */
/* 一个扇区大小1KB */
#define MAIN_BASE_ADDRESS (0x00001000)


/*flash操作不需要初始化*/
void flash_init(void)
{
//	/* 将指定扇区设置为不受保护 */
//	DL_FlashCTL_unprotectSector(
//		FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);	
//	/* 擦除不受保护的指定扇区 */
//	gCmdStatus = DL_FlashCTL_eraseMemoryFromRAM(
//		FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);	
}



uint16_t flash_u16_write(uint16_t data,uint64_t address)
{
	/* 操作结果标记 */
	volatile DL_FLASHCTL_COMMAND_STATUS flag;
	
	/* 将指定扇区设置为不受保护 */
	DL_FlashCTL_unprotectSector(
		FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);	
	
	/* 擦除不受保护的指定扇区 */
	flag = DL_FlashCTL_eraseMemoryFromRAM(
			FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
	
	/* 将指定扇区设置为不受保护 */
	DL_FlashCTL_unprotectSector( 
            FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
	/* 向扇区指定地址写入16位数据 */
	flag = DL_FlashCTL_programMemoryFromRAM16WithECCGenerated(
            FLASHCTL, (MAIN_BASE_ADDRESS + address), &data);
	
	/* 返回操作结果 成功(0x003)/失败(其他) */
	return flag;
}

uint16_t falsh_u16_read(uint16_t *data,uint64_t address)
{
	/* 操作结果标记 */
	volatile DL_FLASHCTL_COMMAND_STATUS flag;
	
	/*保存指定位置数据*/
	uint16_t data1=0x000;
	
	/* 将指定扇区设置为不受保护 */
	DL_FlashCTL_unprotectSector(
	FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
	
	/*检索指定位置数据*/
	for(data1=0x000;data1<0xfff;data1+=0x001)
	{
		flag = DL_FlashCTL_readVerifyFromRAM16WithECCGenerated(
           FLASHCTL, (MAIN_BASE_ADDRESS + address ), &data1);
		if(flag==0x03) break; 
	}
	
	 *data=data1;

	/* 返回操作结果 成功(0x003)/失败(其他) */
	return flag;
}






