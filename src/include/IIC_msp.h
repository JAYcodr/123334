#ifndef _IIC_MSP_H
#define _IIC_MSP_H

#include "stm32F1xx.h"

//////////////////////////////////////////////////////////////////////////////////	 
//V1.0����˵�� 2020-4-28
//����IIC��д����
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

void IIC_Init(void);        							//��ʼ��IIC��IO��				 
void IIC_Start(void);											//����IIC��ʼ�ź�
void IIC_Stop(void);	  									//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);					//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack); //IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 							//IIC�ȴ�ACK�ź�
void IIC_Ack(void);												//IIC����ACK�ź�
void IIC_NAck(void);											//IIC������ACK�ź�

//void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
//uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

#endif
