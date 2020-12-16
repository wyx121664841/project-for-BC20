/***************STM32F103C8T6**********************
 * �ļ���  ��DHT11.c
 * ����    ��DHT11������
 * ��ע    : DHT11�¶�ʪ�ȴ�����
 * �ӿ�    ��PA11-DATA

********************LIGEN*************************/
#include "dht11.h"
#include "delay.h"

#define DT GPIO_Pin_8
#define DHT11_DQ_IN GPIO_ReadInputDataBit(GPIOB,DT)

void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = DT;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = DT;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void DHT11OUT_H(void)
{
	GPIO_SetBits(GPIOB,DT);
}
void DHT11OUT_L(void)
{
	GPIO_ResetBits(GPIOB,DT);
}
//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11OUT_L(); 	//����DQ
	delay_ms(20);    	//��������18ms
	DHT11OUT_H(); 	//DQ=1 
	delay_us(30);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(float *temp,float *humia)    
{        
 	u8 buf[6];
	u8 i,j;
	u16 tem;
	for(j=0;j<10;j++)
		{
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		
			for(i=0;i<5;i++)//��ȡ40λ����
			{
				buf[i]=DHT11_Read_Byte();
			}
			tem=buf[0]+buf[1]+buf[2]+buf[3];
			buf[5]=tem;
			if(buf[4]==buf[5])
			{
				*humia=(float)(buf[0]<<8 | buf[1])/10 ;
				*temp= (float)(buf[2]<<8 | buf[3])/10 ;
				break;				
			}
			DHT11_Rst();
			delay_ms(1000); 
		
		}
	}	
//	else return 1;
	return 0;	    
}

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
 	GPIO_SetBits(GPIOB,DT);						 //PG11 �����
			    
	DHT11_Rst();  //��λDHT11
	return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
} 

