#ifndef _ADC_H_
#define _ADC_H_
#include <stm32F1xx.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
void Adc_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(u32 ch); 		        //���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u32 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ

#endif
