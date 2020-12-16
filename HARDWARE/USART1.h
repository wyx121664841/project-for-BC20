
#ifndef _USART1_H
#define _USART1_H

#include "stm32f0xx.h"
#include <stdio.h>
#include "stm32f0xx_iwdg.h"

void USART1_Init(uint32_t baud);
void TIM2_Config(void);
void sendchar(uint8_t ch);
void sendBUFF(uint8_t *p);
void sendBUFF3(uint8_t *p);
void rs485_r(void);
void rs485_w(void);

void ali_delayms(uint16_t nms);
void sendBUFF2(uint8_t *p);
void sendchar2(uint8_t ch) ;



#endif
