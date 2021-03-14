#ifndef _UART_MSP_H
#define _UART_MSP_H
#include "stm32F1xx.h"

//////////////////////////////////////////////////////////////////////////////////	 
//V1.0创建说明 2019-3-12
//抽象所有串口读写操作
//写操作-直接写串口
//读操作-从队列中直接读，队列大小1K
//初始化操作--在使能相关串口初始化参数,队列初始化
////////////////////////////////////////////////////////////////////////////////// 

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

//串口号定义
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

//串口初始化
void Uart_Init(u8 comx,u32 bound,u32 wordLen,u32 stopBit,u32 parity,u32 hwControl);
//串口写操作,>0-写出去实际字节数，0-未初始化
u32 Uart_Write(u8 comx,u8 *writeBuf,u32 uLen);
//串口读操作
u32 Read_Comm(u8 comx,unsigned char *pBuf,u32 uiLen);

#endif
