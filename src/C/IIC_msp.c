//////////////////////////////////////////////////////////////////////////////////	 
//V1.0����˵�� 2020-4-28
//����IIC��д����
////////////////////////////////////////////////////////////////////////////////// 

#include "IIC_msp.h"

//IIC��ʼ��
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
    
	__HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
	//PB6,7��ʼ������
	GPIO_Initure.Pin = I2C_Pin_SCL|I2C_Pin_SDA;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //�������
	GPIO_Initure.Pull = GPIO_PULLUP;          //����
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;     //����
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

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	IIC_SDA_OUT();     //sda�����
	SDA_H;//IIC_SDA=1;	  	  
	SCL_H;//IIC_SCL=1;
	IIC_delay();
 	SDA_L;//IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_delay();
	SCL_L;//IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SDA_OUT();//SDA_OUT();//sda�����
	SCL_L;//IIC_SCL=0;
	SDA_L;//IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	IIC_delay();//delay_us(4);
	SCL_H;//IIC_SCL=1; 
	SDA_H;//IIC_SDA=1;//����I2C���߽����ź�
	IIC_delay();//delay_us(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	IIC_SDA_IN();//SDA_IN();      //SDA����Ϊ����  
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
	SCL_L;//IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

//����ACKӦ��
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

//������ACKӦ��		    
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

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
	uint8_t t,flag;   
	IIC_SDA_OUT();//SDA_OUT(); 	    
	SCL_L;//IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{    
		flag = (txd&0x80)>>7;
		//IIC_SDA=(txd&0x80)>>7;
		if(flag)
			SDA_H;
		else
			SDA_L;
		
		txd<<=1; 	  
		IIC_delay();//delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		SCL_H;//IIC_SCL=1;
		IIC_delay();//delay_us(2); 
		SCL_L;//IIC_SCL=0;	
		IIC_delay();//delay_us(2);
	}	 
} 

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA_IN();//SDA_IN();//SDA����Ϊ����
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
		IIC_NAck();//����nACK
	else
		IIC_Ack(); //����ACK   
	return receive;
}
