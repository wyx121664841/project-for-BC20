
#include "adc.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_adc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address                0x40012440
__IO uint16_t RegularConvData_Tab;
void ADC1_DMA_Init(void)
{
    ADC_InitTypeDef     ADC_InitStruct;
    GPIO_InitTypeDef    GPIO_InitStruct;
    /* Enable  GPIOA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* ADC1 Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* Configure PA.01  as analog input */
//    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
//    GPIO_Init(GPIOA, &GPIO_InitStruct);				// 
    /* ADC1 DeInit */
    ADC_DeInit(ADC1);
    ADC_StructInit(&ADC_InitStruct);
    /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStruct.ADC_ScanDirection = ADC_ScanDirection_Backward;
    ADC_Init(ADC1, &ADC_InitStruct);
    /* Convert the ADC1 temperature sensor  with 55.5 Cycles as sampling time */
    ADC_ChannelConfig(ADC1, ADC_Channel_6, ADC_SampleTime_55_5Cycles);
//    ADC_TempSensorCmd(ENABLE);

    /* Convert the ADC1 Vref  with 55.5 Cycles as sampling time */
//    ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint, ADC_SampleTime_55_5Cycles);
//    ADC_VrefintCmd(ENABLE);
    /* ADC Calibration */
    ADC_GetCalibrationFactor(ADC1);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
    /* Wait the ADCEN falg */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));
    /* ADC1 regular Software Start Conv */
    ADC_StartOfConversion(ADC1);
}

