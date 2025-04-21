/*
 * flash示例代码
 * S1 S2 S3分别表示字符 A B C， 
 * S15是将数据保存至flash，S16是将数据读取并显示再屏幕上
 */

#include "ti_msp_dl_config.h"
#include "oled.h"
#include "key.h"

/* flash的写入地址 */
#define MAIN_BASE_ADDRESS (0x8000)

int key_value = 0;              // 键值
char writeChar = 0;             // 待写入的字符
char readChar = 0;              // 读取的字符
uint8_t tempNum = 0;            // 临时变量，用于数据类型转换

int main(void)
{
    SYSCFG_DL_init(); // Initialize the device

    OLED_Init();
    OLED_Clear();
    OLED_ShowString(2, 0, "write char:");
    OLED_ShowString(2, 2, "read char:");

    while (1)
    {
        /* 持续读取矩阵键盘键值，直至键盘按下 */
        do
        {
            key_value = getKeyValue();
        } while (key_value == 0);
        /* 等待键盘松开 */
        while (getKeyValue() != 0)
        {
            __NOP();
        }
        /* 根据输入决定操作 */
        switch (key_value)
        {
        case 15: // 保存操作
            tempNum = (uint8_t)writeChar;   //用一个临时变量存储
            /* 先擦除 */
            DL_FlashCTL_unprotectSector(FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
            /* 再写入 */
            DL_FlashCTL_unprotectSector(FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_programMemoryFromRAM8(FLASHCTL, MAIN_BASE_ADDRESS, &tempNum);   // 地址必须是8的倍数
            /* 提示写入成功 */
            OLED_ShowString(2, 4, "write ok!");
            delay_cycles(16000000);
            OLED_ShowString(0, 4, "                  ");
            break;
        case 16: // 读取操作
            tempNum = *(uint8_t *)(MAIN_BASE_ADDRESS);  // 通过指针操作读取对应地址的数据
            readChar = (char)tempNum;                   // 将读取的uint8_t类型转换为char类型
            OLED_ShowChar(12 * 8, 2, readChar);
            break;
        case 1: // 把1 2 3分别定义为字符A B C
            writeChar = 'A';
            OLED_ShowChar(12 * 8, 0, writeChar);
            break;
        case 2:
            writeChar = 'B';
            OLED_ShowChar(12 * 8, 0, writeChar);
            break;
        case 3:
            writeChar = 'C';
            OLED_ShowChar(12 * 8, 0, writeChar);
            break;
        }
    }
}
