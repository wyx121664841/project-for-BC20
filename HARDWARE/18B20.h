#ifndef __18B20_H
#define __18B20_H

#include "stm32f0xx.h"

void DZX_O(void);
void DZX_I(void);
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Reset(void);
double DS18B20_Get_wd(void);
void DS18B20_Write_Byte(uint8_t dat);
void READ_DS18B20_ID(void);
u8 re2bit(void);
void search_DS18B20(void);
void write_1bit(u8 dat);
double DS18B20_Get_tempx(u8 x);



#define DZX_L GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define DZX_H GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define DZX_V GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
#define MaxSensorNum 8









#endif


