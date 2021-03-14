#include "sys.h"
#include "USART3.h"	  
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "timer.h" 
#include "Uart_msp.h"
#include "los_task.h" 
UART_HandleTypeDef UART3_Handler; //UART3���

//���ڷ��ͻ����� 	
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA=0;   	 
void USART3_IRQHandler(void)
{
	u8 res;
	if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        HAL_UART_Receive(&UART3_Handler,&res,1,1000); 
		if((USART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//�����Խ�������
			{
                __HAL_TIM_SET_COUNTER(&TIM2_Handler,0);	//���������			
				if(USART3_RX_STA==0) 				//ʹ�ܶ�ʱ��3���ж� 
				{
                    __HAL_TIM_ENABLE(&TIM2_Handler); //ʹ�ܶ�ʱ��3
				}
				USART3_RX_BUF[USART3_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				USART3_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
    }         													 
}   
//��ʼ��IO ����2
//bound:������ 
void USART3_Init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_Initure;
	       
    //UART3 ��ʼ������
    __HAL_RCC_GPIOB_CLK_ENABLE();			//ʹ��GPIOBʱ��
    __HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART3ʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_10;			//PA2
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PA2

	GPIO_Initure.Pin=GPIO_PIN_11;			//PA3
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //��������
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	//��ʼ��PA3

	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()��ʹ��UART3  
	__HAL_UART_ENABLE_IT(&UART3_Handler,UART_IT_RXNE);  //���������ж�
	HAL_NVIC_EnableIRQ(USART3_IRQn);				    //ʹ��USART3�ж�ͨ��
	HAL_NVIC_SetPriority(USART3_IRQn,0,2);			    //��2�����ȼ�0,2,������ȼ� 

	TIM2_Init(99,7199);	                //10ms�ж�һ��
	__HAL_TIM_DISABLE(&TIM2_Handler);   //�رն�ʱ��3 
	USART3_RX_STA=0;				    //���� 
}

//����2,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
		USART3->DR=USART3_TX_BUF[j];  
	}
}
