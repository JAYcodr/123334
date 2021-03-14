#ifndef __USART3_H
#define __USART3_H 
#include "sys.h"
#include "stdio.h"	  
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32������
//����2��ʼ������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/6/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved  
////////////////////////////////////////////////////////////////////////////////// 	

#define USART3_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern vu16 USART3_RX_STA;   						//��������״̬

void USART3_Init(u32 bound);
void u2_printf(char* fmt,...);
#endif	   















