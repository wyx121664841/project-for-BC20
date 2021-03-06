// ------------------------------------
// 硬件连接：PB10---T-INT
//           PA4----T-CS
//           PA5----T-CLK
//           PA6----T-OUT
// 					 PA7----T-DIN
//           SPI1-- AF0
//
// -------------------------------------

#include "xpt2046.h"
#include "delay.h"
#include "stm32f0xx_spi.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_syscfg.h"
#include "LED.h"
//#include "SYSTICK.h"
Pen_Holder Pen_Point;//定义笔
extern volatile unsigned char touch_flag;
/******************************************************
* 函数名：XPT_Init();
* 描述  ：初始化ADC784的spi接口
* 输入  : 首先配置时钟，然后配置GPIO，最后配置ADC端口复用
* 输出  ：无
* 注意  ：
*********************************************************/
void XPT_Init(void)
{
    //设置需要配置的参数
    GPIO_InitTypeDef  GPIO_InitStruct;
    SPI_InitTypeDef   SPI_InitStruct;
    //配置时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHBPeriphClockCmd(TOUCH_CS_PIN_SCK|TOUCH_SCK_PIN_SCK|TOUCH_MISO_PIN_SCK | TOUCH_MOSI_PIN_SCK|TOUCH_INT_PIN_SCK, ENABLE);
//	 RCC_APB2PeriphClockCmd(TOUCH_SPI2, ENABLE);
    //配置spi使用到的GPIO端口

// 	/< Configure TOUCH_SPI pins: SCK */
    GPIO_InitStruct.GPIO_Pin = TOUCH_SCK_PIN|TOUCH_MOSI_PIN|TOUCH_MISO_PIN;;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(TOUCH_SCK_PORT, &GPIO_InitStruct);

    /*!< Configure TOUCH_SPI pins: MISO */
    GPIO_InitStruct.GPIO_Pin = TOUCH_MISO_PIN;
    GPIO_Init(TOUCH_MISO_PORT, &GPIO_InitStruct);

    /*!< Configure TOUCH_SPI pins: MOSI */
    GPIO_InitStruct.GPIO_Pin =TOUCH_MOSI_PIN;
    GPIO_Init(TOUCH_MOSI_PORT, &GPIO_InitStruct);

    /*!< Configure TOUCH_CS_PIN pin */
    GPIO_InitStruct.GPIO_Pin =TOUCH_CS_PIN| GPIO_Pin_12|GPIO_Pin_1|GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStruct);

    /*!< Configure TOUCH_TIN_PIN pin */
    GPIO_InitStruct.GPIO_Pin =TOUCH_INT_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TOUCH_INT_PORT, &GPIO_InitStruct);
    //复用配置
//  GPIO_PinAFConfig(TOUCH_SCK_PORT, TOUCH_SCK_SOURCE, TOUCH_SCK_AF);
//  GPIO_PinAFConfig(TOUCH_MISO_PORT, TOUCH_MISO_SOURCE, TOUCH_MISO_AF);
//  GPIO_PinAFConfig(TOUCH_MOSI_PORT, TOUCH_MOSI_SOURCE, TOUCH_MOSI_AF);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    //配置spi
    SPI_I2S_DeInit(SPI2);
    SPI_Cmd(SPI2, DISABLE);
    SPI_TOUCH_CS_HIGH();
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStruct);
//  SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
//    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
//    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;		//默认为高电平 控制RC522
//    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;		//从第二个时钟开始传输 即上升沿
//    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
//    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
//    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//    SPI_InitStruct.SPI_CRCPolynomial = 7;
//    SPI_Init(SPI2, &SPI_InitStruct);
//    SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
    SPI_SSOutputCmd(SPI2,ENABLE);
    SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);

    SPI_Cmd(SPI2, ENABLE); /*!< TOUCH_SPI enable */
//    GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_0);

}
//delay_
/******************************************************
* 函数名：TOUCH_INT()；
* 描述  ：AD7845中断嵌套
* 输入  :
* 输出  ：
* 注意  ：
*********************************************************/
void TOUCH_Int(void)
{

    NVIC_InitTypeDef NVIC_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    GPIO_InitTypeDef  GPIO_InitStruct;
    RCC_AHBPeriphClockCmd(TOUCH_MOSI_PIN_SCK, ENABLE);
    GPIO_InitStruct.GPIO_Pin =TOUCH_INT_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TOUCH_INT_PORT, &GPIO_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);


    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource6);
    EXTI_InitStruct.EXTI_Line = EXTI_Line6;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

}

/******************************************************
* 函数名：WR_Cmd();
* 描述  ：xpt spi写命令
* 输入  : cmd
* 输出  ：无
* 注意  ：
*********************************************************/

void WR_Cmd(uint8_t cmd)
{
    /* Wait for SPI1 Tx buffer empty */
//    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
//	//while((SPI2->SR&0x02)==0);		//等待发送区空
//    /* Send SPI1 data */
//    SPI_SendData8(SPI2,cmd);
//    /* Wait for SPI1 data reception */
//    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
//	  //while((SPI2->SR&0x01)==0);//等待接收完一个byte
//    /* Read SPI1 received data */
//    return SPI_ReceiveData8(SPI2);
    uint8_t i;
    clr_SCK;

    for(i=0; i<8; i++)
    {


        if (cmd & 0x80)
            set_SDA;
        else
            clr_SDA;

        cmd <<= 1;
        clr_SCK;
        //	delay_us(1);
        __nop();
        __nop();
        __nop();
        set_SCK;
        //delay_us(1);
        __nop();
        __nop();
        __nop();
    }

}
uint16_t RE_Cmd(void)
{
    uint8_t i;
    uint16_t dwData;
    dwData = 0x00;//Read 8bit
    for(i=0; i<16; i++)
    {
        // delay_us(2) ;
        dwData <<= 1;
        set_SCK;
        //	delay_us(1);
        __nop();
        __nop();
        __nop();
        clr_SCK;
        //delay_us(1);
        __nop();
        __nop();
        __nop();
        if (ReadData)
        {
            dwData |= 1;
        }
        else
            ;

        // 	delay_us(2);
    }
    return dwData;
}


void Touch_Init(void)
{
//	XPT_Init();
    // SPI通信引脚设置
//	GPIO_InitTypeDef GPIO_InitStructure;

//	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
//
//    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
    LCD_GPIO_Init();
    TOUCH_Int();
    // Penirq 中断引脚设置
    SPI_TOUCH_CS_HIGH();
    touch_flag=0;
    Pen_Point.Pen_Sign=Pen_Up;
    ADS_Read_AD(CMD_RDX);
    ADS_Read_AD(CMD_RDY);
}

/******************************************************
* 函数名：DelayUS
* 描述  ：粗略延时cnt 微妙
* 输入  : cnt
* 输出  ：无
* 注意  ：该函数创建的目的是局部使用,从而不必特意去使用定时器来延时
*********************************************************/
//void delay_us(uint32_t cnt)
//{
//    uint16_t i;
//    for(i = 0;i<cnt;i++)
//    {
//        uint8_t us = 22; // 设置值为12，大约延1微秒
//        while (us--)     //延1微秒
//        {
//            ;
//        }
//    }
//}

/*************************************************/
/* 功    能：读取X轴或Y轴的ADC值				 */
/* 入口参数：CMD:命令                            */
/*************************************************/
uint16_t ADS_Read_AD(uint8_t CMD)
{
    uint16_t NUMH;
    uint16_t Num;
    clr_SCK;
    SPI_TOUCH_CS_LOW(); // CS=0 开始SPI通信
//    delay_us(1);
    __nop();
    __nop();
    WR_Cmd(CMD);

    delay_us(6);            // 延时等待转换完成
    set_SCK;
    __nop();
    __nop();
    __nop();
    __nop();
    clr_SCK;
    __nop();
    __nop();
    __nop();
    __nop();
    NUMH=RE_Cmd();
//	NUML=RE_Cmd();
    Num=NUMH;//((NUMH)<<8)+NUML;
    Num>>=4;                // 只有高12位有效.
    SPI_TOUCH_CS_HIGH();  // CS=1 结束SPI通信
    return(Num);
}

#define READ_TIMES 10 //读取次数
#define LOST_VAL 4	  //丢弃值
/*************************************************/
/* 功    能：读取X轴或Y轴的ADC值				 */
/* 入口参数：CMD_RDX:读取X的ADC值                */
/*           CMD_RDY:读取Y的ADC值                */
/* 说    明：与上一个函数相比，这个带有滤波      */
/*************************************************/
uint16_t ADS_Read_XY(uint8_t xy)
{
    uint16_t i, j;
    uint16_t buf[READ_TIMES];
    uint16_t sum=0;
    uint16_t temp;
    for(i=0; i<READ_TIMES; i++)
    {
        buf[i]=ADS_Read_AD(xy);
    }
    for(i=0; i<READ_TIMES-1; i++) //排序
    {
        for(j=i+1; j<READ_TIMES; j++)
        {
            if(buf[i]>buf[j])//升序排列
            {
                temp=buf[i];
                buf[i]=buf[j];
                buf[j]=temp;
            }
        }
    }
    sum=0;
    for(i=LOST_VAL; i<READ_TIMES-LOST_VAL; i++)sum+=buf[i];
    temp=sum/(READ_TIMES-2*LOST_VAL);
    return temp;
}
/*************************************************/
/* 功    能：读取X轴和轴的ADC值                  */
/* 入口参数：&Pen_Point.X_ADC,&Pen_Point.Y_ADC   */
/* 出口参数：0：成功（返回的X,Y_ADC值有效）      */
/*           1: 失败（返回的X,Y_ADC值无效）      */
/*************************************************/
uint8_t Read_ADS(uint16_t *x,uint16_t *y)
{
    uint16_t xtemp,ytemp;
    xtemp=ADS_Read_XY(CMD_RDX);
    ytemp=ADS_Read_XY(CMD_RDY);
    if(xtemp<100||ytemp<100)return 1;//读数失败
    *x=xtemp;
    *y=ytemp;
    return 0;//读数成功
}
/*************************************************/
/* 功能：连续两次读取ADC值						 */
/* 原理：把两次读取的值作比较，在误差范围内可取  */
/* 入口参数：&Pen_Point.X_ADC,&Pen_Point.Y_ADC   */
/* 出口参数：0：成功（返回的X,Y_ADC值有效）      */
/*           1: 失败（返回的X,Y_ADC值无效）      */
/*************************************************/
#define ERR_RANGE 50 //误差范围 
uint8_t Read_ADS2(uint16_t*x,uint16_t *y)
{
    uint16_t x1,y1;
    uint16_t x2,y2;
    uint8_t res;

    res=Read_ADS(&x1,&y1);  // 第一次读取ADC值
    if(res==1)return(1);	// 如果读数失败，返回1
    res=Read_ADS(&x2,&y2);	// 第二次读取ADC值
    if(res==1)return(1);   	// 如果读数失败，返回1
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
            &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 0;	 // 正确读取，返回0
    } else return 1;	 // 前后不在+-50内，读数错误
}

/*************************************************/
/* 功能：把读出的ADC值转换成坐标值               */
/*************************************************/
void Change_XY(void)
{
    Pen_Point.X_Coord=(320-(Pen_Point.X_ADC-120)/11.65); // 把读到的X_ADC值转换成TFT X坐标值
    Pen_Point.Y_Coord=(480-(Pen_Point.Y_ADC)/8.0625); // 把读到的Y_ADC值转换成TFT Y坐标值
}

/*************************************************/
/* 功能：读取一次XY坐标值                        */
/*************************************************/
uint8_t Read_Once(void)
{
//	  touch_flag=0;
    Pen_Point.Pen_Sign=Pen_Up;
    if(Read_ADS2(&Pen_Point.X_ADC,&Pen_Point.Y_ADC)==0)	// 如果读取数据成功
    {
        while(SPI_TOUHC_INT==0);   // 检测笔是不是还在屏上
        Change_XY();   // 把读到的ADC值转变成TFT坐标值
        return 0;	// 返回0，表示成功
    }
    else return 1;	// 如果读取数据失败，返回1表示失败
}
/*************************************************/
/* 功能：持续读取XY坐标值                        */
/*************************************************/
uint8_t Read_Continue(void)
{
//	  touch_flag=0;
    Pen_Point.Pen_Sign=Pen_Up;
    if(Read_ADS2(&Pen_Point.X_ADC,&Pen_Point.Y_ADC)==0)	 // 如果读取数据成功
    {
        Change_XY();   // 把读到的ADC值转变成TFT坐标值
        return 0;	   // 返回0，表示成功
    }
    else return 1;	   // 如果读取数据失败，返回1表示失败
}











