#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"
#include "stm32f1xx.h"
//IO方向设置
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作
#define IIC_SCL    	PCout(8) //SCL
#define IIC_SDA    	PCout(9) //SDA
#define IIC_SDA_IN 	PCin(9)	 //SDA_IN
#define READ_SDA   	PBin(7)  //输入SDA 

//IIC所有操作函数
void OLED_IIC_Pin_Init(void);
void OLED_IIC_Start(void);				//发送IIC开始信号
void OLED_IIC_Stop(void);	  			//发送IIC停止信号
void OLED_IIC_Send_Ack(u8 ack);
u8 OLED_IIC_Reception_Ack(void);
u8 OLED_IIC_Send_Data(u8 data);
u8 OLED_IIC_Read_Data(void);

void OLED_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 OLED_IIC_Read_One_Byte(u8 daddr,u8 addr);	 
#endif
