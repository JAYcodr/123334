#include "Uart_msp.h"
#include "Queue.h"

//串口是否初始化
_Bool bCom1 = 0;
_Bool bCom2 = 0;
_Bool bCom3 = 0;
_Bool bCom4 = 0;
_Bool bCom5 = 0;

//串口1相关变量
static QUEUE8_t m_QueueCom1Rx         = {0}; //
static uint8_t  m_Com1RxBuf[COM1_RX_BUF_SIZE]      = {0};
UART_HandleTypeDef UART1_Handler; //UART句柄

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&UART1_Handler,&ch, 1, 0xffff);
  return ch;
}




void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
}

//串口初始化
void Uart_Init(u8 comx,u32 bound,u32 wordLen,u32 stopBit,u32 parity,u32 hwControl)
{
	switch(comx)
	{
		case 1://串口1
			{
				QUEUE_PacketCreate(&m_QueueCom1Rx, m_Com1RxBuf, sizeof(m_Com1RxBuf));//创建接收数据缓冲区LOOP
				
				UART1_Handler.Instance=USART1;					    //UART1
				UART1_Handler.Init.BaudRate=115200;//bound;				    //波特率
				UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;//wordLen;   //字长为8位数据格式
				UART1_Handler.Init.StopBits=UART_STOPBITS_1;//stopBit;	    //一个停止位
				UART1_Handler.Init.Parity=UART_PARITY_NONE;//parity;		    //无奇偶校验位
				UART1_Handler.Init.HwFlowCtl= UART_HWCONTROL_NONE;//hwControl;   //无硬件流控
				UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
				UART1_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
				HAL_UART_Init(&UART1_Handler); //HAL_UART_Init()会使能UART1
				SET_BIT(UART1_Handler.Instance->CR1, USART_CR1_RXNEIE);//使能接收中断
				bCom1 = 1;
			}
			break;
		default:
			break;
	}
}

//串口写操作,>0-写出去实际字节数，0-未初始化
u32 Uart_Write(u8 comx,u8 *writeBuf,u32 uLen)
{
	u32 uRtn = 0;

	switch(comx)
	{
		case 1://串口1
			{
				if(bCom1 == 0)
					uRtn = 0;
				else
					uRtn = uLen;
				if(uRtn > 0)
				{
					//实现写函数
					HAL_UART_Transmit(&UART1_Handler,writeBuf,uLen,100);
					while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
				}
			}
			break;
		default:
			break;
	}
	return uRtn;
}

//串口1中断服务程序
void USART1_IRQHandler  (void)              	
{ 
	uint8_t buffer[1];
	
	if(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET) 
	{  
		buffer[0] = USART1->DR; 
		QUEUE_PacketIn(&m_QueueCom1Rx, buffer, 1);//LOOP
  } 
	
	if (__HAL_UART_GET_FLAG(&UART1_Handler, UART_FLAG_ORE) != RESET)
  {
		buffer[0] = USART1->DR;
		__HAL_UART_CLEAR_FLAG(&UART1_Handler, UART_FLAG_ORE);
  }
	__HAL_UART_CLEAR_PEFLAG(&UART1_Handler);
} 

//串口读操作
u32 Read_Comm(u8 comx,uint8_t *pBuf,uint32_t uiLen)
{
	int uRtn=0;
	switch(comx)
	{
		case 1://串口1
			{
				uRtn = QUEUE_PacketOut(&m_QueueCom1Rx, pBuf, uiLen);
			}
			break;
		default:
			break;
	}
	return uRtn;
}
