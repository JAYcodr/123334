/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Uart_msp.h"
#include "adc.h"
#include "tim.h"
#include "stdio.h"
#include <24cxx.h>
#include <IIC_msp.h>

//LiteOS include
#include "los_base.h"
#include "los_sys.h"
#include "los_typedef.h"
#include "los_task.ph"

#include "los_mux.h" 
#include "los_task.h" 

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define KEY0        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)  //KEY0按键PC8
#define KEY1        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)  //KEY1按键PC9
#define KEY2        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)  //KEY1按键PD2
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0

#define LED0_GPIO_Port GPIOC
#define LED1_GPIO_Port GPIOC
#define LED2_GPIO_Port GPIOC
#define LED3_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_0
#define LED1_Pin GPIO_PIN_1
#define LED2_Pin GPIO_PIN_2
#define LED3_Pin GPIO_PIN_3



/* USER CODE END PM */

	int STATUSKEY0=0;
	int STATUSKEY1=0;
	int STATUSKEY2=0;
	int KEY1_Flag=0;
	int KEY2_Flag=0;
	int KEY3_Flag=0;
	
//	/* -----------------------创建接收字符识别----------------------------------*/
//	char UARTR_RECEIVE_Flag[] = "7F\r";				//定义相关的字符串     \n 被吃掉了
//	char UARTR_SEND_Flag[] = "5F\r";
//	char ADDR_Flag[] = "10\r";
//	

/* Private variables ---------------------------------------------------------*/
UINT32 g_TskHandle;

UINT32 Blinky_Demo(VOID);

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void task1(void)
{
	
	while (1)
  {
		if(KEY0==0)
		{

			LOS_TaskDelay(5);
			if(KEY0==0)
			{
				STATUSKEY0=!STATUSKEY0;
				KEY1_Flag=1;
			}		
		}
				if(KEY1_Flag==1)
			{
				Debug_Printf("\r\n ********************************** \r\n");
				Debug_Printf("\r\n             KEY0已按下             \r\n");
				Debug_Printf("\r\n ********************************** \r\n");
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0 ,GPIO_PIN_SET );
			 	KEY1_Flag=0;
			}
			LOS_TaskDelay(500);		
  }
	
}
UINT32 creat_task1()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "task1";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

void task2(void)
{
	while (1)
  {
		LOS_TaskDelay(1000);
		
			 if(KEY1==0)
		{
			LOS_TaskDelay(5);
			if(KEY1==0)
			{
				STATUSKEY1=!STATUSKEY1;
				KEY2_Flag=1;
			}		
		}
					if(KEY2_Flag==1)
			{
				Debug_Printf("\r\n ********************************** \r\n");
				Debug_Printf("\r\n             KEY1已按下             \r\n");
				Debug_Printf("\r\n ********************************** \r\n");
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET );
			 	KEY2_Flag=0;
			}
  }
}
UINT32 creat_task2()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "task2";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

void task3(void)
{
	
	while (1)
  {
				if(KEY2==0)
		{

			LOS_TaskDelay(50);
			if(KEY2==0)
			{
				STATUSKEY2=!STATUSKEY2;
				KEY3_Flag=1;
			}	

		}
			if(KEY3_Flag==1)
			{
				Debug_Printf("\r\n ********************************** \r\n");
				Debug_Printf("\r\n             KEY2已按下             \r\n");
				Debug_Printf("\r\n ********************************** \r\n");
			 	KEY3_Flag=0;
			}
		LOS_TaskDelay(500);
  }
}


UINT32 creat_task3()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 2;
    task_init_param.pcName = "task3";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task3;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

void task4(void)
{
	while (1)
  {
		u8 i=0;
	static	u8 Buf[200];
		u32 len=0;
		char ch,ch1;
		int j;
		
		len = Read_Comm(COM1,Buf,200);
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);	
		LOS_TaskDelay(200);
		
/********************************************************/

/********************************************************/
		if(len>0)
		{
			ch=Buf[0];
			ch1=Buf[1];
			Debug_Printf("\r\n ********************************** \r\n");
			Debug_Printf("\r\n 输入完毕:%c%c \r\n",ch,ch1);
			switch(ch)
			{
				case '7':
					if(ch1=='F')
					{
						Debug_Printf("\r\n 检测到保存命令 \r\n");
						Debug_Printf("\r\n 数据将存放到0X10地址 \r\n");
						for(j=0;j<200;j++)
						{
							Buf[j]=Buf[j+3];
						}
						Uart_Write(COM1,Buf,len);
						Debug_Printf("\r\n\r\n");
						AT24CXX_Write(0x10,Buf,len);
						Debug_Printf("\r\n ********************************** \r\n");
					}
					LOS_TaskDelay(500);
					break;
				case '5':
					if(ch1=='F')
					{
					Debug_Printf("\r\n ********************************** \r\n");
					Debug_Printf("\r\n 检测到读取命令 \r\n");
					Debug_Printf("\r\n 从24C02读取到的信息为： \r\n");
					for(i=0;i<len;i++)
					Buf[i]=0x00;
					AT24CXX_Read(0x10,Buf,len);
					Uart_Write(COM1,Buf,len);
					Debug_Printf("\r\n \r\n");
					Debug_Printf("\r\n ********************************** \r\n");
					}
					break;
			}
				
		}

	}
}
UINT32 creat_task4()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "task4";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task4;
    task_init_param.uwStackSize = 0x200;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	Adc_Init();
  SystemClock_Config();
	AT24CXX_Init();
	uint8_t AT24CXX_Check(void);
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	Uart_Init(COM1,115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE,UART_HWCONTROL_NONE);//DEBUG 串口
	
	
	UINT32 uwRet = 0;
  uwRet = LOS_KernelInit();
	if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }
	Debug_Printf("欢迎进入-LiteOS实验平台！\r\n");
	Debug_Printf("LOS_KernelInit！\r\n");
	
	uwRet = creat_task1();
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }
	Debug_Printf("creat_task1()！\r\n");
	
  uwRet = creat_task2();
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }
	Debug_Printf("creat_task2()！\r\n");
	
	  uwRet = creat_task3();
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }
	Debug_Printf("creat_task3()！\r\n");
	
	  uwRet = creat_task4();
  if (uwRet != LOS_OK)
  {
      return LOS_NOK;
  }
	Debug_Printf("creat_task4()！\r\n");

	Debug_Printf("\r\n ********************************** \r\n");
	Debug_Printf("\r\n           基于LiteOS操作系统       \r\n");
	Debug_Printf("\r\n             嵌入式程序设计         \r\n");
	Debug_Printf("\r\n              昆明理工大学          \r\n");
	Debug_Printf("\r\n           指导老师：王剑平       \r\n");
	Debug_Printf("\r\n             测控171 文杰        \r\n");
	Debug_Printf("\r\n             201610402117       \r\n");
	Debug_Printf("\r\n ********************************** \r\n");

	
	
	Blinky_Demo();
	(void)LOS_Start();
	
  /*while (1)
  {

  }*/
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PC2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
		/*Configure GPIO pin : PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
