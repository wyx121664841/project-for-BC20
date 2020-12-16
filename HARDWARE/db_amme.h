#ifndef _SPI_H_
#define _SPI_H_

#include "stdint.h"
#include "stdio.h"
//#include "SPI.h"



//#define SPI_CS1_HIGH() 	 GPIO_SetBits(GPIOA, GPIO_Pin_8)
//#define SPI_CS2_HIGH() 	 GPIO_SetBits(GPIOA, GPIO_Pin_11)
//#define SPI_CS3_HIGH() 	 GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define SPI_DI_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define SPI_DO_HIGH()    GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define SPI_CLK_HIGH()   GPIO_SetBits(GPIOA, GPIO_Pin_5)

//#define SPI_CS1_LOW()  	GPIO_ResetBits(GPIOA, GPIO_Pin_8)
//#define SPI_CS2_LOW()  	GPIO_ResetBits(GPIOA, GPIO_Pin_11)
//#define SPI_CS3_LOW()  	GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define SPI_DI_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define SPI_DO_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define SPI_CLK_LOW()   GPIO_ResetBits(GPIOA, GPIO_Pin_5)   //��ӦATT7022�ϵĹܽ�

#define RDSPIData   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//������

#define ATT_G  1.163
#define INATTV 	72			      // ģ������8��         
#define INATTI 	5			        //��������Ӧ��ѹ ����1000��  
#define EC     3200 					//������
#define UI	   220 					  //���ѹ
#define VI	   0.0478
#define UI_K   10.38  // 


#define Exterl_meter_start_number 2
//#define  HFCONST ((2.592*ATT_G*ATT_G*10000*INATTV*INATTI)/(EC*UI*VI))  =76
//#define P_K   1.829E-4//1000000*UI*VI/(ATT_G*ATT_G*INATTV*INATTI*8388608)	//	  2^23 = 8388608


extern u32 CurrentElectric;				//�ܵ���

extern u8 SetGainData;					//������������
//����оƬ�Ĵ�������(Read Only)--------------------------------------------------
#define	r_SPlI1			0x00					//����ͨ��1��ADC��������
#define	r_SPlI2			0x01					//����ͨ��2��ADC��������
#define	r_SPlU			0x02					//��ѹͨ����ADC��������
//#define	r_SPlP			0x03					//�й����ʲ�������
//#define	r_SPlQ			0x04					//�޹����ʲ�������
//#define	r_SPlS			0x05					//���ڹ��ʲ�������
#define	r_RmsI1			0x06					//����ͨ��1����Чֵ
#define	r_RmsI2			0x07					//����ͨ��2����Чֵ
#define	r_RmsU			0x08					//��ѹͨ������Чֵ
#define	r_FreqU			0x09					//��ѹƵ��
#define	r_PowerP1		0x0A					//��һͨ���й�����
#define	r_PowerQ1		0x0B					//��һͨ���޹�����
#define	r_PowerS		0x0C					//���ڹ���
#define	r_EnergyP0		0x0D					//�й�����
#define	r_EnergyQ0		0x0E					//�޹�����
#define	r_EnergyS0		0x0F					//��������
#define	r_PowerP2		0x10					//�ڶ�ͨ���й�����
#define	r_PowerQ2		0x11					//�ڶ�ͨ���޹�����
#define	r_MaxWave		0x12					//��ѹ���η�ֵ�Ĵ���
#define	r_BackupData	0x16					//ͨѶ���ݱ��ݼĴ���
#define	r_ComChkSum		0x17					//ͨѶУ��ͼĴ���
#define	r_SumChkSum		0x18					//У�����У��ͼĴ���
#define	r_EMUSR			0x19					//EMU״̬�Ĵ���
#define	r_SYSSTA		0x1A					//ϵͳ״̬�Ĵ���
#define	r_ChipID		0x1B					//оƬID,Ĭ��ֵATT7053B0
#define	r_DeviceID		0x1C					//����ID,Ĭ��ֵATT705301

//����оƬ�Ĵ�������(Read/Write)--------------------------------------------------
#define	w_EMUIE			0x30					//EMU�ж�ʹ�ܼĴ���
#define	w_EMUIF			0x31					//EMU�жϱ�־�Ĵ���
#define	w_WPREG			0x32					//д�����Ĵ���
#define	w_SRSTREG		0x33					//�����λ�Ĵ���

#define	w_EMUCFG		0x40					//EMU���üĴ���
#define	w_FreqCFG		0x41					//ʱ��/����Ƶ�����üĴ���
#define	w_ModuleEn		0x42					//EMUģ��ʹ�ܼĴ���
#define	w_ANAEN			0x43					//ģ��ģ��ʹ�ܼĴ���(ADC���ؼĴ���)
//#define	w_STATUSCFG		0x44					//STATUS������üĴ���
#define	w_IOCFG			0x45					//IO������üĴ���

#define	w_GP1			0x50					//ͨ��1���й�����У��
#define	w_GQ1			0x51					//ͨ��1���޹�����У��
#define	w_GS1			0x52					//ͨ��1�����ڹ���У��
//#define	w_Phase1		0x53					//ͨ��1����λУ�����Ʋ����㷽ʽ��
#define	w_GP2			0x54					//ͨ��2���й�����У��
#define	w_GQ2			0x55					//ͨ��2���޹�����У��
#define	w_GS2			0x56					//ͨ��2�����ڹ���У��
//#define	w_Phase2		0x57					//ͨ��2����λУ�����Ʋ����㷽ʽ��
#define	w_QPhsCal		0x58					//�޹���λ����
#define	w_ADCCON		0x59					//ADCͨ������ѡ��
//#define	w_AllGain		0x5A					//3��ADCͨ����������Ĵ���
#define	w_I2Gain		0x5B					//����ͨ��2���油��
#define	w_I1Off			0x5C					//����ͨ��1��ƫ��У��
#define	w_I2Off			0x5D					//����ͨ��2��ƫ��У��
#define	w_UOff			0x5E					//��ѹͨ����ƫ��У��
#define	w_PQStart		0x5F					//������������
//#define	w_RMSStart		0x60					//��Чֵ����ֵ����
#define	w_HFConst		0x61					//�������Ƶ������
#define	w_CHK			0x62					//�Ե���ֵ����
#define	w_IPTEMP		0x63					//�Ե�����ֵ����
//#define	w_UConst		0x64					//��ѹ��Чֵ���ֵ����
#define	w_P1Offset		0x65					//ͨ��1�й�����ƫ��У������
#define	w_P2Offset		0x66					//ͨ��2�й�����ƫ��У������
#define	w_Q1Offset		0x67					//ͨ��1�޹�����ƫ��У������
#define	w_Q2Offset		0x68					//ͨ��2�޹�����ƫ��У������
#define	w_I1RMSOff		0x69					//����ͨ��1��Чֵ�����Ĵ���
#define	w_I2RMSOff		0x6A					//����ͨ��2��Чֵ�����Ĵ���
//#define	w_URMSOff		0x6B					//��ѹͨ����Чֵ�����Ĵ���
#define	w_ZCrossCur		0x6C					//����������ֵ���üĴ���
#define	w_GPhs1			0x6D					//ͨ��1����λУ����PQ��ʽ��
#define	w_GPhs2			0x6E					//ͨ��2����λУ����PQ��ʽ��
#define	w_PFCnt			0x6F					//�����й��������
#define	w_QFCnt			0x70					//�����޹��������
#define	w_SFCnt			0x71					//���������������

#define	w_ANACON		0x72					//ģ����ƼĴ���
#define	w_SumChkL		0x73					//У��͵�16λ
#define	w_SumChkH		0x74					//У��͸�8λ
#define	w_MODECFG		0x75					//ģʽ���üĴ���
#define	w_P1OffsetL		0x76					//ͨ��1�й�����ƫ��У����8λ
#define	w_P2OffsetL		0x77					//ͨ��2�й�����ƫ��У����8λ
#define	w_Q1OffsetL		0x78					//ͨ��1�޹�����ƫ��У����8λ
#define	w_Q2OffsetL		0x79					//ͨ��2�޹�����ƫ��У����8λ
#define	w_UPeakLvl		0x7A					//UPEAK��ֵ�Ĵ���
#define	w_USagLvl		0x7B					//USAG��ֵ�Ĵ���
#define	w_UCycLen		0x7C					//PEAK&SAG����������üĴ���
typedef struct
{
    u16 I_Curr;
    u16 Voltage;
    u16 Active_Power;
    u16  Reactive_Power;
    u16  Apparent_Power;
    u8 	Factor;
    unsigned long int Apparent_Energy ; 		// dian liang
} MeterData;


extern u16 Huganqibeilv;                             //����������


void SPI_GPIO_Config(void);
u32 READ_Att70xx(u8 Address);
void Write_Att70xx(u8 address, u32 write_data);
void EMU_init(void);
void InitAmmeter(void);
void ReadAmmeterData(u8 x);
void ATT7053_offset(u8 i2);//ATT7053У׼����
//void CSx_set(u8 x);
//void CSx_clr(u8 x);
void stor_parameter(u8 *data,u8 i);
#endif
