#include "timer.h"
#include "usart3.h"
TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM2_Init(u16 arr,u16 psc)
{  
	__HAL_RCC_TIM2_CLK_ENABLE(); 
	
    TIM2_Handler.Instance=TIM2;                          //通用定时器3
    TIM2_Handler.Init.Prescaler=psc;                     //分频系数
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
		TIM2_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;//使能自动重载
    HAL_TIM_Base_Init(&TIM2_Handler);
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE 
	
	HAL_NVIC_SetPriority(TIM2_IRQn,0,1);    //设置中断优先级，抢占优先级0，子优先级1
	HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	
  
}

//定时器3中断服务函数
void TIM2_IRQHandler(void)
{
   
	if(__HAL_TIM_GET_FLAG(&TIM2_Handler,TIM_FLAG_UPDATE)!=RESET)    //更新中断
    {
        __HAL_TIM_CLEAR_IT(&TIM2_Handler,TIM_IT_UPDATE);            //清除中断
        USART3_RX_STA|=1<<15;	                                    //标记接收完成
        __HAL_TIM_DISABLE(&TIM2_Handler);                           //关闭定时器7 
    }
		
}

