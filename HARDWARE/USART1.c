

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
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
/* FILE is typedef�� d in stdio.h. */
FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}
//�ض���fputc����
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

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //ʹ��GPIOA��ʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  //ʹ��GPIOB��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//ʹ��USART��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//ʹ��USART��ʱ��

    /* USART1�Ķ˿����� */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//����PA9�ɵڶ���������	TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//����PA10�ɵڶ���������  RX
	
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);//����PA9�ɵڶ���������	TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);//����PA10�ɵڶ���������  RX
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;//485�շ�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* USART1�Ļ������� */
    USART_InitStructure.USART_BaudRate = baud;              //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ClearFlag(USART1, USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //ʹ�ܽ����ж�
    USART_ITConfig(USART1, USART_IT_ORE, ENABLE);   //��������ж�
    USART_Cmd(USART1, ENABLE);                             //ʹ��USART1

//
    /* USART2�Ļ������� */
    USART_InitStructure.USART_BaudRate = 115200;              //������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);           //ʹ�ܽ����ж�
    USART_ITConfig(USART2, USART_IT_ORE, ENABLE);//��������ж�
    USART_Cmd(USART2, ENABLE);                             //ʹ��USART2


    /* USART1\2��3��NVIC�ж����� */
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
    //ʹ��TIM3ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM14, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���


    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;//TIM14�ж�
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���


    //��ʱ����ʱʱ��T���㹫ʽ��T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK
    TIM_TimeBaseStructure.TIM_Period = (60-1);//�Զ���װ��ֵ10--��ʱʱ��(10*4800/48M)s
    TIM_TimeBaseStructure.TIM_Prescaler =(4800-1);//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ��
    TIM_TimeBaseStructure.TIM_ClockDivision =0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);	//ʹ��TIM3�ж�Դ

    TIM_TimeBaseStructure.TIM_Period = (100-1);//�Զ���װ��ֵ10--��ʱʱ��(10*4800/48M)s
    TIM_TimeBaseStructure.TIM_Prescaler =(4800-1);//Ԥ��Ƶֵ��+1Ϊ��Ƶϵ��
    TIM_TimeBaseStructure.TIM_ClockDivision =0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);	//ʹ��TIM14�ж�Դ

    //ʹ��TIMx����
    TIM_Cmd(TIM3, DISABLE);//�رն�ʱ��
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
//�ļ����ƣ�
//���ܸ�Ҫ��USART1�жϺ���
//����˵������
//�������أ���
//=============================================================================
void USART1_IRQHandler(void)
{

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {

        USART_ClearFlag(USART1, USART_FLAG_RXNE);
        USART_ClearITPendingBit(USART1,USART_IT_ORE);//������������־
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
        USART_ClearITPendingBit(USART2,USART_IT_ORE);//������������־
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



//��ʱ���жϺ���

void TIM3_IRQHandler(void)			//1ms����һ���ж�   ����1 ����
{
    uint8_t cc,len;
    if ( TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET ) //�Ƿ����ж�
    {

        TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);    //����жϴ�����λ
        TIM_Cmd(TIM3, DISABLE);
        USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);           //�رս����ж�  AT+QIDEACT
        memset(ReFlame_OneBak,0,150);
			  memcpy(ReFlame_OneBak,rxbuff1,count);
				len=count;count=0;
				USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //ʹ�ܽ����ж�

			      rs485_w();//485��ӡ����3��Ϣ
        delay_ms(1);
        for(cc=0; cc<wireless_rxbuff_count; cc++) //������1��������
        {
            USART_SendData(USART1,wireless_rxbuff[cc]);
            while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
        }
      
         rs485_r();	
				 rs485_w();		
				 delay_us(3);
        for(cc=0; cc<len; cc++) //�������ݵ�����2�۲�   485
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
			y[num]=(char)RB[num];//ǿ��ת��
		}

//				y=strstr((const char*)RB,(const char*)"99.99,,");
//		long h = atoi(y);			

//		x=strstr(h,"GBGGA");


//h[t]=*y;

        memset(rxbuff1,0,150);
    }
}
void TIM14_IRQHandler(void)					//5ms����һ���ж�   ����2 ����
{
	  char *pcBegin=0;

    if ( TIM_GetITStatus(TIM14,TIM_IT_Update) != RESET ) //�Ƿ����ж�
    {
        TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);     //����жϴ�����λ
        TIM_Cmd(TIM14, DISABLE);

        USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);           //�رս����ж�
				
			  pcBegin=strstr((const char*)rxbuff_485,(const char*)"*#");   
			  if(pcBegin==NULL)    //�������ò���ʱ����ת��������1 Ҳ���ǲŷ��͵�4gģ��
				{
					sendBUFF(rxbuff_485);
				}
    }
        /*********************************************************************/
        ac=0;   //����2��������ָ��
        memset(rxbuff_485,0,150);
        USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);           //�رս����ж�
}




void sendchar(uint8_t ch)     //ͬʱ���͵�����1�ʹ���2�Ա�۲�����
{
    USART_SendData(USART1,ch);
    while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET) {}

}

void sendchar2(uint8_t ch)       //����2 ����
{
    USART_SendData(USART2,ch);
    while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);

}
void sendBUFF(uint8_t *p)       //����һ������
{
    while((*p)&&(*p!=0xff))
    {
        sendchar(*p);
        p++;
    }
}
void sendBUFF2(uint8_t *p)       //����һ������
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


/*���Ź�*/
void IWDG_Config(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
  IWDG_SetPrescaler(IWDG_Prescaler_256);		  //����IWDGԤ��Ƶֵ��256��Ƶ���
  IWDG_SetReload(4000);	   //����IWDG����װ��ֵ	:��Χ0~0x0FFF
  IWDG_ReloadCounter();	   //ι��������װ�ؼ�����
//    IWDG_Enable();			   //ʹ��IWDG��ʱ��
}
