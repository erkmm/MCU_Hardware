#include <stdio.h>
#include "NUC029xAN.h"
#include <stdint.h>

//volatile uint32_t potvalue;
volatile uint32_t g_u32AdcIntFlag;

//void PotValue(void)
//{
//    if(ADC->ADSR & ADC_ADSR_ADF_Msk)
//    {
//        ADC->ADSR = ADC_ADSR_ADF_Msk;  // Clear the ADC conversion flag
//
//        while(!(ADC->ADSR & (ADC_ADSR_ADF_Msk)));
//
//      potvalue = ((ADC->ADDR[0] & ADC_ADDR_RSLT_Msk)>>ADC_ADDR_RSLT_Pos) ;
//      float  v1 = potvalue/4096;
//      float  result=5*v1;
//
//      printf("Result: %.4f",result);
//    }
//}

void GetPotVal(){
	uint32_t  AdcValue;
   //printf("|                      ADC single mode sample code                     |\n");

    ADC->ADCR = 0
    			|ADC_ADCR_ADEN_Msk
    			|ADC_ADCR_ADIE_Msk
    			|(0 << ADC_ADCR_ADMD_Pos)
				|(0 << ADC_ADCR_DIFFEN_Pos);

    ADC->ADCHER |= ((ADC->ADCHER & ~ADC_ADCHER_CHEN_Msk) | (1 << 0));

    ADC->ADSR = ADC_ADSR_ADF_Msk;

    ADC->ADCR |= ADC_ADCR_ADIE_Msk;  // Enable ADC interrupt
	NVIC_EnableIRQ(ADC_IRQn);        // Enable ADC interrupt in NVIC

	AdcValue = 0;
	ADC->ADCR |= ADC_ADCR_ADST_Msk;

	while(g_u32AdcIntFlag == 0);

    ADC->ADCR &= ~ADC_ADCR_ADIE_Msk;

    AdcValue = (ADC->ADDR[0] & ADC_ADDR_RSLT_Msk) >> ADC_ADDR_RSLT_Pos;
    printf("ADC: %d\n",AdcValue);

    float result = 5.0f * ((float)AdcValue / 4096.0f);
    printf("Result: %.4f V\n", result);
}


void ADC_IRQHandler(void)
{
    g_u32AdcIntFlag = 1;
    ADC->ADSR = ADC_ADSR_ADF_Msk;      /* clear the A/D conversion flag */
}
