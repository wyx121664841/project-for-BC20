#ifndef __IODEF_H
#define __IODEF_H

//#define TESTIO GPIOB
//#define RCC_IO_TEST RCC_APB2Periph_GPIOB
//#define TESTIO0 GPIO_Pin_0
//#define TESTIO1 GPIO_Pin_1
//#define TESTIO1 GPIO_Pin_1
//#define TESTIO2 GPIO_Pin_2
//#define TESTIO3 GPIO_Pin_3
//#define TESTIO4 GPIO_Pin_4



#define	LED_SYS1_RUN		GPIO_Pin_15	//ucos运行指示灯  D1
//#define	LED_SYS_COM		GPIO_Pin_1	//串口通信指示灯
//#define	LED_SYS_SPI1	GPIO_Pin_13	//SPI1 IC读卡器活动指示灯
//#define	LED_SYS_SPI2	GPIO_Pin_14	//SPI2 ID读卡器活动指示灯
#define	LED_SYS2_RC522	GPIO_Pin_15	//SPI2 ID读卡器活动指示灯  GPIOA15

#define	LED1C	GPIO_Pin_12	// 输出指示灯1
#define	LED2C	GPIO_Pin_11	//输出指示灯2


//GPIOb
#define	LOCK_EN			GPIO_Pin_3	//锁上电使能
#define	LOCK_OP			GPIO_Pin_5	//锁动作控制	锁接线端子上  L==伸出   H=缩进  电源板上对  OP  SON  SOFF 做了反向
//GPIOC
#define	LOCK_SON		GPIO_Pin_8	//锁开信号 1有效
#define	LOCK_SOFF		GPIO_Pin_7	//锁闭信号 1有效
#define LOCK_Ssw		GPIO_Pin_6   //锁信号输出，锁上后12v信号输出
#define LOCK_CTL		GPIO_Pin_4   //外部锁控制信号，有12v，就开锁。

#define TM1640_IO		GPIOA
#define	TM1640_DIN		GPIO_Pin_7	//显示IC的驱动  TM1640 pin7
#define	TM1640_CLK		GPIO_Pin_6	//显示IC的驱动  TM1640 pin8


#define	WTH_IO	GPIOC
#define WTH_REST	GPIO_Pin_4		//WTH语音IC驱动信号
#define WTH_DATA	GPIO_Pin_5		//WTH语音IC驱动信号
#define RS485RW		GPIO_Pin_1 	//GPIOA	低电平进入设置状态 高电平工作

#define HT6809_IO	GPIOB
#define HT6809_MUTE	GPIO_Pin_2		//HT6809 的静音控制  开机的时候消音
#define HT6809_UP	GPIO_Pin_1
#define HT6809_DW	GPIO_Pin_0

#define RS485IO		GPIOA

//#define TS433_EN	GPIO_Pin_3	//GPIOC	低电平进入设置状态 高电平工作
//#define TS433_SLP	GPIO_Pin_2	//GPIOC  高电平休眠  低电平100ms后工作
//#define TS433_RST	GPIO_Pin_1	//GPIOC  5ms低脉冲复位

#define	PSAM_IO	GPIOA
#define	PSAM_CLK	GPIO_Pin_8
#define	PSAM_TR		GPIO_Pin_9
#define	PSAM_RST	GPIO_Pin_10		//PSAM卡的复位信号

#define	PSAM2_IO GPIOB
#define	PSAM2_CLK	GPIO_Pin_12
#define	PSAM2_TR	GPIO_Pin_10
#define	PSAM2_RST	GPIO_Pin_11		//PSAM卡的复位信号


#define RC522_IC	0x11111111		//用于切换操作的SPI口
#define RC522_ID	0x22222222

#define SPI1_IO		GPIOA
#define	SPI1_NSS	GPIO_Pin_8
#define	SPI1_SCK	GPIO_Pin_5
#define	SPI1_MISO	GPIO_Pin_6
#define	SPI1_MOSI	GPIO_Pin_7
#define SPI7053RST	GPIO_Pin_8

//#define SPI3_IO		GPIOB
//#define	SPI3_NSS	GPIO_Pin_6		//GPIOB
//#define	SPI3_SCK	GPIO_Pin_3
//#define	SPI3_MISO	GPIO_Pin_4
//#define	SPI3_MOSI	GPIO_Pin_5


#define	SPI2_IO		GPIOB
//#define	RC522_NSS	GPIO_Pin_6		//GPIOC
#define	RFIC_NSS		GPIO_Pin_7		//GPIOC
//#define	Flash_CS		GPIO_Pin_12		//GPIOB
#define	RC522_RST	GPIO_Pin_11		//GPIOB
#define	SPI2_SCK	GPIO_Pin_13
#define	SPI2_MISO	GPIO_Pin_14
#define	SPI2_MOSI	GPIO_Pin_15

//#define	I2C2_IO	GPIOB
//#define I2C2_SCL GPIO_Pin_10
//#define I2C2_SDA GPIO_Pin_11
//电压采集
#define SampV		GPIO_Pin_0	//GPIOC  电压采集
typedef enum  {ENABLE_LOCK,CLOSE_LOCK,OPEN_LOCK,DISABLE_LOCK,NO_ACT,READPICC,WRITEPICC,EXISTPICC} CMD_Ctrl;




#endif
