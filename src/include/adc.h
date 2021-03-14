#ifndef _ADC_H_
#define _ADC_H_
#include <stm32F1xx.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u32 ch); 		        //获得某个通道值 
u16 Get_Adc_Average(u32 ch,u8 times);//得到某个通道给定次数采样的平均值

#endif
