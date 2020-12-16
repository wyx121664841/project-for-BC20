/***************STM32F103C8T6**********************
 * 文件名  ：DHT11.c
 * 描述    ：DHT11传感器
 * 备注    : DHT11温度湿度传感器
 * 接口    ：PA11-DATA

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
//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11OUT_L(); 	//拉低DQ
	delay_ms(20);    	//拉低至少18ms
	DHT11OUT_H(); 	//DQ=1 
	delay_us(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
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

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
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
		
			for(i=0;i<5;i++)//读取40位数据
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

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef    GPIO_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
 	GPIO_SetBits(GPIOB,DT);						 //PG11 输出高
			    
	DHT11_Rst();  //复位DHT11
	return DHT11_Check();//等待DHT11的回应
} 

