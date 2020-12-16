

#include "USART1.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_iwdg.h"
#include "stm32f0xx_usart.h"
#include <stdio.h>
#include "string.h"
#include "delay.h"
#include <time.h>    
#include <stdlib.h> 
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
/* FILE is typedef’ d in stdio.h. */
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
    while(!((USART1->ISR)&(1<<7))) {}
    USART1->TDR=ch;
    return (ch);
}
#endif
char y[200];
uint8_t h[200];
int t=0;
char *x;
/********************************************************/
uint8_t rxbuff1[200];
uint8_t rxbuff_485[200];

uint8_t ReFlame_OneBak[200];
uint8_t count=0;
uint8_t ac=0;
extern uint8_t cuu;
uint8_t wireless_rxbuff[200];
uint8_t wireless_rxbuff_count;

uint16_t num;

 unsigned char RB[200];

void USART1_Init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //使能GPIOB的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//使能USART的时钟

    /* USART1的端口配置 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX
	
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);//配置PA9成第二功能引脚	TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);//配置PA10成第二功能引脚  RX
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;//485收发控制
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* USART1的基本配置 */
    USART_InitStructure.USART_BaudRate = baud;              //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ClearFlag(USART1, USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);   //数据溢出中断
    USART_Cmd(USART1, ENABLE);                             //使能USART1

//
    /* USART2的基本配置 */
    USART_InitStructure.USART_BaudRate = 115200;              //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);           //使能接收中断
    USART_ITConfig(USART2, USART_IT_ORE, ENABLE);//数据溢出中断
    USART_Cmd(USART2, ENABLE);                             //使能USART2


    /* USART1\2、3的NVIC中断配置 */
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

void TIM2_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;
    //使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM14, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器


    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;//TIM14中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器


    //定时器定时时间T计算公式：T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
    TIM_TimeBaseStructure.TIM_Period = (60-1);//自动重装载值10--定时时间(10*4800/48M)s
    TIM_TimeBaseStructure.TIM_Prescaler =(4800-1);//预分频值，+1为分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision =0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	//使能TIM3中断源

    TIM_TimeBaseStructure.TIM_Period = (100-1);//自动重装载值10--定时时间(10*4800/48M)s
    TIM_TimeBaseStructure.TIM_Prescaler =(4800-1);//预分频值，+1为分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision =0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);	//使能TIM14中断源

    //使能TIMx外设
    TIM_Cmd(TIM3, DISABLE);//关闭定时器
    TIM_Cmd(TIM14, DISABLE);
}
void rs485_r(void)
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}
void rs485_w(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
//=============================================================================
//文件名称：
//功能概要：USART1中断函数
//参数说明：无
//函数返回：无
//=============================================================================
void USART1_IRQHandler(void)
{

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {

        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART1,USART_IT_ORE);//清除数据溢出标志
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        USART_ClearITPendingBit(USART1,USART_IT_NE);
        USART_ClearITPendingBit(USART1,USART_IT_FE);
        rxbuff1[count]=USART_ReceiveData(USART1);
        count++;
        if(count>199)count=0;
        TIM3->CNT=0;
        TIM_Cmd(TIM3, ENABLE);
    }
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_ORE);//清除数据溢出标志
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        USART_ClearITPendingBit(USART2,USART_IT_NE);
        USART_ClearITPendingBit(USART2,USART_IT_FE);

        rxbuff_485[ac]=USART_ReceiveData(USART2);
        ac++;
        if(ac>200)ac=0;

        TIM14->CNT=0;
        TIM_Cmd(TIM14, ENABLE);
    }
}



//定时器中断函数

void TIM3_IRQHandler(void)			//1ms进入一次中断   串口1 处理
{
    uint8_t cc,len;
    if ( TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET ) //是否发生中断
    {

        TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);    //清除中断待处理位
        TIM_Cmd(TIM3, DISABLE);
        USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);           //关闭接收中断  AT+QIDEACT
        memset(ReFlame_OneBak,0,150);
			  memcpy(ReFlame_OneBak,rxbuff1,count);
				len=count;count=0;
				USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断

			      rs485_w();//485打印串口3信息
        delay_ms(1);
        for(cc=0; cc<wireless_rxbuff_count; cc++) //往串口1发送数据
        {
            USART_SendData(USART1,wireless_rxbuff[cc]);
            while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
        }
      
         rs485_r();	
				 rs485_w();		
				 delay_us(3);
        for(cc=0; cc<len; cc++) //发送数据到串口2观察   485
        {
            USART_SendData(USART2,ReFlame_OneBak[cc]);
            while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
        }  
				 rs485_r(); 
				
for(num=0;num<200;num++) 
{
//	int *RB= &ReFlame_OneBak;

 	RB[num]=rxbuff1[num];

//	int *i= &RB;
//            (ReFlame_OneBak->*RB)();					 
}
//for(t=0; t<100; t++)
//{h[t] = ReFlame_OneBak[t];}
	for(num=0;num<200;num++)
		{
		//		 *xinh=strx[0]; 			
			y[num]=(char)RB[num];//强制转换
		}

//				y=strstr((const char*)RB,(const char*)"99.99,,");
//		long h = atoi(y);			

//		x=strstr(h,"GBGGA");


//h[t]=*y;

        memset(rxbuff1,0,150);
    }
}
void TIM14_IRQHandler(void)					//5ms进入一次中断   串口2 处理
{
	  char *pcBegin=0;

    if ( TIM_GetITStatus(TIM14,TIM_IT_Update) != RESET ) //是否发生中断
    {
        TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);     //清除中断待处理位
        TIM_Cmd(TIM14, DISABLE);

        USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);           //关闭接收中断
				
			  pcBegin=strstr((const char*)rxbuff_485,(const char*)"*#");   
			  if(pcBegin==NULL)    //不是设置参数时，才转发到串口1 也就是才发送到4g模块
				{
					sendBUFF(rxbuff_485);
				}
    }
        /*********************************************************************/
        ac=0;   //串口2接收数组指针
        memset(rxbuff_485,0,150);
        USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);           //关闭接收中断
}




void sendchar(uint8_t ch)     //同时发送到串口1和串口2以便观察数据
{
    USART_SendData(USART1,ch);
    while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET) {}

}

void sendchar2(uint8_t ch)       //串口2 发送
{
    USART_SendData(USART2,ch);
    while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);

}
void sendBUFF(uint8_t *p)       //发送一串数据
{
    while((*p)&&(*p!=0xff))
    {
        sendchar(*p);
        p++;
    }
}
void sendBUFF2(uint8_t *p)       //发送一串数据
{
    while((*p)&&(*p!=0xff))
    {
        sendchar2(*p);
        p++;
    }
}


void ali_delayms(uint16_t nms)
{
    uint16_t i,j;
    for(i=0; i<nms; i++)
        for(j=0; j<24000; j++);
}


/*看门狗*/
void IWDG_Config(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对寄存器IWDG_PR、IWDG_RLR的写操作
  IWDG_SetPrescaler(IWDG_Prescaler_256);		  //设置IWDG预分频值：256分频最大
  IWDG_SetReload(4000);	   //设置IWDG的重装载值	:范围0~0x0FFF
  IWDG_ReloadCounter();	   //喂狗：重新装载计数器
//    IWDG_Enable();			   //使能IWDG定时器
}
