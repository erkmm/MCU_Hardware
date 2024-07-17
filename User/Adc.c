#include <stdio.h>
#include "NUC029xAN.h"

//volatile uint32_t potvalue;
volatile uint32_t g_u32AdcIntFlag;
uint32_t  AdcValue;

void AdcInit() {
	ADC->ADCR = 0
			| ADC_ADCR_ADEN_Msk
			| ADC_ADCR_ADIE_Msk
			| ADC_ADCR_ADMD_SINGLE_CYCLE
			| ADC_ADCR_DIFFEN_SINGLE_END;


	ADC->ADCHER = (1 << 0) | (1 << 1);

	ADC->ADSR = ADC_ADSR_ADF_Msk;

	ADC->ADCR |= ADC_ADCR_ADIE_Msk;  // Enable ADC interrupt
	NVIC_EnableIRQ(ADC_IRQn);        // Enable ADC interrupt in NVIC
}

void AdcTask(){

}

uint32_t AdcGetChannelRaw(int channel) {
	AdcValue = 0;
	ADC->ADCR |= ADC_ADCR_ADST_Msk;

	while (g_u32AdcIntFlag == 0);

//    while(!((ADC->ADSR & ADC_ADSR_ADF_Msk) >> ADC_ADSR_ADF_Pos));

//	ADC->ADCR &= ~ADC_ADCR_ADIE_Msk;

	AdcValue = (ADC->ADDR[channel] & ADC_ADDR_RSLT_Msk) >> ADC_ADDR_RSLT_Pos;
//	printf("ADC: %d\n", AdcValue);
	return AdcValue;
}

void AdcGetChannelVoltage(int channel){
	float result = 5.0f * ((float) AdcGetChannelRaw(channel) / 4096.0f);
	printf("Result: %.4f V\n", result);
	printf("ADC%d: %d\n",channel, AdcGetChannelRaw(channel));
}

void AdcGetChannelTemp(int channel) {
	double R0 = 10000.0;
	double B = 3435.0;
	double K0 = 298.0;
	double res, felc, felb, fela;
	double result;

	float v1 = 4096.0 - AdcGetChannelRaw(channel);
	float v2 = v1 / AdcGetChannelRaw(channel);
	fela = log(v2);
	felb = fela / B;
	felc = 1 - K0 * felb;

	result = K0 / felc;
	float R = R0 / v2;

	printf("\n\n");
	printf("\n ADC     : %d", AdcGetChannelRaw(channel));
	printf("\n V2      : %.4f", (float) v2);
	printf("\n fela     : %.4f", (float) fela);
	printf("\n R       : %.4f", (float) R);
	printf("\n Result  : %.2f", (float) result);
	printf("\n RoomTemp: %.2f C", (float) result - 273);

}

void ADC_IRQHandler(void)
{
    g_u32AdcIntFlag = 1;
    ADC->ADSR = ADC_ADSR_ADF_Msk;      /* clear the A/D conversion flag */
}
