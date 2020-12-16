#include "stm32f0xx.h"
#include "stm32f0xx_flash.h"

//#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
// #define FLASH_PAGE_SIZE    ((uint16_t)0x800)

//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
//u8 Pull_ID[4] = {0};	//����ID ���ص�ַ   ͨ�ŵ�ַΪUID�����λ
//u8 ComAddress = 0xff;	//ͨ�ŵ�ַ
//#else
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)

//#define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* FLASH Page Size */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08006000)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ((uint32_t)0x08007000)   /* End @ of user Flash area */
#define DATA_32   ((uint32_t)0x12345678)
//uint32_t EraseCounter = 0x00, Address = 0x00;
//uint32_t Data = 0x3210ABCD;
//uint32_t NbrOfPage = 0x00;
//__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
//__IO TestStatus MemoryProgramStatus = PASSED;
//#endif


//#define FLASH_START_ADDR  ((u32)(0x08000000+1024*28))
//#define FLASH_END_ADDR    ((u32)(0x08000000+1024*120))


#define FLASH_canshu ((u32)(0x08000000+1024*63))     //��������
#define FLASH_PULL_ID ((u32)(0x08000000+1024*62))     //���
#define FlASH_APP_VER ((u32)(0x08000000+1024*61))   //�汾��
//#define FlASH_temper ((u32)(0x08000000+1024*246))   //�¶�̽ͷID


//#define FLASH_canshu ((u32)(0x08000000+1024*60))     //У׼����
//#define FLASH_yuzhi ((u32)(0x08000000+1024*62))     //��ֵ����

////#define FLASH_PRICE 	((u32)(0x08000000+1024*80))		//����־
//#define FLASH_PULL_ID ((u32)(0x08000000+1024*58))     //���
//#define FlASH_APP_VER ((u32)(0x08000000+1024*56))   //�汾��



//#define FLASH_PRICE 	((u32)(0x08000000+1024*115+16))

//#define FLASH_RECODE_CACHE ((u32)(0x08000000+1024*116))
//#define FLASH_RECODE	((u32)(0x08000000+1024*116+16))



uint16_t Flash_Write_Without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite);
int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) ;
int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite);

