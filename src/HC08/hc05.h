#ifndef __HC05_H
#define __HC05_H	 
#include "sys.h" 
#include "Uart_msp.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//ATK-HC05����ģ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/5/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved									  								  
//////////////////////////////////////////////////////////////////////////////////    

#define HC05_KEY  	PAout(1) 	//��������KEY�ź�
#define HC05_LED  	PAin(4)		//��������״̬�ź�

void HC05_Sta_Show(void);
void HC05_Role_Show(void);
u8 HC05_Init(void);
void HC05_CFG_CMD(u8 *str);
u8 HC05_Get_Role(void);
u8 HC05_Set_Cmd(u8* atstr);	   
#endif  

