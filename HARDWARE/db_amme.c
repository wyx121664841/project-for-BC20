/****************SPI ���ö˿�ģ��**********************************/
#include "math.h"
#include "db_amme.h"
#include "xpt2046.h"
#include "iodef.h"
#include "LED.h"
#include "delay.h"
#include "Flash_Mem.h"
//#include "SPI.h"
//u32 CurrentElectric;				//�ܵ���
//u32 CurrentElectricA;
//u32 CurrentElectricB;
//u32 CurrentElectricC;


#define HFCONST   369//((2.592 * ATT_G * ATT_G*10000*INATTV*INATTI)/(EC*UI*VI)) 
#define P_K   0.0515//2.592*pow(10,10)	/(HFCONST *EC *pow(2,23))  0.01225 //0.965595/HFCONST//0.0025745

//u32 Device_ID = 0;


//u8 VoltageA = 0;   //A ���ѹ��Чֵ

//u32 PA_Q = 0;  // A����
float CurrentA = 0;	 //A �������Чֵ
float  TempA = 0; //A �๦������
unsigned int Up_VolA = 0;
unsigned int Up_CurrentA = 0;
u32 Up_PA_Q = 0;
u32 Up_PA_Power = 0;
u32 Up_QA_Power = 0;


u8 VoltageB = 0;   //B ���ѹ��Чֵ
//u32 PB_Q = 0;  // B����
float CurrentB = 0;	 //B �������Чֵ
float TempB = 0; //B �๦������


//u16 Huganqibeilv = 1;                             //����������
float V_modulus = 0.00117;						//��ѹУ��ϵ��
float I_modulus = 2.92E-6;						//����У��ϵ��
float P_modulus = 0.029;						//����У��ϵ��
unsigned long int Apparent_Energy0; // ����100���ĵ���
unsigned long int Sum;
unsigned long int  Apparent_Energy_Buff = 0; 		 //KWH
unsigned char Read_Energy_flag;
unsigned long int Meter_Checksum = 0x000121AD;	//���ģ��Ĵ���У���
MeterData Pre_MeterData;
u8 SetGainData;
extern u8 Derror;
extern int eget;
/*****************SPI GPIO ���ܶ���******************************/
void SPI_GPIO_Config(void)
{
    //����һ��GPIO_InitTypeDef���͵Ľṹ��//
    GPIO_InitTypeDef GPIO_InitStructure;

    //����GPIOB�͵�����ʱ��//
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC,ENABLE);
////    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOE, ENABLE);

    //��������ģʽΪͨ���������//

//	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_8 ;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//

    GPIO_InitStructure.GPIO_Pin =SPI1_MISO ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  SPI1_SCK | SPI1_MOSI|GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SPI1_IO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | SPI7053RST|GPIO_Pin_11|GPIO_Pin_12 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(SPI1_IO, &GPIO_InitStructure);

//    GPIO_SetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_12);


}

/************************* READ_Att70xx*************************************************/
u32 READ_Att70xx(u8 Address)
{
    u8 i, bTemp;
    u32 dwData=0;
    Address&= 0x7F;
    SPI_CS_HIGH();
//    CSx_set(x);
    delay_us(20) ;
    SPI_CLK_LOW();
    delay_us(20) ;
    SPI_CS_LOW();    // ����SPI����
//    CSx_clr(x);
    bTemp = 0x80;
    for(i=0; i<8; i++)   //Write the Address
    {

        delay_us(20) ;
        //        SPI_CLK_LOW();
        SPI_CLK_HIGH();
        //	delay_us(2);
        if(Address & bTemp)
        {
            SPI_DI_HIGH();
        }
        else
            SPI_DI_LOW();
        Address <<= 1;
        delay_us(20);
        //      SPI_CLK_HIGH();
        SPI_CLK_LOW();
        //  delay_us(2) ;
    }
    SPI_DI_LOW();
    delay_us(20);

    dwData = 0x00000000;//Read 24bit
    for(i=0; i<24; i++)
    {
        SPI_CLK_LOW();
        delay_us(20) ;
        dwData <<= 1;
        SPI_CLK_HIGH();
        delay_us(20);

        if (RDSPIData)
        {
            dwData |= 1;
        }
        else
            ;
        delay_us(20);



        //  delay_us(2);
    }
    delay_us(20);

    SPI_CLK_HIGH();
    delay_us(20);
    SPI_CS_HIGH();
    if(dwData==0xffffff)
    {
        return 0;
        Derror++;
    }
    else
    {
        eget=0;
        return dwData;
    }
}
/**************************Wr_Dat_Att70xx******************************/
//void CSx_set(u8 x)
//{
//    switch (x)
//    {
//    case 1 :
//        SPI_CS2_HIGH();
//        break;
//    case 2 :
//        SPI_CS2_HIGH();
//        break;
//    case 3 :
//        SPI_CS2_HIGH();
//        break;
//    }
//}

//void CSx_clr(u8 x)
//{
//    switch (x)
//    {
//    case 1 :
//        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
//        break;
//    case 2 :
//        SPI_CS2_LOW();
//        break;
//    case 3 :
//        SPI_CS3_LOW();
//        break;
//    }
//}
void Write_Att70xx(u8 Address, u32 dwData)
{
    u8 i, bTemp;
    u32 dwTemp;

    SPI_CS_HIGH();
//    CSx_set(x);
    delay_us(20) ;
    SPI_CLK_LOW();
    delay_us(20) ;
//    CSx_clr(x);

    SPI_CS_LOW();	 //����SPI����

    Address |= 0x80;//write
    bTemp = 0x80;
    for(i=0; i<8; i++)         //Write the Address
    {
        delay_us(20);
        SPI_CLK_HIGH();
        if(Address & bTemp)
        {
            SPI_DI_HIGH();
        }
        else
            SPI_DI_LOW();
        bTemp >>= 1;
        delay_us(20) ;
        SPI_CLK_LOW();

    }
    SPI_DI_LOW();
    delay_us(20);

    dwTemp = 0x800000;
    for(i=0; i<24; i++)  //Write Data
    {
        delay_us(20);
        SPI_CLK_HIGH();

        if(dwData & dwTemp)
        {
            SPI_DI_HIGH();
        }
        else
            SPI_DI_LOW();
        delay_us(20);
        //   delay_us(2);
        SPI_CLK_LOW();

        dwTemp >>= 1;
    }

    SPI_CS_HIGH();	    //��������
//    CSx_set(x);
    delay_us(20);
    SPI_CLK_HIGH();
    delay_us(20);
}
//void EMU_init(void)
//{
//	u8 i;
//	for(i=1;i<=3;i++)
//	{
//    Write_Att70xx(0x33,0x55,i); //REST
//    delay_us(30);
//    Write_Att70xx(0x32,0xA6,i); //Open the register of 50-70
//    Write_Att70xx(0x50,0x7880,i);
//    Write_Att70xx(0x51,0x7880,i);
//    Write_Att70xx(0x52,0x7880,i);
//    Write_Att70xx(0x59,0x0528,i);//ADC ��ѹ1�� ����8��  //0528
//    Write_Att70xx(0x61,0x00B9,i);//ConF��ѹ1�� ����8��1d/f11Ϊ���Լ���ֵʵ�ʵ���/���Ե���=0x40/e50(0x40Ϊ�Ĵ���Ĭ��ֵ)
//    Write_Att70xx(0x69,0x10,i);  // ����1 ��ЧֵУ׼
//    Write_Att70xx(0x6B,(int)(pow(571,2)/pow(2,15)),i);  // ��ѹ ��ЧֵУ׼
//    Write_Att70xx(0x32,0,i); //Close the register of 50-70
//    Write_Att70xx(0x32,0xBC,i); //Open the register of 40-45
//    Write_Att70xx(0x40,0x2010,i); //  ������������
//    Write_Att70xx(0x41,0x00CC,i);
//    Write_Att70xx(0x32,0,i); //Open the register of 50-70
//	}
//}
void InitAmmeter(void)
{
//	data0 = ((2.592 * ATT_G * ATT_G*10000*INATTV*INATTI)/(EC*UI*VI));
//	data1 = 25920000000	/(HFCONST *EC *8388608);//0.0025745//

    SPI_GPIO_Config(); //�ܽų�ʼ��
    //  EMU_init();    // ����ʼ��
    delay_us(100);
}

//void ReadAmmeterData(u8 x)
//{
//    u8 *Pdata;
//    u8 i;
//    long int  Curre = 0;
//    unsigned long Vol = 0;
//    unsigned long int Active_Power_buff = 0 ;				//  Active Power
//    unsigned long int Reactive_Power_buff = 0 ;				//  Active Power
//    unsigned long int Apparent_Power_buff = 0;				//  ���ڹ���
////	unsigned int Voltage = 0;
//    float Current = 0;
//    float Active_Power0  =0; //Active Power
//    float Reactive_Power0  = 0;///Apparent Power
//    float Apparent_Power0  = 0;///Apparent Power
//    unsigned long int Apparent_Energy_buff0 = 0; 		// dian liang

////    Sum = READ_Att70xx(0x18);//У��Ĵ���Ч���
////    if(Sum != Meter_Checksum) //�Ĵ���У���Meter_Checksum
////    {
////        InitAmmeter();
////        return;
////    }
//    READ_Att70xx(10,x);
//    Vol = READ_Att70xx(0x00,x);  //vo
//    if (Vol == 0xffffff)
//    {
//        Vol = 0;
//    }
//    Pre_MeterData.Voltage = (unsigned int)(Vol*V_modulus);//218/2445950
//    delay_us(10);

//    Curre = READ_Att70xx(0x06,x);   //curr
//    delay_us(10);
//    if (Curre == 0xffffff)
//    {
//        Curre = 0;
//    }
//    Active_Power_buff = READ_Att70xx(0x0A,x);  // Active Power
//    delay_us(10);
//    Reactive_Power_buff = READ_Att70xx(0x0B,x);  //Reactive Power
//    delay_us(10);
//    Apparent_Power_buff = READ_Att70xx(0x0C,x);   // shi zai gong lv
//    delay_us(10);

//    if(Apparent_Power_buff == 0xFFFFFF)
//    {
//        Apparent_Power_buff = 0;
//    }

//    if(Reactive_Power_buff == 0xFFFFFF)
//    {
//        Reactive_Power_buff = 0;
//    }

//    if(Active_Power_buff == 0xFFFFFF)
//    {
//        Active_Power_buff = 0;
//    }

//    Current = (float)(Curre*I_modulus); // 0.265/438885=6.0380E-7   0.28/395171=7.08554E-7
//    Pre_MeterData.I_Curr = (unsigned int)(Current*1000);

//    if(Reactive_Power_buff > 0x800000)
//    {
//        Reactive_Power_buff = ~((Reactive_Power_buff - 1)<<8);
//        Reactive_Power_buff >>=8;
//    }

//    if(Active_Power_buff > 0x800000)
//    {
//        Active_Power_buff = ~((Active_Power_buff - 1)<<8);
//        Active_Power_buff >>=8;
//    }

//    if(Apparent_Power_buff > 0x800000)
//    {
//        Apparent_Power_buff = ~((Apparent_Power_buff - 1)<<8);
//        Apparent_Power_buff >>=8;
//    }

//    Apparent_Power0 = (float)(Apparent_Power_buff*P_modulus);  //���� 0.265* ��ѹ 219 / -120576   //0.28*220/113766=5.4146
//    Reactive_Power0 = (float)Reactive_Power_buff*P_modulus;
//    Active_Power0 = (float)(Active_Power_buff*P_modulus);

//    if(Active_Power0<0)
//    {
//        Active_Power0 = -Active_Power0;
//    }

//    if(Reactive_Power0<0)
//    {
//        Reactive_Power0 = -Reactive_Power0;
//    }

//    if(Apparent_Power0 < 0)
//    {
//        Apparent_Power0 = -Apparent_Power0;
//    }

//    Pre_MeterData.Active_Power =	(unsigned int)(Active_Power0);
//    Pre_MeterData.Reactive_Power =	(unsigned int)(Reactive_Power0);
//    Pre_MeterData.Apparent_Power = (unsigned int)(Apparent_Power0);
//    delay_us(10);

//    Pre_MeterData.Factor = (unsigned char)((Active_Power0*(100))/(Apparent_Power0));

//    if (Read_Energy_flag == 1)
//    {
//        Read_Energy_flag = 0;
//        Apparent_Energy_buff0 = READ_Att70xx(0x0F,x); // shizai dian liang
//        delay_us(10);
//        if(Apparent_Energy_buff0 == 0xFFFFFF)
//        {
//            Apparent_Energy_buff0 = 0;
//        }

//        if (Apparent_Energy_buff0 <6400)	//��������°�Сʱ��һ�β��ᳬ��2�ȵ�
//        {
//            Apparent_Energy_Buff = (Apparent_Energy_buff0>>5)+Apparent_Energy_Buff;
//            Apparent_Energy0 = Apparent_Energy_Buff;		//�õ���Ҫ��EEPROM
//        }
//    }

//    Pre_MeterData.Apparent_Energy  = (unsigned int)(Apparent_Energy_Buff/100); // ȡ�� ����ʱ������100					//�ϱ��ĵ���							//����

//    Pdata = (u8 *)&Pre_MeterData;

//    for(i=0; i<(sizeof (Pre_MeterData)); i++)
//    {
//
//    }
////		USART_SendData(USART2,sendbuff[cc]);
////    while (USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
//}


extern u32 temp1,temp2,temp3;
extern float KrmsU1,KrmsI11,KrmsI12,KrmsU2,KrmsI21,KrmsI22,KrmsU3,KrmsI31,KrmsI32;//��ѹ����ת��ϵ��
static u32 HFconst=0x006d;
extern volatile unsigned char touch_flag;
void ATT7053_offset(u8 i2)//ATT7053У׼����
{
//    u8 ii,i;
//    u8 datatemp[6];
//    for(i=1; i<=3; i++)
//    {
//        LED_OFF();
//        Write_Att70xx(0x33,0x55,i); //RESET
//        delay_ms(100);
//        //	Write_Att70xx(w_WPREG,0x00);		//�ر�д�����Ĵ�������У������Ĵ���������Ч
//        Write_Att70xx(w_EMUIE,0x0000,i);		//�ر�����EMU�ж�ʹ��
//        Write_Att70xx(w_EMUIF,0x0000,i);		//�������EMU�жϱ�־λ

//        Write_Att70xx(w_SRSTREG,0x55,i);		//��7053BU��λ����λ��üĴ�����0
//        Write_Att70xx(w_WPREG,0xBC,i);		//д�����򿪣�ֻ�ܲ���40H��45H��У������Ĵ��������ɲ���50H��7CH ��У������Ĵ���

//        Write_Att70xx(w_EMUCFG,0x0000,i);		//Ĭ��EMU����
//        Write_Att70xx(w_FreqCFG,0x0088,i);	//����EMUʱ�ӣ�CFP����Ϊ90ms����Чֵ��������Ϊ3.8Hz��EMUʱ��Ƶ��Ϊ1MHz�����β���Ƶ��Ϊ0.976KHz
//        Write_Att70xx(w_ModuleEn,0x007e,i);	//ģʽ���ƼĴ�����ʹ��Ĭ��ֵ
//        Write_Att70xx(w_ANAEN,0x0007,i);		//ADC���ؼĴ���

//        Write_Att70xx(w_IOCFG,0x0000,i);		//����������üĴ�����PF/QF/SFΪ�ߵ�ƽ��Ч���ж�������͵�ƽ��Ч

//        Write_Att70xx(0x32,0xa6,i);//д����,ֻ�ܲ���50H��7CH��У������Ĵ��������ɲ���40H��45H ��У������Ĵ���

//        Write_Att70xx(0x50,0x0000,i);//�й����޹������ڹ���У׼
//        Write_Att70xx(0x51,0x0000,i);
//        Write_Att70xx(0x51,0x0000,i);
//        Write_Att70xx(w_GP2,0x0000,i);		//ͨ��2�й�����У��
//        Write_Att70xx(w_GQ2,0x0000,i);		//ͨ��2�޹�����У��
//        Write_Att70xx(w_GS2,0x0000,i);		//ͨ��2���ڹ���У��
//        Write_Att70xx(0x61,HFconst,i);
//    }



//    for(i=i2; i<=i2; i++)
//    {
//        for(ii=0; ii<20; ii++) //����ͨ��1/2��Чֵƫ��У����������0����ʱУ��
//        {
//            temp1+=READ_Att70xx(0x06,i);
//            delay_ms(20);
//            temp2+=READ_Att70xx(0x07,i);
//            delay_ms(20);
//            temp3+=READ_Att70xx(0x08,i);
//            delay_ms(20);
//        }
//        temp1/=20;
//        temp2/=20;
//        temp3/=20;
//        switch(i)
//        {
//        case 1 : {
//            KrmsU1=220000.0/(float)temp3;//��λ mV
//            KrmsI11=200000.0/(float)temp1;//��λuA
//            KrmsI12=200000.0/(float)temp2;//��λuA
//            datatemp[0]=KrmsI11;
//            datatemp[1]=KrmsI12;
//            datatemp[2]=KrmsU1;

//        }
//        break;
//        case 2 : {
//            KrmsU2=220000.0/(float)temp3;//��λ mV
//            KrmsI21=200000.0/(float)temp1;//��λuA
//            KrmsI22=200000.0/(float)temp2;//��λuA
//            datatemp[0]=KrmsI21;
//            datatemp[1]=KrmsI22;
//            datatemp[2]=KrmsU2;
//        }
//        break;
//        case 3 : {
//            KrmsU3=220000.0/(float)temp3;//��λ mV
//            KrmsI31=200000.0/(float)temp1;//��λuA
//            KrmsI32=200000.0/(float)temp2;//��λuA
//            datatemp[0]=KrmsI31;
//            datatemp[1]=KrmsI32;
//            datatemp[2]=KrmsU3;
//        }
//        break;
//        }

//        delay_ms(5000);
//        LED_ON();

////			while(touch_flag==0);
////			touch_flag=0;

//        delay_ms(5000);
//        LED_OFF();
//        for(ii=0; ii<20; ii++) //����ͨ��1/2��Чֵƫ��У����������0����ʱУ��
//        {
//            temp1+=READ_Att70xx(0x06,i);
//            delay_ms(20);
//            temp2+=READ_Att70xx(0x07,i);
//            delay_ms(20);
//            temp3+=READ_Att70xx(0x08,i);
//            delay_ms(20);
//        }
//        temp1/=20;
//        temp2/=20;
//        temp3/=20;
//        temp1*=temp1;
//        temp2*=temp2;
//        temp3*=temp3;
//        switch(i)
//        {
//        case 1: {
//            temp1-=((200*200)/(KrmsI11*KrmsI11));
//            temp2-=((200*200)/(KrmsI12*KrmsI12));
//            temp3-=((220*220)/(KrmsU1*KrmsU1));
//        }
//        break;
//        case 2: {
//            temp1-=((200*200)/(KrmsI21*KrmsI21));
//            temp2-=((200*200)/(KrmsI22*KrmsI22));
//            temp3-=((220*220)/(KrmsU2*KrmsU2));
//        }
//        break;
//        case 3: {
//            temp1-=((200*200)/(KrmsI31*KrmsI31));
//            temp2-=((200*200)/(KrmsI32*KrmsI32));
//            temp3-=((220*220)/(KrmsU3*KrmsU3));
//        }
//        break;
//        }

//        temp1/=0x8000;
//        temp2/=0x8000;
//        temp3/=0x8000;
//        //	temp1=0x00000050;
//        Write_Att70xx(0x69,temp1,i);
//        Write_Att70xx(0x6a,temp2,i);
//        Write_Att70xx(0x6b,temp3,i);

//        Write_Att70xx(w_WPREG,0x00,i);		//�ر�д�����Ĵ�������У������Ĵ���������Ч
//        delay_ms(100);
//        datatemp[3]=temp1;
//        datatemp[4]=temp2;
//        datatemp[5]=temp3;
//        stor_parameter(datatemp,i);  ///�������

}
//				temp1=0x00000010;
//		Write_Att70xx(0x69,temp1,2);
//	Write_Att70xx(0x6a,temp2,2);
//	Write_Att70xx(0x6b,temp3,2);
//
//	Write_Att70xx(0x69,temp1,3);
//	Write_Att70xx(0x6a,temp2,3);
//	Write_Att70xx(0x6b,temp3,3);

//}
void stor_parameter(u8 *data,u8 i)
{


    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    FLASH_ErasePage(FLASH_canshu+96*i& (~(FLASH_PAGE_SIZE -1 )));
//	Flash_Write_Without_check(FLASH_PRICE,&PricePlan[0][0],40);
    // Pull_ID[3] = ComAddress;
    Flash_Write_Without_check(FLASH_canshu+96*i,data,6);
    FLASH_Lock();
}





