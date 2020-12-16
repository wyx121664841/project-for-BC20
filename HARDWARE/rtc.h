/**
  ********************************  STM32F0xx  *********************************
  * @�ļ���     �� rtc.h
  * @����       �� strongerHuang
  * @��汾     �� V1.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��06��05��
  * @ժҪ       �� RTC�ײ�ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _RTC_H
#define _RTC_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f0xx.h"


/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct
{
    uint8_t Year;
    uint8_t Month;
    uint8_t Date;
    uint8_t Week;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;
} RTC_DateTimeTypeDef;

typedef enum {
    RTC_OK = 0x00,                  //��ȷ
    RTC_ERR,                        //����
    RTC_TIMEOUT,                    //��ʱ
    RTC_EXIT,                       //�˳�
    RTC_OVERFLOW,                   //���
} RTC_RESULT;

/* �궨�� --------------------------------------------------------------------*/
#define SEC_DAY                   (24*3600)                //һ������
#define SEC_YEAR                  (365*SEC_DAY)            //һ������(ƽ��)

/* �������� ------------------------------------------------------------------*/
void RTC_Configuration(void);
void RTC_Initializes(void);
RTC_RESULT RTC_DateRegulate(RTC_DateTimeTypeDef RTC_DateTimeStructure);
RTC_RESULT RTC_TimeRegulate(RTC_DateTimeTypeDef RTC_DateTimeStructure);
RTC_RESULT RTC_SetDateTime(RTC_DateTimeTypeDef RTC_DateTimeStructure);
void RTC_GetDateTime(RTC_DateTimeTypeDef *RTC_DateTimeStructure);


#endif /* _RTC_H */

/**** Copyright (C)2016 strongerHuang. All Rights Reserved **** END OF FILE ****/
