#include "delay.h" 	
#include "Uart_msp.h"	 
//#include "usart.h"
#include"debug.h"
#include "Queue.h"	 			 
#include "USART3.h" 			 
#include "hc05.h" 
#include "led.h" 
#include "string.h"	 
#include "math.h"
#include "los_task.h" 
//��ʾATK-HC05ģ�������״̬
void HC05_Role_Show(void)
{
	if(HC05_Get_Role()==1) 
	{
		Debug_Printf("ROLE:Master\r\n");//����
	}
		
	else Debug_Printf("ROLE:Slave\r\n");//�ӻ�
}
//��ʾATK-HC05ģ�������״̬
void HC05_Sta_Show(void)
{	
    static u8 HC05_STA=2;
	if(HC05_LED)
	{
		if(HC05_STA!=1)
		{
			HC05_STA=1;
			Debug_Printf("STA:Connected\r\n");//���ӳɹ�
		}
	}		
	else 
	{
		if(HC05_STA!=0)
		{
			HC05_STA=0;
			Debug_Printf("STA:Disconnect\r\n");//δ����	
		}
	}
}	

 
 
//��ʼ��ATK-HC05ģ��
//����ֵ:0,�ɹ�;1,ʧ��.
u8 HC05_Init(void)
{
	u8 retry=10,t;	  		 
	u8 temp=1;
	
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
	
   
    GPIO_Initure.Pin=GPIO_PIN_4;            //PA4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_1;            //PA1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);	//PA1��1 
	
		USART3_Init(9600);	//��ʼ������2Ϊ:9600,������.
		Debug_Printf("TEST TEST TEST\r\n");
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		LOS_TaskDelay(20);
		u2_printf("AT\r\n");		//����AT����ָ��
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		for(t=0;t<10;t++) 			//��ȴ�50ms,������HC05ģ��Ļ�Ӧ
		{
			if(USART3_RX_STA&0X8000)break;
			LOS_TaskDelay(5);
		}		
		if(USART3_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
			USART3_RX_STA=0;			 
			if(temp==4&&USART3_RX_BUF[0]=='O'&&USART3_RX_BUF[1]=='K')
			{
				temp=0;//���յ�OK��Ӧ
				break;
			}
		}			    		
	}		    
	if(retry==0)temp=1;	//���ʧ��
	return temp;	 
}

//��ȡATK-HC05ģ��Ľ�ɫ
//����ֵ:0,�ӻ�;1,����;0XFF,��ȡʧ��.							  
u8 HC05_Get_Role(void)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		LOS_TaskDelay(10);
		u2_printf("AT+ROLE?\r\n");	//��ѯ��ɫ
		for(t=0;t<20;t++) 			//��ȴ�200ms,������HC05ģ��Ļ�Ӧ
		{
			LOS_TaskDelay(10);
			if(USART3_RX_STA&0X8000)break;
		}		
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		if(USART3_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
			USART3_RX_STA=0;			 
			if(temp==13&&USART3_RX_BUF[0]=='+')//���յ���ȷ��Ӧ����
			{
				temp=USART3_RX_BUF[6]-'0';//�õ�����ģʽֵ
				break;
			}
		}		
	}
	if(retry==0)temp=0XFF;//��ѯʧ��.
	return temp;
} 							   
//ATK-HC05��������
//�˺�����������ATK-HC05,�����ڽ�����OKӦ���ATָ��
//atstr:ATָ�.����:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"���ַ���
//����ֵ:0,���óɹ�;����,����ʧ��.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	u8 retry=0X0F;
	u8 temp,t;
	while(retry--)
	{
		HC05_KEY=1;					//KEY�ø�,����ATģʽ
		LOS_TaskDelay(20);
		u2_printf("%s\r\n",atstr);	//����AT�ַ���
		HC05_KEY=0;					//KEY����,�˳�ATģʽ
		for(t=0;t<20;t++) 			//��ȴ�100ms,������HC05ģ��Ļ�Ӧ
		{
			if(USART3_RX_STA&0X8000)break;
			LOS_TaskDelay(5);
		}		
		if(USART3_RX_STA&0X8000)	//���յ�һ��������
		{
			temp=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
			USART3_RX_STA=0;			 
			if(temp==4&&USART3_RX_BUF[0]=='O')//���յ���ȷ��Ӧ����
			{			
				temp=0;
				break;			 
			}
		}		
	}
	if(retry==0)temp=0XFF;//����ʧ��.
	return temp;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////
//ͨ���ú���,��������USMART,���Խ��ڴ���2�ϵ�ATK-HC05ģ��
//str:���.(����ע�ⲻ����Ҫ������س���)
void HC05_CFG_CMD(u8 *str)
{					  
	u8 temp;
	u8 t;		  
	HC05_KEY=1;						//KEY�ø�,����ATģʽ
	LOS_TaskDelay(10);
	u2_printf("%s\r\n",(char*)str); //����ָ��
	for(t=0;t<50;t++) 				//��ȴ�500ms,������HC05ģ��Ļ�Ӧ
	{
		if(USART3_RX_STA&0X8000)break;
		LOS_TaskDelay(10);
	}									    
	HC05_KEY=0;						//KEY����,�˳�ATģʽ
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{
		temp=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		USART3_RX_STA=0;
		USART3_RX_BUF[temp]=0;		//�ӽ�����		 
		Debug_Printf("\r\n%s",USART3_RX_BUF);//���ͻ�Ӧ���ݵ�����1
	} 				 
}



