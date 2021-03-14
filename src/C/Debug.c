/******************** (C) COPYRIGHT 2016 KMUST ********************************
* File Name          : debug.c
* Author             : ����ƽ
* Version            : V1.0.1
* Date               : 2/23/2016
* �޸�               : 5/23/2019 �޸�
* Description        : ����ģ��(����COM1��Ϊ��������ںͱ��ص��Կ�)
********************************************************************************/
#include "debug.h"
#include "Uart_msp.h"
#include <stdarg.h>  
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "los_mux.h" 
#include "los_task.h" 

#include "los_base.h"
#include "los_sys.h"
#include "los_typedef.h"
#include "los_task.ph"


#define MAXDEBUGSEND 512  //���ݰ���󳤶�

volatile uint8_t b_DebugInit = 0;
/*���������ID*/ 
UINT32  g_Testmux01;

//���ڸ�ʽ�����
void Debug_Printf(const  void *format, ...)
{
	if(b_DebugInit == 0)
	{
		/*����������*/ 
    LOS_MuxCreate(&g_Testmux01);
		b_DebugInit = 1;
		Uart_Init(COM1,115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE,UART_HWCONTROL_NONE);
	}
	
	/*���뻥����*/ 
	UINT32 uwRet; 
  uwRet=LOS_MuxPend(g_Testmux01, 10); 
	if(uwRet == LOS_OK) 
	{ 
		static  uint8_t  buffer[MAXDEBUGSEND + 1];
		uint32_t uLen;
		va_list     vArgs;
		va_start(vArgs, format);
		uLen = vsprintf((char *)buffer, (char const *)format, vArgs);
		va_end(vArgs);
		if(uLen > MAXDEBUGSEND)
			uLen = MAXDEBUGSEND;
		Uart_Write(COM1,buffer,uLen);
		
		LOS_MuxPost(g_Testmux01); 
	}
}

