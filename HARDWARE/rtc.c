/**
  ********************************  STM32F0xx  *********************************
  * @文件名     ： rtc.c
  * @作者       ： strongerHuang
  * @库版本     ： V1.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年06月05日
  * @摘要       ： RTC底层源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2016-06-05 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "rtc.h"
#include "stm32f0xx_rtc.h"
#include "stm32f0xx_pwr.h"

/* 变量 ----------------------------------------------------------------------*/
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

#define RTC_CLOCK_SOURCE_LSI                     //使用LSI时钟
#define RTC_FLAG_BKP              0xA55A         //标志


/************************************************
函数名称 ： RTC_Configuration
功    能 ： RTC配置
参    数 ： 无
返 回 值 ： 无
作    者 ： MCD Application Team
*************************************************/
void RTC_Configuration(void)
{
    /* 1、使用LSI时钟 */
#if defined (RTC_CLOCK_SOURCE_LSI)

    RCC_LSICmd(ENABLE);                            //使能LSI时钟

    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {   //等待LSI就绪
    }

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);        //选择LSI时钟

    SynchPrediv = 0x18F;
    AsynchPrediv = 0x63;

    /* 2、使用LSE时钟 */
#elif defined (RTC_CLOCK_SOURCE_LSE)

    RCC_LSEConfig(RCC_LSE_ON);                     //使能LSI时钟

    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {   //等待LSI就绪
    }

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);        //选择LSI时钟

    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;

#else
#error Please select the RTC Clock source inside the main.c file
#endif

    RCC_RTCCLKCmd(ENABLE);                         //使能RTC

    RTC_WaitForSynchro();                          //等待同步
}

/************************************************
函数名称 ： RTC_Initializes
功    能 ： RTC初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： Huang Fugui
*************************************************/
void RTC_Initializes(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    RTC_DateTimeTypeDef RTC_DateTimeStructure;

    PWR_BackupAccessCmd(ENABLE);                   //允许操作RTC

    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != RTC_FLAG_BKP)
    {
        RTC_Configuration();                         //配置RTC

        RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
        if(RTC_Init(&RTC_InitStructure) == ERROR)
        {
            while(1);                                  //初始化失败
        }

        RTC_DateTimeStructure.Year = 19;
        RTC_DateTimeStructure.Month = 10;
        RTC_DateTimeStructure.Date = 11;
        RTC_DateTimeStructure.Week = 5;
        RTC_DateTimeStructure.Hour = 14;
        RTC_DateTimeStructure.Minute = 42;
        RTC_DateTimeStructure.Second = 0;
        RTC_SetDateTime(RTC_DateTimeStructure);      //首次初始化RTC(2016年6月5日 周七 12:00:00)

        RTC_WriteBackupRegister(RTC_BKP_DR0, RTC_FLAG_BKP);
    }
    else
    {
#ifdef RTC_CLOCK_SOURCE_LSI
        RCC_LSICmd(ENABLE);
#endif

        RTC_WaitForSynchro();                        //等待RTC与RTC_APB时钟同步
    }
}

/************************************************
函数名称 ： RTC_DateRegulate
功    能 ： RTC日期矫正
参    数 ： RTC_DateTimeStructure -- RTC结构体
返 回 值 ： RTC_OK ----------------- 成功
            RTC_ERR ---------------- 错误
作    者 ： Huang Fugui
*************************************************/
RTC_RESULT RTC_DateRegulate(RTC_DateTimeTypeDef RTC_DateTimeStructure)
{
    RTC_DateTypeDef RTC_DateStructure;

    RTC_DateStructure.RTC_Year = RTC_DateTimeStructure.Year;
    RTC_DateStructure.RTC_Month = RTC_DateTimeStructure.Month;
    RTC_DateStructure.RTC_Date =RTC_DateTimeStructure.Date;
    RTC_DateStructure.RTC_WeekDay = RTC_DateTimeStructure.Week;

    if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == SUCCESS)
    {
        return RTC_OK;
    }
    else
    {
        return RTC_ERR;
    }
}

/************************************************
函数名称 ： RTC_TimeRegulate
功    能 ： RTC时间矫正
参    数 ： RTC_DateTimeStructure -- RTC结构体
返 回 值 ： RTC_OK ----------------- 成功
            RTC_ERR ---------------- 错误
作    者 ： Huang Fugui
*************************************************/
RTC_RESULT RTC_TimeRegulate(RTC_DateTimeTypeDef RTC_DateTimeStructure)
{
    RTC_TimeTypeDef RTC_TimeStructure;

    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = RTC_DateTimeStructure.Hour;
    RTC_TimeStructure.RTC_Minutes = RTC_DateTimeStructure.Minute;
    RTC_TimeStructure.RTC_Seconds = RTC_DateTimeStructure.Second;

    if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == SUCCESS)
    {
        return RTC_OK;
    }
    else
    {
        return RTC_ERR;
    }
}

/************************************************
函数名称 ： RTC_SetDateTime
功    能 ： RTC日期时间设置
参    数 ： RTC_DateTimeStructure -- RTC结构体
返 回 值 ： RTC_OK ----------------- 成功
            RTC_ERR ---------------- 错误
作    者 ： Huang Fugui
*************************************************/
RTC_RESULT RTC_SetDateTime(RTC_DateTimeTypeDef RTC_DateTimeStructure)
{
    if(RTC_ERR == RTC_DateRegulate(RTC_DateTimeStructure))
    {
        return RTC_ERR;
    }

    if(RTC_ERR == RTC_TimeRegulate(RTC_DateTimeStructure))
    {
        return RTC_ERR;
    }

    return RTC_OK;
}

/************************************************
函数名称 ： RTC_GetDateTime
功    能 ： 读取RTC日期时间（含日历、星期）
参    数 ： RTC_DateTimeStructure -- RTC结构体
返 回 值 ： 无
作    者 ： Huang Fugui
*************************************************/
void RTC_GetDateTime(RTC_DateTimeTypeDef *RTC_DateTimeStructure)
{
    RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeTypeDef RTC_TimeStructure;

    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);

    RTC_DateTimeStructure->Year = RTC_DateStructure.RTC_Year;
    RTC_DateTimeStructure->Month = RTC_DateStructure.RTC_Month;
    RTC_DateTimeStructure->Date = RTC_DateStructure.RTC_Date;
    RTC_DateTimeStructure->Week = RTC_DateStructure.RTC_WeekDay;
    RTC_DateTimeStructure->Hour = RTC_TimeStructure.RTC_Hours;
    RTC_DateTimeStructure->Minute = RTC_TimeStructure.RTC_Minutes;
    RTC_DateTimeStructure->Second = RTC_TimeStructure.RTC_Seconds;
}


/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
