//////////////////////////////////////////////////////////////////////////////////	 
//V1.0创建说明 2020-4-28
//抽象IIC读写操作
////////////////////////////////////////////////////////////////////////////////// 

#include "IIC_msp.h"

//IIC初始化
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    
	__HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
	//PB6,7初始化设置
	GPIO_Initure.Pin = I2C_Pin_SCL|I2C_Pin_SDA;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;     //高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
	SDA_H;//IIC_SDA=1;
	SCL_H;//IIC_SCL=1;  
}

void IIC_SDA_OUT()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.Pin = I2C_Pin_SDA;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ANO_GPIO_I2C,&GPIO_InitStructure);
}

void IIC_SDA_IN()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.Pin = I2C_Pin_SDA;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//GPIO_MODE_IPU; GPIO_MODE_INPUT
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(ANO_GPIO_I2C, &GPIO_InitStructure);	
}


void IIC_delay(void)
{
	uint8_t t = 2;
	while(t--);
	return;
}

//产生IIC起始信号
void IIC_Start(void)
{
	IIC_SDA_OUT();     //sda线输出
	SDA_H;//IIC_SDA=1;	  	  
	SCL_H;//IIC_SCL=1;
	IIC_delay();
 	SDA_L;//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_delay();
	SCL_L;//IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  

//产生IIC停止信号
void IIC_Stop(void)
{
	IIC_SDA_OUT();//SDA_OUT();//sda线输出
	SCL_L;//IIC_SCL=0;
	SDA_L;//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	IIC_delay();//delay_us(4);
	SCL_H;//IIC_SCL=1; 
	SDA_H;//IIC_SDA=1;//发送I2C总线结束信号
	IIC_delay();//delay_us(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	IIC_SDA_IN();//SDA_IN();      //SDA设置为输入  
	SDA_H;//IIC_SDA=1;
	IIC_delay();//delay_us(1);	   
	SCL_H;//IIC_SCL=1;
	IIC_delay();//delay_us(1);	 
	//while(READ_SDA)
	while(SDA_read)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_L;//IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

//产生ACK应答
void IIC_Ack(void)
{
	SCL_L;//IIC_SCL=0;
	IIC_SDA_OUT();//SDA_OUT();
	SDA_L;//IIC_SDA=0;
	IIC_delay();//delay_us(2);
	SCL_H;//IIC_SCL=1;
	IIC_delay();//delay_us(2);
	SCL_L;//IIC_SCL=0;
}

//不产生ACK应答		    
void IIC_NAck(void)
{
	SCL_L;//IIC_SCL=0;
	IIC_SDA_OUT();//SDA_OUT();
	SDA_H;//IIC_SDA=1;
	IIC_delay();//delay_us(2);
	SCL_H;//IIC_SCL=1;
	IIC_delay();//delay_us(2);
	SCL_L;//IIC_SCL=0;
}	

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
	uint8_t t,flag;   
	IIC_SDA_OUT();//SDA_OUT(); 	    
	SCL_L;//IIC_SCL=0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{    
		flag = (txd&0x80)>>7;
		//IIC_SDA=(txd&0x80)>>7;
		if(flag)
			SDA_H;
		else
			SDA_L;
		
		txd<<=1; 	  
		IIC_delay();//delay_us(2);   //对TEA5767这三个延时都是必须的
		SCL_H;//IIC_SCL=1;
		IIC_delay();//delay_us(2); 
		SCL_L;//IIC_SCL=0;	
		IIC_delay();//delay_us(2);
	}	 
} 

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		SCL_L;//IIC_SCL=0; 
    IIC_delay();//delay_us(2);
		SCL_H;//IIC_SCL=1;
		receive<<=1;
		//if(READ_SDA)
		if(SDA_read)
			receive++;   
		IIC_delay();//delay_us(1); 
	}					 
	if (!ack)
		IIC_NAck();//发送nACK
	else
		IIC_Ack(); //发送ACK   
	return receive;
}
