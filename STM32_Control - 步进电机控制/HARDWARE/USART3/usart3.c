#include "usart3.h"
#include "led.h"
					 
//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void usart3_init(u32 pclk2,u32 bound)
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // 得到USARTDIV@OVER8=0
	mantissa = temp;  // 得到整数部分
	fraction = (temp-mantissa)*16;  // 得到小数部分@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<1;  // 使能PORTB口时钟  
	RCC->APB1ENR |= 1<<18;  // 使能串口3时钟 
	GPIO_Set(GPIOB, PIN10|PIN11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PA9,PA10,复用功能,上拉输出
 	GPIO_AF_Set(GPIOB, 10, 7);	// PB10,AF7
	GPIO_AF_Set(GPIOB, 11, 7);	// PB11,AF7  	   

 	
	USART3->BRR=mantissa;  // 波特率设置	 
	USART3->CR1 &= ~(1<<15);  // 设置OVER8=0 
	USART3->CR1 |= 1<<3;  // 串口发送使能 
	//使能接收中断 
	USART3->CR1 |= 1<<2;  // 串口接收使能
	USART3->CR1 |= 1<<5;  // 接收缓冲区非空中断使能	    	
	MY_NVIC_Init(1, 3, USART3_IRQn, 2);  // 组2，最低优先级 
	USART3->CR1 |= 1<<13;  // 串口使能
	
	
/**********************************************************
***	清除USART1中断
**********************************************************/
	USART3->SR; USART3->DR;
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);

/**********************************************************
***	使能USART1中断
**********************************************************/	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
}


__IO bool rxFrameFlag = false;
__IO uint8_t rxCmd[FIFO_SIZE] = {0};
__IO uint8_t rxCount = 0;
__IO uint8_t u_rxCount = 0;
float pos = 0.0f, Motor_Cur_Pos = 0.0f, vel = 0.0f, Motor_Cur_Vel = 0.0f;

float Read_Motor_Cur_Pos(void)
{
	/**********************************************************
	***	等待返回命令，命令数据缓存在数组rxCmd上，长度为rxCount
	**********************************************************/	
//	while(rxFrameFlag == false){}; rxFrameFlag = false;
	/**********************************************************
	***	校验地址、功能码、返回数据长度，校验成功则计算当前位置角度
	**********************************************************/	
	if(rxCmd[0] == 1 && rxCmd[1] == 0x36 && rxCount == 8)
	{
	// 拼接成uint32_t类型
	pos = (uint32_t)(
					  ((uint32_t)rxCmd[3] << 24)    |
					  ((uint32_t)rxCmd[4] << 16)    |
					  ((uint32_t)rxCmd[5] << 8)     |
					  ((uint32_t)rxCmd[6] << 0)
					);

	// 转换成角度
	Motor_Cur_Pos = (float)pos * 360.0f / 65536.0f;

	// 符号
	if(rxCmd[2]) { Motor_Cur_Pos = -Motor_Cur_Pos; }
	}
	return Motor_Cur_Pos;
}


float Read_Motor_Cur_Vel(void)
{
	/**********************************************************
	***	等待返回命令，命令数据缓存在数组rxCmd上，长度为rxCount
	**********************************************************/	
//	while(rxFrameFlag == false){}; rxFrameFlag = false;
	/**********************************************************
	***	校验地址、功能码、返回数据长度，校验成功则计算当前位置角度
	**********************************************************/	
	if(rxCmd[0] == 1 && rxCmd[1] == 0x35 && rxCount == 6)
	{
	// 拼接成uint32_t类型
	vel = (uint32_t)(
					  ((uint32_t)rxCmd[3] << 8)    |
					  ((uint32_t)rxCmd[4] << 0)    
					);

	// 转换成速度
	Motor_Cur_Vel = (float)vel;

	// 符号
	if(rxCmd[2]) { Motor_Cur_Vel = -Motor_Cur_Vel; }
	}
	return Motor_Cur_Vel;
}



/**
	* @brief   USART1中断函数
	* @param   无
	* @retval  无
	*/
void USART3_IRQHandler(void)
{
	__IO uint16_t i = 0;
	
	
/**********************************************************
***	串口接收中断
**********************************************************/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		// 未完成一帧数据接收，数据进入缓冲队列
		fifo_enQueue((uint8_t)USART3->DR);

		// 清除串口接收中断
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}

/**********************************************************
***	串口空闲中断
**********************************************************/
	else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		// 先读SR再读DR，清除IDLE中断
		USART3->SR; USART3->DR;

		// 提取一帧数据命令
		rxCount = fifo_queueLength(); for(i=0; i < rxCount; i++) { rxCmd[i] = fifo_deQueue(); }

		// 一帧数据接收完成，置位帧标志位
		rxFrameFlag = true;
	}
}

/**
	* @brief   USART发送多个字节
	* @param   无
	* @retval  无
	*/
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
	__IO uint8_t i = 0;
	
	for(i=0; i < len; i++) { usart_SendByte(cmd[i]); }
}

/**
	* @brief   USART发送一个字节
	* @param   无
	* @retval  无
	*/
void usart_SendByte(uint16_t data)
{
	__IO uint16_t t0 = 0;
	
	USART3->DR = (data & (uint16_t)0x01FF);

	while(!(USART3->SR & USART_FLAG_TXE))
	{
		++t0; if(t0 > 8000)	{	return; }
	}
}











/**
  * @brief  Clears the USARTx's interrupt pending bits.
  * @param  USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3, UART4 or UART5.
  * @param  USART_IT: specifies the interrupt pending bit to clear.
  *   This parameter can be one of the following values:
  *     @arg USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
  *     @arg USART_IT_LBD:  LIN Break detection interrupt
  *     @arg USART_IT_TC:   Transmission complete interrupt. 
  *     @arg USART_IT_RXNE: Receive Data register not empty interrupt.
  *   
  * @note
  *   - PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
  *     error) and IDLE (Idle line detected) pending bits are cleared by 
  *     software sequence: a read operation to USART_SR register 
  *     (USART_GetITStatus()) followed by a read operation to USART_DR register 
  *     (USART_ReceiveData()).
  *   - RXNE pending bit can be also cleared by a read to the USART_DR register 
  *     (USART_ReceiveData()).
  *   - TC pending bit can be also cleared by software sequence: a read 
  *     operation to USART_SR register (USART_GetITStatus()) followed by a write 
  *     operation to USART_DR register (USART_SendData()).
  *   - TXE pending bit is cleared only by a write to the USART_DR register 
  *     (USART_SendData()).
  * @retval None
//  */
void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  uint16_t bitpos = 0x00, itmask = 0x00;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_IT(USART_IT));
  /* The CTS interrupt is not available for UART4 and UART5 */
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  bitpos = USART_IT >> 0x08;
  itmask = ((uint16_t)0x01 << (uint16_t)bitpos);
  USARTx->SR = (uint16_t)~itmask;
}


/**
  * @brief  Enables or disables the specified USART interrupts.
  * @param  USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3, UART4 or UART5.
  * @param  USART_IT: specifies the USART interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
  *     @arg USART_IT_LBD:  LIN Break detection interrupt
  *     @arg USART_IT_TXE:  Transmit Data Register empty interrupt
  *     @arg USART_IT_TC:   Transmission complete interrupt
  *     @arg USART_IT_RXNE: Receive Data register not empty interrupt
  *     @arg USART_IT_IDLE: Idle line detection interrupt
  *     @arg USART_IT_PE:   Parity Error interrupt
  *     @arg USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @param  NewState: new state of the specified USARTx interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
//  */
void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState)
{
  uint32_t usartreg = 0x00, itpos = 0x00, itmask = 0x00;
  uint32_t usartxbase = 0x00;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  /* The CTS interrupt is not available for UART4 and UART5 */
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  usartxbase = (uint32_t)USARTx;

  /* Get the USART register index */
  usartreg = (((uint8_t)USART_IT) >> 0x05);

  /* Get the interrupt position */
  itpos = USART_IT & IT_Mask;
  itmask = (((uint32_t)0x01) << itpos);
    
  if (usartreg == 0x01) /* The IT is in CR1 register */
  {
    usartxbase += 0x0C;
  }
  else if (usartreg == 0x02) /* The IT is in CR2 register */
  {
    usartxbase += 0x10;
  }
  else /* The IT is in CR3 register */
  {
    usartxbase += 0x14; 
  }
  if (NewState != DISABLE)
  {
    *(__IO uint32_t*)usartxbase  |= itmask;
  }
  else
  {
    *(__IO uint32_t*)usartxbase &= ~itmask;
  }
}

/**
  * @brief  Checks whether the specified USART interrupt has occurred or not.
  * @param  USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3, UART4 or UART5.
  * @param  USART_IT: specifies the USART interrupt source to check.
  *   This parameter can be one of the following values:
  *     @arg USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
  *     @arg USART_IT_LBD:  LIN Break detection interrupt
  *     @arg USART_IT_TXE:  Tansmit Data Register empty interrupt
  *     @arg USART_IT_TC:   Transmission complete interrupt
  *     @arg USART_IT_RXNE: Receive Data register not empty interrupt
  *     @arg USART_IT_IDLE: Idle line detection interrupt
  *     @arg USART_IT_ORE:  OverRun Error interrupt
  *     @arg USART_IT_NE:   Noise Error interrupt
  *     @arg USART_IT_FE:   Framing Error interrupt
  *     @arg USART_IT_PE:   Parity Error interrupt
  * @retval The new state of USART_IT (SET or RESET).
  */
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  uint32_t bitpos = 0x00, itmask = 0x00, usartreg = 0x00;
  ITStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_GET_IT(USART_IT));
  /* The CTS interrupt is not available for UART4 and UART5 */ 
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  /* Get the USART register index */
  usartreg = (((uint8_t)USART_IT) >> 0x05);
  /* Get the interrupt position */
  itmask = USART_IT & IT_Mask;
  itmask = (uint32_t)0x01 << itmask;
  
  if (usartreg == 0x01) /* The IT  is in CR1 register */
  {
    itmask &= USARTx->CR1;
  }
  else if (usartreg == 0x02) /* The IT  is in CR2 register */
  {
    itmask &= USARTx->CR2;
  }
  else /* The IT  is in CR3 register */
  {
    itmask &= USARTx->CR3;
  }
  
  bitpos = USART_IT >> 0x08;
  bitpos = (uint32_t)0x01 << bitpos;
  bitpos &= USARTx->SR;
  if ((itmask != (uint16_t)RESET)&&(bitpos != (uint16_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  
  return bitstatus;  
}
