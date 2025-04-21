/*
 * flashʾ������
 * S1 S2 S3�ֱ��ʾ�ַ� A B C�� 
 * S15�ǽ����ݱ�����flash��S16�ǽ����ݶ�ȡ����ʾ����Ļ��
 */

#include "ti_msp_dl_config.h"
#include "oled.h"
#include "key.h"

/* flash��д���ַ */
#define MAIN_BASE_ADDRESS (0x8000)

int key_value = 0;              // ��ֵ
char writeChar = 0;             // ��д����ַ�
char readChar = 0;              // ��ȡ���ַ�
uint8_t tempNum = 0;            // ��ʱ������������������ת��

int main(void)
{
    SYSCFG_DL_init(); // Initialize the device

    OLED_Init();
    OLED_Clear();
    OLED_ShowString(2, 0, "write char:");
    OLED_ShowString(2, 2, "read char:");

    while (1)
    {
        /* ������ȡ������̼�ֵ��ֱ�����̰��� */
        do
        {
            key_value = getKeyValue();
        } while (key_value == 0);
        /* �ȴ������ɿ� */
        while (getKeyValue() != 0)
        {
            __NOP();
        }
        /* ��������������� */
        switch (key_value)
        {
        case 15: // �������
            tempNum = (uint8_t)writeChar;   //��һ����ʱ�����洢
            /* �Ȳ��� */
            DL_FlashCTL_unprotectSector(FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
            /* ��д�� */
            DL_FlashCTL_unprotectSector(FLASHCTL, MAIN_BASE_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
            DL_FlashCTL_programMemoryFromRAM8(FLASHCTL, MAIN_BASE_ADDRESS, &tempNum);   // ��ַ������8�ı���
            /* ��ʾд��ɹ� */
            OLED_ShowString(2, 4, "write ok!");
            delay_cycles(16000000);
            OLED_ShowString(0, 4, "                  ");
            break;
        case 16: // ��ȡ����
            tempNum = *(uint8_t *)(MAIN_BASE_ADDRESS);  // ͨ��ָ�������ȡ��Ӧ��ַ������
            readChar = (char)tempNum;                   // ����ȡ��uint8_t����ת��Ϊchar����
            OLED_ShowChar(12 * 8, 2, readChar);
            break;
        case 1: // ��1 2 3�ֱ���Ϊ�ַ�A B C
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
