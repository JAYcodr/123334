#ifndef _UART_MSP_H
#define _UART_MSP_H
#include "stm32F1xx.h"

//////////////////////////////////////////////////////////////////////////////////	 
//V1.0����˵�� 2019-3-12
//�������д��ڶ�д����
//д����-ֱ��д����
//������-�Ӷ�����ֱ�Ӷ������д�С1K
//��ʼ������--��ʹ����ش��ڳ�ʼ������,���г�ʼ��
////////////////////////////////////////////////////////////////////////////////// 

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//���ںŶ���
#define COM1 1
#define COM2 2
#define COM3 3
#define COM4 4
#define COM5 5

#define COM1_RX_BUF_SIZE         512
#define COM2_RX_BUF_SIZE         550
#define COM3_RX_BUF_SIZE         256
#define COM4_RX_BUF_SIZE         256
#define COM5_RX_BUF_SIZE         1024

//���ڳ�ʼ��
void Uart_Init(u8 comx,u32 bound,u32 wordLen,u32 stopBit,u32 parity,u32 hwControl);
//����д����,>0-д��ȥʵ���ֽ�����0-δ��ʼ��
u32 Uart_Write(u8 comx,u8 *writeBuf,u32 uLen);
//���ڶ�����
u32 Read_Comm(u8 comx,unsigned char *pBuf,u32 uiLen);

#endif
