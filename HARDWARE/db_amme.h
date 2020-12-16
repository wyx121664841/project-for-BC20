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
#define SPI_CLK_LOW()   GPIO_ResetBits(GPIOA, GPIO_Pin_5)   //对应ATT7022上的管脚

#define RDSPIData   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//读数据

#define ATT_G  1.163
#define INATTV 	72			      // 模拟扩大8倍         
#define INATTI 	5			        //互感器感应电压 扩大1000倍  
#define EC     3200 					//电表表常数
#define UI	   220 					  //额定电压
#define VI	   0.0478
#define UI_K   10.38  // 


#define Exterl_meter_start_number 2
//#define  HFCONST ((2.592*ATT_G*ATT_G*10000*INATTV*INATTI)/(EC*UI*VI))  =76
//#define P_K   1.829E-4//1000000*UI*VI/(ATT_G*ATT_G*INATTV*INATTI*8388608)	//	  2^23 = 8388608


extern u32 CurrentElectric;				//总电量

extern u8 SetGainData;					//设置增益数据
//计量芯片寄存器定义(Read Only)--------------------------------------------------
#define	r_SPlI1			0x00					//电流通道1的ADC采样数据
#define	r_SPlI2			0x01					//电流通道2的ADC采样数据
#define	r_SPlU			0x02					//电压通道的ADC采样数据
//#define	r_SPlP			0x03					//有功功率波形数据
//#define	r_SPlQ			0x04					//无功功率波形数据
//#define	r_SPlS			0x05					//视在功率波形数据
#define	r_RmsI1			0x06					//电流通道1的有效值
#define	r_RmsI2			0x07					//电流通道2的有效值
#define	r_RmsU			0x08					//电压通道的有效值
#define	r_FreqU			0x09					//电压频率
#define	r_PowerP1		0x0A					//第一通道有功功率
#define	r_PowerQ1		0x0B					//第一通道无功功率
#define	r_PowerS		0x0C					//视在功率
#define	r_EnergyP0		0x0D					//有功能量
#define	r_EnergyQ0		0x0E					//无功能量
#define	r_EnergyS0		0x0F					//视在能量
#define	r_PowerP2		0x10					//第二通道有功功率
#define	r_PowerQ2		0x11					//第二通道无功功率
#define	r_MaxWave		0x12					//电压波形峰值寄存器
#define	r_BackupData	0x16					//通讯数据备份寄存器
#define	r_ComChkSum		0x17					//通讯校验和寄存器
#define	r_SumChkSum		0x18					//校表参数校验和寄存器
#define	r_EMUSR			0x19					//EMU状态寄存器
#define	r_SYSSTA		0x1A					//系统状态寄存器
#define	r_ChipID		0x1B					//芯片ID,默认值ATT7053B0
#define	r_DeviceID		0x1C					//器件ID,默认值ATT705301

//计量芯片寄存器定义(Read/Write)--------------------------------------------------
#define	w_EMUIE			0x30					//EMU中断使能寄存器
#define	w_EMUIF			0x31					//EMU中断标志寄存器
#define	w_WPREG			0x32					//写保护寄存器
#define	w_SRSTREG		0x33					//软件复位寄存器

#define	w_EMUCFG		0x40					//EMU配置寄存器
#define	w_FreqCFG		0x41					//时钟/更新频率配置寄存器
#define	w_ModuleEn		0x42					//EMU模块使能寄存器
#define	w_ANAEN			0x43					//模拟模块使能寄存器(ADC开关寄存器)
//#define	w_STATUSCFG		0x44					//STATUS输出配置寄存器
#define	w_IOCFG			0x45					//IO输出配置寄存器

#define	w_GP1			0x50					//通道1的有功功率校正
#define	w_GQ1			0x51					//通道1的无功功率校正
#define	w_GS1			0x52					//通道1的视在功率校正
//#define	w_Phase1		0x53					//通道1的相位校正（移采样点方式）
#define	w_GP2			0x54					//通道2的有功功率校正
#define	w_GQ2			0x55					//通道2的无功功率校正
#define	w_GS2			0x56					//通道2的视在功率校正
//#define	w_Phase2		0x57					//通道2的相位校正（移采样点方式）
#define	w_QPhsCal		0x58					//无功相位补偿
#define	w_ADCCON		0x59					//ADC通道增益选择
//#define	w_AllGain		0x5A					//3个ADC通道整体增益寄存器
#define	w_I2Gain		0x5B					//电流通道2增益补偿
#define	w_I1Off			0x5C					//电流通道1的偏置校正
#define	w_I2Off			0x5D					//电流通道2的偏置校正
#define	w_UOff			0x5E					//电压通道的偏置校正
#define	w_PQStart		0x5F					//启动功率设置
//#define	w_RMSStart		0x60					//有效值启动值设置
#define	w_HFConst		0x61					//输出脉冲频率设置
#define	w_CHK			0x62					//窃电阈值设置
#define	w_IPTEMP		0x63					//窃电检测阈值设置
//#define	w_UConst		0x64					//电压有效值替代值设置
#define	w_P1Offset		0x65					//通道1有功功率偏置校正参数
#define	w_P2Offset		0x66					//通道2有功功率偏置校正参数
#define	w_Q1Offset		0x67					//通道1无功功率偏置校正参数
#define	w_Q2Offset		0x68					//通道2无功功率偏置校正参数
#define	w_I1RMSOff		0x69					//电流通道1有效值补偿寄存器
#define	w_I2RMSOff		0x6A					//电流通道2有效值补偿寄存器
//#define	w_URMSOff		0x6B					//电压通道有效值补偿寄存器
#define	w_ZCrossCur		0x6C					//电流过零阈值设置寄存器
#define	w_GPhs1			0x6D					//通道1的相位校正（PQ方式）
#define	w_GPhs2			0x6E					//通道2的相位校正（PQ方式）
#define	w_PFCnt			0x6F					//快速有功脉冲计数
#define	w_QFCnt			0x70					//快速无功脉冲计数
#define	w_SFCnt			0x71					//快速视在脉冲计数

#define	w_ANACON		0x72					//模拟控制寄存器
#define	w_SumChkL		0x73					//校验和低16位
#define	w_SumChkH		0x74					//校验和高8位
#define	w_MODECFG		0x75					//模式配置寄存器
#define	w_P1OffsetL		0x76					//通道1有功功率偏置校正低8位
#define	w_P2OffsetL		0x77					//通道2有功功率偏置校正低8位
#define	w_Q1OffsetL		0x78					//通道1无功功率偏置校正低8位
#define	w_Q2OffsetL		0x79					//通道2无功功率偏置校正低8位
#define	w_UPeakLvl		0x7A					//UPEAK阈值寄存器
#define	w_USagLvl		0x7B					//USAG阈值寄存器
#define	w_UCycLen		0x7C					//PEAK&SAG检测周期设置寄存器
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


extern u16 Huganqibeilv;                             //互感器倍率


void SPI_GPIO_Config(void);
u32 READ_Att70xx(u8 Address);
void Write_Att70xx(u8 address, u32 write_data);
void EMU_init(void);
void InitAmmeter(void);
void ReadAmmeterData(u8 x);
void ATT7053_offset(u8 i2);//ATT7053校准程序
//void CSx_set(u8 x);
//void CSx_clr(u8 x);
void stor_parameter(u8 *data,u8 i);
#endif
