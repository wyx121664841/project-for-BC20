#ifndef DHT11_H
#define DHT11_H

#include "stm32f0xx.h"

//PA11
//#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;}
//#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;} 
//IO��������   
//#define	DHT11_DQ_OUT PAout(11)
//#define	DHT11_DQ_IN  PAin(11) 

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(float *temp,float *humia);//��ȡ����
u8 DHT11_Read_Byte(void);//��ȡһ���ֽ�
u8 DHT11_Read_Bit(void);//��ȡһλ
u8 DHT11_Check(void);//���DHT11
void DHT11_Rst(void);//��λDHT11   
void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);
void DHT11OUT_H(void);
void DHT11OUT_L(void);
#endif
