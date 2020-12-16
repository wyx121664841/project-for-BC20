#include "Flash_Mem.h"
#include "stm32f0xx_flash.h"

uint16_t Flash_Write_Without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;

    // FLASH_UnlockBank1();
    while((i < iNumByteToWrite) && (FLASHStatus == FLASH_COMPLETE))
    {
        FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(uint16_t*)buf);
        i = i+2;
        iAddress = iAddress + 2;
        buf = buf + 2;
    }

    return iNumByteToWrite;
}
/**
* @brief Programs a half word at a specified Option Byte Data address.
* @note This function can be used for all STM32F10x devices.
* @param Address: specifies the address to be programmed.
* @param buf: specifies the data to be programmed.
* @param iNbrToWrite: the number to write into flash
* @retval if success return the number to write, -1 if error
*
*/
uint8_t flash_tmp[FLASH_PAGE_SIZE];

int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i = 0;

    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    FLASH_Unlock();//FLASH_UnlockBank1();
    secpos=iAddress & (~(FLASH_PAGE_SIZE -1 )) ;//������ַ
    secoff=iAddress & (FLASH_PAGE_SIZE -1); //�������ڵ�ƫ��
    secremain=FLASH_PAGE_SIZE-secoff; //����ʣ��ռ��С
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

    if(iNumByteToWrite<=secremain) secremain = iNumByteToWrite;//������4096���ֽ�

    while( 1 )
    {
        Flash_Read(secpos, flash_tmp, FLASH_PAGE_SIZE); //������������
        for(i=0; i<secremain; i++)
        {   //У������
            if(flash_tmp[secoff+i]!=0XFF)break; //��Ҫ����
        }
        if(i<secremain)
        {   //��Ҫ����
            FLASHStatus = FLASH_ErasePage(secpos); //�����������
            if(FLASHStatus != FLASH_COMPLETE)
                return -1;
            for(i=0; i<secremain; i++) //����
            {
                flash_tmp[i+secoff]=buf[i];
            }
            Flash_Write_Without_check(secpos,flash_tmp,FLASH_PAGE_SIZE);  //д����������
        }
        else
        {
            Flash_Write_Without_check(iAddress,buf,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
        }

        if(iNumByteToWrite==secremain) //д�������
        {
            break;
        }
        else
        {
            secpos += FLASH_PAGE_SIZE;
            secoff = 0;//ƫ��λ��Ϊ0
            buf += secremain; //ָ��ƫ��
            iAddress += secremain;//д��ַƫ��
            iNumByteToWrite -= secremain; //�ֽ����ݼ�
            if(iNumByteToWrite>FLASH_PAGE_SIZE) secremain=FLASH_PAGE_SIZE;//��һ����������д����
            else secremain = iNumByteToWrite; //��һ����������д����
        }
    }
    FLASH_Lock();//FLASH_LockBank1();
    return iNbrToWrite;
}






/**
* @brief Programs a half word at a specified Option Byte Data address.
* @note This function can be used for all STM32F10x devices.
* @param Address: specifies the address to be programmed.
* @param buf: specifies the data to be programmed.
* @param iNbrToWrite: the number to read from flash
* @retval if success return the number to write, without error
*
*/
int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead)
{
    int i = 0;
    while(i < iNbrToRead)
    {
        *(buf + i) = *(__IO uint8_t*) iAddress++;
        i++;
    }
    return i;
}


