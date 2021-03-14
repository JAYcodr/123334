#include "OLED_IIC.h"
#include "delay.h"
#include "stm32f1xx.h"
#include "stm32_hal_legacy.h"
//OLED_IIC初始化
void OLED_IIC_Pin_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    
    //PC8,PC9初始化设置
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    IIC_SDA=1;
    IIC_SCL=1;  
}


//产生IIC起始信号
void OLED_IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void OLED_IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}

/*******************************************************************************
*函数的原型：void OLED_IIC_Send_Ack(u8 ack)
*函数的功能：发送应答信号
*函数的参数：
	@ u8 ack：应答信号 0：非应答 1：应答
*函数返回值：None
*函数的说明：
*函数编写者：庆
*函数编写日期：2021/2/22
*函数的版本号：V1.0
********************************************************************************/  
void OLED_IIC_Send_Ack(u8 ack)
{
	IIC_SCL = 1;
	delay_us(5);
	IIC_SCL = 0;
	delay_us(5);
	
	if(ack)
	{
		IIC_SDA = 1;
	}
	else
	{
		IIC_SDA = 0;
	}
	
	delay_us(5);
	//保持周期完整性
	IIC_SCL = 1;
	delay_us(5);
	IIC_SCL = 0;
}

/*******************************************************************************
*函数的原型：u8 OLED_IIC_Reception_Ack(void)
*函数的功能：接收应答信号
*函数的参数：None
*函数返回值：
	0：非应答
	1：应答
*函数的说明：高电平数据采集 低电平数据发送
*函数编写者：庆
*函数编写日期：2021/2/22
*函数的版本号：V1.0
********************************************************************************/
u8 OLED_IIC_Reception_Ack(void)
{
	IIC_SDA = 1;
	delay_us(5);
	IIC_SCL = 0;
	delay_us(5);
	IIC_SCL = 1;
	
	if(IIC_SDA_IN)
	{
		return 1;
	}
	return 0;
}

/*******************************************************************************
*函数的原型：u8 OLED_IIC_Send_Data(u8 data)
*函数的功能：发送数据
*函数的参数：
	@ u8 data：一字节数据
*函数返回值：应答信号
*函数的说明：8位数据发送 每个字节后面都要跟一个响应
*函数编写者：庆
*函数编写日期：2021/2/22
*函数的版本号：V1.0
********************************************************************************/
u8 OLED_IIC_Send_Data(u8 data)
{
	u8 i = 0;
	
	for(i = 0; i < 8; i++)
	{
		IIC_SCL = 0; 
		if(data & 0x80)
		{
			IIC_SDA = 1;
		}
		else
		{
			IIC_SDA = 0;
		}
		data <<= 1;
		delay_us(5);
	//保持周期完整性
		IIC_SCL = 1;
		delay_us(5);
	}
	
	IIC_SCL = 0;
	return OLED_IIC_Reception_Ack();
	//接收应答
}

/*******************************************************************************
*函数的原型：u8 OLED_IIC_Read_Data(void)
*函数的功能：读一字节数据
*函数的参数：None
*函数返回值：
	一字节数据
*函数的说明：
*函数编写者：庆
*函数编写日期：2021/2/22
*函数的版本号：V1.0
********************************************************************************/
u8 OLED_IIC_Read_Data(void)
{
	u8 data = 0;
	u8 i = 0;
	
	IIC_SDA = 1;
	
	for(i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		delay_us(5);
		IIC_SCL = 1;
		
		if(IIC_SDA_IN)
		{
			data |= 1;
		}
		
		data <<= 1;
	}
	
	OLED_IIC_Send_Ack(1);
	
	return data;
}
