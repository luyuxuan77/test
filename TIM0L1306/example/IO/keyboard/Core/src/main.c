#include "ti_msp_dl_config.h"

#define CLK_HZ 32e+06 // 系统时钟

void delay_ms(int x);

uint8_t key_num = 0;
uint8_t Key();

// 无论使用哪种扫描方式，都将前四个拨码开关拨至下方
// 这样所有行被硬件上拉，如果行作为输入则不需要额外软件上拉，如果行作为输出那么上拉也不会有影响

// 矩阵键盘扫描，本例程使用行输出列输入，行扫描顺序为0111-1011-1101-1110，因此列输入需要软件上拉，
// 如果使用列输出行输入，行输入使用硬件上拉即可
int main(void)
{
  SYSCFG_DL_init();

  while (1)
  {
    key_num = Key(); // 读取按键值1-16，默认为0

    if (key_num != 0)
    {
      // key_num-1使范围变为0-15，可用2进制表示
      if ((key_num - 1) & 1) // 转为2进制后的第0位，如11=b1011，b1011&b0001=b0001，因此LED3亮，同理LED2和LED0亮，而LED1不亮
      {
        DL_GPIO_clearPins(LED_PORT, LED_LED3_PIN);
      }
      else
      {
        DL_GPIO_setPins(LED_PORT, LED_LED3_PIN);
      }
      if ((key_num - 1) & 2) // 转为2进制后的第1位
      {
        DL_GPIO_clearPins(LED_PORT, LED_LED2_PIN);
      }
      else
      {
        DL_GPIO_setPins(LED_PORT, LED_LED2_PIN);
      }
      if ((key_num - 1) & 4) // 转为2进制后的第2位
      {
        DL_GPIO_clearPins(LED_PORT, LED_LED1_PIN);
      }
      else
      {
        DL_GPIO_setPins(LED_PORT, LED_LED1_PIN);
      }
      if ((key_num - 1) & 8) // 转为2进制后的第3位
      {
        DL_GPIO_clearPins(LED_PORT, LED_LED0_PIN);
      }
      else
      {
        DL_GPIO_setPins(LED_PORT, LED_LED0_PIN);
      }

      key_num = 0; // key_num恢复默认
    }
  }
}

void delay_ms(int x)
{
  delay_cycles(CLK_HZ / 1000 * x);
}

uint8_t Key()
{
  uint8_t key_num = 0;         // 按键值1-16，默认为0
  static uint8_t key_flag = 0; // 按下按键标志

  if (key_flag)
  {
    delay_ms(300); // 300ms延迟，防止按下一次按键却被认为按下了多次按键，导致得到了多个相同的按键值
    key_flag = 0;  // 按下按键标志清零
  }

  // 行扫描
  // ROW 0111
  // 0111-1011-1101-1110
  DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN);
  DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW1_PIN | MAT_KEY_ROW2_PIN | MAT_KEY_ROW3_PIN);
  delay_ms(10); // 按键消抖延迟，一般为10ms，一定要在外界改变时延迟，而不是读取输入后再延迟
                // 所谓的外界改变时，即改变行扫描时，如行输出由0111变为1011
                // 这里实际上有两个外界改变时，一个是用户刚刚按下时，另一个是改变行扫描时
                // 然而由于按键消抖延迟只需10ms，扫描一轮也只需40ms，远小于用户按下的时间
                // 因此选择改变行扫描时作为外界改变时，总能涵盖两种外界改变
                // 并且在按键消抖例程中提到，无法得知用户刚刚按下的时刻
                // 同时不断判断低电平也不能近似这个时刻，因为低电平出现有两种情况
                // 一种是用户先按下，然后改变行扫描，另一种是先改变行扫描，用户再按下
                // 普遍错误写法如下:
  // DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN);
  // DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW1_PIN | MAT_KEY_ROW2_PIN | MAT_KEY_ROW3_PIN);

  // if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN))//每次改变行扫描后，直接读取并未经过延迟，有可能只能读到抖动中的高电平，从而永远检测不到，如常见的第二行“不灵敏”问题，究其原因是134行过于灵敏导致消抖错误也能检测正确
  // {
  //   delay_ms(10);
  //   if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN))
  //   {
  //     key_num = 1;
  //     key_flag = 1;
  //   }
  // }

  if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN))
  {

    key_num = 1;
    key_flag = 1; // 按键已按下
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN))
  {
    key_num = 2;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN))
  {
    key_num = 3;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN))
  {
    key_num = 4;
    key_flag = 1;
  }

  // ROW 1011
  DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW1_PIN);
  DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN | MAT_KEY_ROW2_PIN | MAT_KEY_ROW3_PIN);
  delay_ms(10);

  if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN))
  {
    key_num = 5;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN))
  {
    key_num = 6;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN))
  {
    key_num = 7;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN))
  {
    key_num = 8;
    key_flag = 1;
  }

  // ROW 1101
  DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW2_PIN);
  DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN | MAT_KEY_ROW1_PIN | MAT_KEY_ROW3_PIN);
  delay_ms(10);

  if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN))
  {
    key_num = 9;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN))
  {
    key_num = 10;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN))
  {
    key_num = 11;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN))
  {
    key_num = 12;
    key_flag = 1;
  }

  DL_GPIO_clearPins(MAT_KEY_PORT, MAT_KEY_ROW3_PIN);
  DL_GPIO_setPins(MAT_KEY_PORT, MAT_KEY_ROW0_PIN | MAT_KEY_ROW1_PIN | MAT_KEY_ROW2_PIN);
  delay_ms(10);

  // ROW 1110
  if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL0_PIN))
  {
    key_num = 13;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL1_PIN))
  {
    key_num = 14;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL2_PIN))
  {
    key_num = 15;
    key_flag = 1;
  }
  else if (!DL_GPIO_readPins(MAT_KEY_PORT, MAT_KEY_COL3_PIN))
  {
    key_num = 16;
    key_flag = 1;
  }

  return key_num;
}