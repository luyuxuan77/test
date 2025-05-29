#include "usart3.h"
#include "led.h"
					 
//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void usart3_init(u32 pclk2,u32 bound)
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp = (float)(pclk2*1000000)/(bound*16);  // �õ�USARTDIV@OVER8=0
	mantissa = temp;  // �õ���������
	fraction = (temp-mantissa)*16;  // �õ�С������@OVER8=0 
    mantissa <<= 4;
	mantissa += fraction; 
	
	RCC->AHB1ENR |= 1<<1;  // ʹ��PORTB��ʱ��  
	RCC->APB1ENR |= 1<<18;  // ʹ�ܴ���3ʱ�� 
	GPIO_Set(GPIOB, PIN10|PIN11, GPIO_MODE_AF, GPIO_OTYPE_PP, GPIO_SPEED_50M, GPIO_PUPD_PU);  // PA9,PA10,���ù���,�������
 	GPIO_AF_Set(GPIOB, 10, 7);	// PB10,AF7
	GPIO_AF_Set(GPIOB, 11, 7);	// PB11,AF7  	   

 	
	USART3->BRR=mantissa;  // ����������	 
	USART3->CR1 &= ~(1<<15);  // ����OVER8=0 
	USART3->CR1 |= 1<<3;  // ���ڷ���ʹ�� 
	//ʹ�ܽ����ж� 
	USART3->CR1 |= 1<<2;  // ���ڽ���ʹ��
	USART3->CR1 |= 1<<5;  // ���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(1, 3, USART3_IRQn, 2);  // ��2��������ȼ� 
	USART3->CR1 |= 1<<13;  // ����ʹ��
	
	
/**********************************************************
***	���USART1�ж�
**********************************************************/
	USART3->SR; USART3->DR;
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);

/**********************************************************
***	ʹ��USART1�ж�
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
	***	�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	**********************************************************/	
//	while(rxFrameFlag == false){}; rxFrameFlag = false;
	/**********************************************************
	***	У���ַ�������롢�������ݳ��ȣ�У��ɹ�����㵱ǰλ�ýǶ�
	**********************************************************/	
	if(rxCmd[0] == 1 && rxCmd[1] == 0x36 && rxCount == 8)
	{
	// ƴ�ӳ�uint32_t����
	pos = (uint32_t)(
					  ((uint32_t)rxCmd[3] << 24)    |
					  ((uint32_t)rxCmd[4] << 16)    |
					  ((uint32_t)rxCmd[5] << 8)     |
					  ((uint32_t)rxCmd[6] << 0)
					);

	// ת���ɽǶ�
	Motor_Cur_Pos = (float)pos * 360.0f / 65536.0f;

	// ����
	if(rxCmd[2]) { Motor_Cur_Pos = -Motor_Cur_Pos; }
	}
	return Motor_Cur_Pos;
}


float Read_Motor_Cur_Vel(void)
{
	/**********************************************************
	***	�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	**********************************************************/	
//	while(rxFrameFlag == false){}; rxFrameFlag = false;
	/**********************************************************
	***	У���ַ�������롢�������ݳ��ȣ�У��ɹ�����㵱ǰλ�ýǶ�
	**********************************************************/	
	if(rxCmd[0] == 1 && rxCmd[1] == 0x35 && rxCount == 6)
	{
	// ƴ�ӳ�uint32_t����
	vel = (uint32_t)(
					  ((uint32_t)rxCmd[3] << 8)    |
					  ((uint32_t)rxCmd[4] << 0)    
					);

	// ת�����ٶ�
	Motor_Cur_Vel = (float)vel;

	// ����
	if(rxCmd[2]) { Motor_Cur_Vel = -Motor_Cur_Vel; }
	}
	return Motor_Cur_Vel;
}



/**
	* @brief   USART1�жϺ���
	* @param   ��
	* @retval  ��
	*/
void USART3_IRQHandler(void)
{
	__IO uint16_t i = 0;
	
	
/**********************************************************
***	���ڽ����ж�
**********************************************************/
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		// δ���һ֡���ݽ��գ����ݽ��뻺�����
		fifo_enQueue((uint8_t)USART3->DR);

		// ������ڽ����ж�
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}

/**********************************************************
***	���ڿ����ж�
**********************************************************/
	else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		// �ȶ�SR�ٶ�DR�����IDLE�ж�
		USART3->SR; USART3->DR;

		// ��ȡһ֡��������
		rxCount = fifo_queueLength(); for(i=0; i < rxCount; i++) { rxCmd[i] = fifo_deQueue(); }

		// һ֡���ݽ�����ɣ���λ֡��־λ
		rxFrameFlag = true;
	}
}

/**
	* @brief   USART���Ͷ���ֽ�
	* @param   ��
	* @retval  ��
	*/
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
	__IO uint8_t i = 0;
	
	for(i=0; i < len; i++) { usart_SendByte(cmd[i]); }
}

/**
	* @brief   USART����һ���ֽ�
	* @param   ��
	* @retval  ��
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
