#include "timer.h"
#include "usart3.h"
TIM_HandleTypeDef TIM2_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM2_Init(u16 arr,u16 psc)
{  
	__HAL_RCC_TIM2_CLK_ENABLE(); 
	
    TIM2_Handler.Instance=TIM2;                          //ͨ�ö�ʱ��3
    TIM2_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM2_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
		TIM2_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;//ʹ���Զ�����
    HAL_TIM_Base_Init(&TIM2_Handler);
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE 
	
	HAL_NVIC_SetPriority(TIM2_IRQn,0,1);    //�����ж����ȼ�����ռ���ȼ�0�������ȼ�1
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //����ITM3�ж�   
	
  
}

//��ʱ��3�жϷ�����
void TIM2_IRQHandler(void)
{
   
	if(__HAL_TIM_GET_FLAG(&TIM2_Handler,TIM_FLAG_UPDATE)!=RESET)    //�����ж�
    {
        __HAL_TIM_CLEAR_IT(&TIM2_Handler,TIM_IT_UPDATE);            //����ж�
        USART3_RX_STA|=1<<15;	                                    //��ǽ������
        __HAL_TIM_DISABLE(&TIM2_Handler);                           //�رն�ʱ��7 
    }
		
}

