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



#define	LED_SYS1_RUN		GPIO_Pin_15	//ucos����ָʾ��  D1
//#define	LED_SYS_COM		GPIO_Pin_1	//����ͨ��ָʾ��
//#define	LED_SYS_SPI1	GPIO_Pin_13	//SPI1 IC�������ָʾ��
//#define	LED_SYS_SPI2	GPIO_Pin_14	//SPI2 ID�������ָʾ��
#define	LED_SYS2_RC522	GPIO_Pin_15	//SPI2 ID�������ָʾ��  GPIOA15

#define	LED1C	GPIO_Pin_12	// ���ָʾ��1
#define	LED2C	GPIO_Pin_11	//���ָʾ��2


//GPIOb
#define	LOCK_EN			GPIO_Pin_3	//���ϵ�ʹ��
#define	LOCK_OP			GPIO_Pin_5	//����������	�����߶�����  L==���   H=����  ��Դ���϶�  OP  SON  SOFF ���˷���
//GPIOC
#define	LOCK_SON		GPIO_Pin_8	//�����ź� 1��Ч
#define	LOCK_SOFF		GPIO_Pin_7	//�����ź� 1��Ч
#define LOCK_Ssw		GPIO_Pin_6   //���ź���������Ϻ�12v�ź����
#define LOCK_CTL		GPIO_Pin_4   //�ⲿ�������źţ���12v���Ϳ�����

#define TM1640_IO		GPIOA
#define	TM1640_DIN		GPIO_Pin_7	//��ʾIC������  TM1640 pin7
#define	TM1640_CLK		GPIO_Pin_6	//��ʾIC������  TM1640 pin8


#define	WTH_IO	GPIOC
#define WTH_REST	GPIO_Pin_4		//WTH����IC�����ź�
#define WTH_DATA	GPIO_Pin_5		//WTH����IC�����ź�
#define RS485RW		GPIO_Pin_1 	//GPIOA	�͵�ƽ��������״̬ �ߵ�ƽ����

#define HT6809_IO	GPIOB
#define HT6809_MUTE	GPIO_Pin_2		//HT6809 �ľ�������  ������ʱ������
#define HT6809_UP	GPIO_Pin_1
#define HT6809_DW	GPIO_Pin_0

#define RS485IO		GPIOA

//#define TS433_EN	GPIO_Pin_3	//GPIOC	�͵�ƽ��������״̬ �ߵ�ƽ����
//#define TS433_SLP	GPIO_Pin_2	//GPIOC  �ߵ�ƽ����  �͵�ƽ100ms����
//#define TS433_RST	GPIO_Pin_1	//GPIOC  5ms�����帴λ

#define	PSAM_IO	GPIOA
#define	PSAM_CLK	GPIO_Pin_8
#define	PSAM_TR		GPIO_Pin_9
#define	PSAM_RST	GPIO_Pin_10		//PSAM���ĸ�λ�ź�

#define	PSAM2_IO GPIOB
#define	PSAM2_CLK	GPIO_Pin_12
#define	PSAM2_TR	GPIO_Pin_10
#define	PSAM2_RST	GPIO_Pin_11		//PSAM���ĸ�λ�ź�


#define RC522_IC	0x11111111		//�����л�������SPI��
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
//��ѹ�ɼ�
#define SampV		GPIO_Pin_0	//GPIOC  ��ѹ�ɼ�
typedef enum  {ENABLE_LOCK,CLOSE_LOCK,OPEN_LOCK,DISABLE_LOCK,NO_ACT,READPICC,WRITEPICC,EXISTPICC} CMD_Ctrl;




#endif
