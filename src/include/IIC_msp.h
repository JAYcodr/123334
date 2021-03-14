#ifndef _IIC_MSP_H
#define _IIC_MSP_H

#include "stm32F1xx.h"

//////////////////////////////////////////////////////////////////////////////////	 
//V1.0创建说明 2020-4-28
//抽象IIC读写操作
////////////////////////////////////////////////////////////////////////////////// 

#define ANO_GPIO_I2C    GPIOB
#define I2C_Pin_SCL     GPIO_PIN_6
#define I2C_Pin_SDA     GPIO_PIN_7
#define ANO_RCC_I2C     RCC_APB2Periph_GPIOB  

#define SCL_H         ANO_GPIO_I2C->BSRR = I2C_Pin_SCL
#define SCL_L         ANO_GPIO_I2C->BRR  = I2C_Pin_SCL
#define SDA_H         ANO_GPIO_I2C->BSRR = I2C_Pin_SDA
#define SDA_L         ANO_GPIO_I2C->BRR  = I2C_Pin_SDA
#define SCL_read      ANO_GPIO_I2C->IDR  & I2C_Pin_SCL
#define SDA_read      ANO_GPIO_I2C->IDR  & I2C_Pin_SDA

void IIC_Init(void);        							//初始化IIC的IO口				 
void IIC_Start(void);											//发送IIC开始信号
void IIC_Stop(void);	  									//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);					//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack); //IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 							//IIC等待ACK信号
void IIC_Ack(void);												//IIC发送ACK信号
void IIC_NAck(void);											//IIC不发送ACK信号

//void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
//uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

#endif
