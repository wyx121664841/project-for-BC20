#ifndef _XPT2046_H_
#define _XPT2046_H_

#include "stm32f0xx.h"
//#include  "ili9328.h"
//#include "delay.h"
#define TOUCH_INT_PIN        GPIO_Pin_6
#define TOUCH_INT_PORT       GPIOB
#define TOUCH_INT_PIN_SCK    RCC_AHBPeriph_GPIOB

#define TOUCH_CS_PIN        GPIO_Pin_7
#define TOUCH_CS_PORT       GPIOB
#define TOUCH_CS_PIN_SCK    RCC_AHBPeriph_GPIOB

#define TOUCH_SCK_PIN        GPIO_Pin_13
#define TOUCH_SCK_PORT       GPIOB
#define TOUCH_SCK_PIN_SCK    RCC_AHBPeriph_GPIOB
#define TOUCH_SCK_SOURCE     GPIO_PinSource13
#define TOUCH_SCK_AF         GPIO_AF_0

#define TOUCH_MISO_PIN       GPIO_Pin_14
#define TOUCH_MISO_PORT      GPIOB
#define TOUCH_MISO_PIN_SCK   RCC_AHBPeriph_GPIOB
#define TOUCH_MISO_SOURCE    GPIO_PinSource14
#define TOUCH_MISO_AF        GPIO_AF_0

#define TOUCH_MOSI_PIN       GPIO_Pin_15
#define TOUCH_MOSI_PORT      GPIOB
#define TOUCH_MOSI_PIN_SCK   RCC_AHBPeriph_GPIOB
#define TOUCH_MOSI_SOURCE    GPIO_PinSource15
#define TOUCH_MOSI_AF        GPIO_AF_0


#define TOUCH_SPI2               RCC_APB1Periph_SPI2
#define SPI_TOUCH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define SPI_TOUCH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_7)

//#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
//#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define SPI_TOUHC_INT          GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6	)

#define CMD_RDY 0X90  //0B10010000即用差分方式读X坐标
#define CMD_RDX	0XD0  //0B11010000即用差分方式读Y坐标  
#define Pen_Down 1
#define Pen_Up   0

//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06
#define W25X_WriteDisable		      0x04
#define W25X_ReadStatusReg		    0x05
#define W25X_WriteStatusReg		    0x01
#define W25X_ReadData			        0x03
#define W25X_FastReadData		      0x0B
#define W25X_FastReadDual		      0x3B
#define W25X_PageProgram		      0x02
#define W25X_BlockErase			      0xD8
#define W25X_SectorErase		      0x20
#define W25X_ChipErase			      0xC7
#define W25X_PowerDown			      0xB9
#define W25X_ReleasePowerDown	    0xAB
#define W25X_DeviceID			        0xAB
#define W25X_ManufactDeviceID   	0x90
#define W25X_JedecDeviceID		    0x9F

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF


typedef struct
{


    uint16_t X_Coord;//LCD坐标
    uint16_t Y_Coord;
    uint16_t X_ADC; //ADC值
    uint16_t Y_ADC;
    uint8_t  Pen_Sign;//笔的状态
} Pen_Holder;
//void delay_us(uint32_t cnt);
extern Pen_Holder Pen_Point;
extern void Touch_Init(void);
// extern void PIOINT2_IRQHandler(void);
extern uint16_t ADS_Read_AD(uint8_t CMD);
extern uint8_t Read_ADS(uint16_t*x,uint16_t *y);
extern uint8_t Read_ADS2(uint16_t *x,uint16_t *y);
extern uint8_t Read_Once(void);
extern uint8_t Read_Continue(void);
extern void Change_XY(void);
void WR_Cmd(uint8_t cmd);
uint16_t RE_Cmd(void);

void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
uint8_t SPI_FLASH_ReadByte(void);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_SendByte(uint8_t byte);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_Flash_PowerDown(void)  ;
void SPI_Flash_WAKEUP(void)  ;
uint32_t SPI_FLASH_ReadID(void);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
uint8_t SPI_FLASH_Read8Byte(void);






#endif





















