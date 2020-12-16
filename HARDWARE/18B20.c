#include "18B20.h"
#include "xpt2046.h"
#include "string.h"
#include "delay.h"
extern u8 DSID[8];
unsigned char DS18B20_ID[MaxSensorNum][8]= {0};
unsigned char DS18B20_SensorNum=0;
void DZX_O(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}


void DZX_I(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

uint8_t DS18B20_Read_Byte(void)
{
    uint8_t i=0;
    uint8_t TempData=0;

    for(i=0; i<8; i++)
    {
        TempData>>=1;

        DZX_O();
        DZX_L;
        delay_us(2);
        DZX_H;
        DZX_I();
        delay_us(2);

        if(DZX_V==1)
        {
            TempData|=0x80;
        }
        delay_us(60);
    }

    return TempData;
}

void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t i=0;

    DZX_O();

    for(i=0; i<8; i++)
    {
        DZX_L;
        delay_us(10);

        if((dat&0x01)==1)
        {
            DZX_H;
        }
        else
        {
            DZX_L;
        }
        delay_us(60);
        DZX_H;
        dat>>=1;
    }
}

void DS18B20_Reset(void)
{
    DZX_O();
    DZX_L;
    delay_us(750);
    DZX_H;
    delay_us(480);
}

//double DS18B20_Get_wd(void)
//{
//    uint8_t TL=0,TH=0;
//    uint16_t temp=0;
//    double wd=0;
//
//    DS18B20_Reset();
//    DS18B20_Write_Byte(0xCC);
//    DS18B20_Write_Byte(0x44);
//	//Reconfigure_SYSTICK();
//    delay_ms(800);
//
//    DS18B20_Reset();
//    DS18B20_Write_Byte(0xCC);
//    DS18B20_Write_Byte(0xBE);
//
//    TL=DS18B20_Read_Byte();
//    TH=DS18B20_Read_Byte();
//
//    temp=TH;
//    temp=(temp<<8)+TL;
//
//    if((temp&0xF800)==0xF800)//负温度
//    {
//        temp=~temp;
//        temp=temp+1;
//        wd=temp*(-0.0625);
//    }
//    else
//    {
//        wd=temp*0.0625;
//    }
//
//	//Reconfigure_SYSTICK();
//    return wd;
//}

void READ_DS18B20_ID(void)
{
    u8 i;
    DS18B20_Reset();
    DS18B20_Write_Byte(0x33);
    for(i=0; i<8; i++)
    {
        DSID[i]=DS18B20_Read_Byte();
    }

}
void search_DS18B20(void)
{
    u8 k, l, chongtuwei, m, n, num;
    u8 zhan[5]= {0};
    u8 ss[64]= {0};
    u8 tempp=0;
    l = 0;
    num = 0;
    DS18B20_SensorNum=0;
    memset(zhan,0,sizeof(zhan));
    memset(ss,0,sizeof(ss));
    do {
        DS18B20_Reset();
        delay_us(480);
        DS18B20_Write_Byte(0xf0);
        for (m = 0; m < 8; m++)
        {
            u8 s = 0;
            for (n = 0; n < 8; n++)
            {
                k = re2bit();
                k = k & 0x03;
                s >>= 1;
                if (k == 0x01)
                {
                    write_1bit(0);
                    ss[(m * 8 + n)] = 0;
                }
                else if (k == 0x02)
                {
                    s = s | 0x80;
                    write_1bit(1);
                    ss[(m * 8 + n)] = 1;
                }
                else if (k == 0x00)
                {

                    chongtuwei = m * 8 + n + 1;
                    if (chongtuwei > zhan[l])
                    {
                        write_1bit(0);
                        ss[(m * 8 + n)] = 0;
                        zhan[++l] = chongtuwei;
                    }
                    else if (chongtuwei < zhan[l])
                    {
                        s = s | ((ss[(m * 8 + n)] & 0x01) << 7);
                        write_1bit(ss[(m * 8 + n)]);
                    }
                    else if (chongtuwei == zhan[l])
                    {
                        s = s | 0x80;
                        write_1bit(1);
                        ss[(m * 8 + n)] = 1;
                        l = l - 1;
                    }
                }
                else
                {

                }
            }
            tempp = s;
            DS18B20_ID[num][m] = tempp;
        }
        num = num + 1;





    } while (zhan[l] != 0 && (num < MaxSensorNum));

    DS18B20_SensorNum = num;

}
u8 re2bit(void)
{
    u8 i,dat;
    for(i=0; i<2; i++)
    {
        dat = dat << 1;
        DZX_O();
        DZX_L;
        delay_us(2);
        DZX_H;
        DZX_I();
        delay_us(2);

        if(DZX_V==1)
        {
            dat|=0x01;
        }
        delay_us(50);
    }
    return dat;



}

void write_1bit(u8 dat)
{
    DZX_O();
    if(dat)
    {
        DZX_L;
        delay_us(10);
        DZX_H;
        delay_us(60);
    } else
    {
        DZX_L;
        delay_us(60);
        DZX_H;
        delay_us(10);
    }
}


double DS18B20_Get_tempx(u8 x)
{
    u8 i;
    u8 TL=0,TH=0;
    u16 temp=0;
    double wd=0;
    DS18B20_Reset();
    DS18B20_Write_Byte(0x55); // 寻找ID命令
    for(i=0; i<8; i++) //写入64位ROM代码 （要读取温度的ID号）
    {
        if(x==1)
        {
            DS18B20_Write_Byte(DS18B20_ID[0][i]);
        }
        if(x==2)
        {
            DS18B20_Write_Byte(DS18B20_ID[1][i]);
        }
        if(x==3)
        {
            DS18B20_Write_Byte(DS18B20_ID[2][i]);
        }
    }
    DS18B20_Write_Byte(0x44); // 启动温度转换
    delay_ms(800);
    DS18B20_Reset();//复位DS18B20
    DS18B20_Write_Byte(0x55); // 寻找ID命令

    for(i=0; i<8; i++) //写入64位ROM代码 （要读取温度的ID号）
    {
        if(x==1)
        {
            DS18B20_Write_Byte(DS18B20_ID[0][i]);
        }
        if(x==2)
        {
            DS18B20_Write_Byte(DS18B20_ID[1][i]);
        }
        if(x==3)
        {
            DS18B20_Write_Byte(DS18B20_ID[2][i]);
        }
    }


    DS18B20_Write_Byte(0xbe);//发送温度读取命令(读暂存器)0xbe
    TL=DS18B20_Read_Byte();
    TH=DS18B20_Read_Byte();

    temp=TH;
    temp=(temp<<8)+TL;

    if((temp&0xF800)==0xF800)//负温度
    {
        temp=~temp;
        temp=temp+1;
        wd=temp*(-0.0625);
    }
    else
    {
        wd=temp*0.0625;
    }

    //Reconfigure_SYSTICK();

    return wd;

}























