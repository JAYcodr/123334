
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "adc.h"

#include "los_swtmr.h"
#include "los_queue.h"
#include "los_task.h"

/**********************************README****************************************************
1, this demo will use 1 task,1 queue,2 softtimer,make sure these are configured correctly
2, the soft timer will run periodic to write message to the queue
3, the task_rcv will read the message from the queue,check the message,if right,it will print
   that is all!
********************************************************************************************/

/**************************************MACRO DEFINES*****************************************/
#define QUEUE_LENGTH             10
#define SND_CMD1_MSG             500
#define SND_CMD2_MSG             250
#define SND_CMD3_MSG             300
#define SND_CMD4_MSG             1000
#define RECV_TASK_PRIOR          8          //receive task priority
#define RECV_TASK_STACK_SIZE     0x600      //receive task stack size,if you do more deal,make it bigger

#define LED0_GPIO_Port GPIOC
#define LED1_GPIO_Port GPIOC
#define LED2_GPIO_Port GPIOC
#define LED3_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_0
#define LED1_Pin GPIO_PIN_1
#define LED2_Pin GPIO_PIN_2
#define LED3_Pin GPIO_PIN_3

#define KEY0        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)  //KEY0按键PC8
#define KEY1        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)  //KEY1按键PC9
#define KEY2        HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)  //KEY1按键PD2
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0



/**************************************GLOBAL VARS********************************************/
UINT32 g_QueueID;
UINT32 g_RecvTaskID;
UINT16 g_TimerID1;
UINT16 g_TimerID2;
UINT16 g_TimerID3;
UINT16 g_TimerID4;
/***************************************************************************************************
function     :this is the timer callback entry
parameters   :args,not used yet
instruction  :we will send message to the queue periodicly
***************************************************************************************************/
static VOID Timer_Callback(UINT32 ulArg)
{  
    UINT32 ulRet = LOS_OK;
    
    ulRet = LOS_QueueWriteCopy(
                g_QueueID,
                (VOID *)&ulArg,         /* send to msg */
                sizeof(UINT32),
                0
                ); 
    if(ulRet != LOS_OK)
    {
        Debug_Printf("send message failure,error:%x\r\n",ulRet);
				LOS_TaskDelay(5000);
    }
    return;
}


static UINT32 Timers_Init(UINT32 ulInterval, UINT16 ulTimerID, UINT32 ulSendMsg)
{
    UINT32 ulRet = LOS_OK;

    ulRet = LOS_SwtmrCreate(
              ulInterval,             /* interval for ulInterval tick */
              LOS_SWTMR_MODE_PERIOD,  /* peridic software timer */ 
              Timer_Callback,         /* software timer callback function */
              &ulTimerID,             /* software timer id */
              ulSendMsg               /* sendmsg */
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == YES)
              , OS_SWTMR_ROUSES_ALLOW, OS_SWTMR_ALIGN_SENSITIVE
#endif
            );
    if (LOS_OK == ulRet)
    {
        ulRet = LOS_SwtmrStart(ulTimerID);
        if (LOS_OK != ulRet)
        {
            Debug_Printf("start software timer error:%x\r\n",ulRet);
            return ulRet;
        }
    }

    return ulRet;
}

static VOID Task_RecvQ(UINT32 ulArg)
{
    UINT32 ulRecvMsg = 0;
    UINT32 ulRet = LOS_OK;
    UINT32 ulLength = 0;
    
    while (1) {
        ulLength = sizeof(ulRecvMsg);
        ulRet = LOS_QueueReadCopy(g_QueueID, &ulRecvMsg, &ulLength, LOS_WAIT_FOREVER);
        if (LOS_OK == ulRet)
        {
            switch(ulRecvMsg)
            {
                case SND_CMD1_MSG:
								{
									extern int STATUSKEY0;
									if(STATUSKEY0==1)
									{
										HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);	
									}		
								}
                    break;
                case SND_CMD2_MSG:
                    
								{
									extern int STATUSKEY1;
									if(STATUSKEY1==1)
									{
										HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
										
									}
								}
                    break;
								 case SND_CMD3_MSG:
								 {
											uint16_t cnt;
											float temp=0;
									 
									   HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
									   uint16_t adcx=Get_Adc_Average(ADC_CHANNEL_9,3);//ADC原始值 
										 temp=(float)adcx*(3.3/4096);//ADC电压值
										 cnt=temp;
									   Debug_Printf("\r\n ********************************** \r\n");
									   Debug_Printf("\r\n          ADC电压检测模块        \r\n");
										 Debug_Printf("\r\n 获取到采样原始值=%d，电压值=%d.2V \r\n",adcx,cnt);
									 Debug_Printf("\r\n ********************************** \r\n");
									 
								 }
                    break;
								 	case SND_CMD4_MSG:
								 {
									  Debug_Printf("\r\n ********************************** \r\n");
										Debug_Printf("\r\n       24C02存取模块       \r\n");
										Debug_Printf("\r\n 7F XX-------------保存xx内容 \r\n");
										Debug_Printf("\r\n 5F   -------------读取保存的内容 \r\n");
									  Debug_Printf("\r\n ********************************** \r\n");
								 }
                    break;
								 
                default:
                    break;
            }
          
						if(ulRecvMsg==500|ulRecvMsg==250)
						{
							Debug_Printf("RCVMSG:   OK:%d\r\n", ulRecvMsg); //valid message here
						}
            
            ulRecvMsg = 0; //clear the received message here.
        }
    }
}

UINT32 Blinky_Demo(VOID) 
{
    UINT32 ulRet = LOS_OK;
    TSK_INIT_PARAM_S stTask = {0};
	

    /* Create the queue. */
    ulRet = LOS_QueueCreate(
              "MsgQueue",       /* queue name */
              QUEUE_LENGTH,     /* queue length */
              &g_QueueID,       /* queue id */
              0,                /* flags, not used */
              sizeof(UINT32)    /* message size */
              );
    if (ulRet != LOS_OK)
    {
      Debug_Printf("create the queue error:%x\r\n", ulRet);
      return ulRet;
    }
    
    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)Task_RecvQ;
    stTask.uwStackSize = RECV_TASK_STACK_SIZE;
    stTask.pcName = "RecvTask";
    stTask.usTaskPrio = RECV_TASK_PRIOR;

    /* Create the task. */
    ulRet = LOS_TaskCreate(
              &g_RecvTaskID,    /* receive task id */
              &stTask           /* task params */
              );
    if (ulRet != LOS_OK)
    {
        Debug_Printf("create task_rcv error:%x\r\n",ulRet);
        return ulRet;
    }

    /* Create the software timer1. */
    ulRet = Timers_Init(500, g_TimerID1, SND_CMD1_MSG);
    if (ulRet != LOS_OK)
    {
        Debug_Printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }

    ulRet = Timers_Init(250, g_TimerID2, SND_CMD2_MSG);
    if (ulRet != LOS_OK)
    {
        Debug_Printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }

    ulRet = Timers_Init(5000, g_TimerID3, SND_CMD3_MSG);
    if (ulRet != LOS_OK)
    {
        Debug_Printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }		

		    ulRet = Timers_Init(9600, g_TimerID4, SND_CMD4_MSG);
    if (ulRet != LOS_OK)
    {
        Debug_Printf("create timer error:%x\r\n", ulRet);
        return ulRet;
    }	
    return LOS_OK;
}



