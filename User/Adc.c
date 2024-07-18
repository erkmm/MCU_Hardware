#include <stdio.h>
#include "NUC029xAN.h"
#include <stdint.h>

volatile uint32_t g_u32AdcIntFlag;
uint32_t  AdcValue;

typedef struct {
  float x;
  float y;
} dsp_lookup_f_t;

float dsp_lookup_f(const dsp_lookup_f_t * table, float x, int size){
  int i;
  float m;
  i = 0;
  while((i < (size)) && (x > table[i].x) ){  //find the two points in the table to use
    i++;
  }
  if ( i == size ){   //make sure the point isn't past the end of the table
    return table[i-1].y;
  }

  if ( i == 0 ){  //make sure the point isn't before the beginning of the table
    return table[i].y;
  }

  m = (table[i].y - table[i-1].y) / ( table[i].x - table[i-1].x); //calculate the slope
  return m * (x - table[i].x) + table[i].y; //this is the solution to the point slope formula
}

//#define TABLE_SIZE 73
//dsp_lookup_f_t my_table[TABLE_SIZE] = {
//{ .x=  72911 ,  .y= -15.00 },
//{ .x=  68518 ,  .y= -13.89 },
//{ .x=  64419 ,  .y= -12.78 },
//{ .x=  60592 ,  .y= -11.67 },
//{ .x=  57017 ,  .y= -10.56 },
//{ .x=  53647 ,  .y= -9.44  },
//{ .x=  50526 ,  .y= -8.33  },
//{ .x=  47606 ,  .y= -7.22  },
//{ .x=  44874 ,  .y= -6.11  },
//{ .x=  42317 ,  .y= -5.00  },
//{ .x=  39921 ,  .y= -3.89  },
//{ .x=  37676 ,  .y= -2.78  },
//{ .x=  35573 ,  .y= -1.67  },
//{ .x=  33599 ,  .y= -0.56  },
//{ .x=  31732 ,  .y=  0.56  },
//{ .x=  29996 ,  .y=  1.67  },
//{ .x=  28365 ,  .y=  2.78  },
//{ .x=  26834 ,  .y=  3.89  },
//{ .x=  25395 ,  .y=  5.00  },
//{ .x=  24042 ,  .y=  6.11  },
//{ .x=  22770 ,  .y=  7.22  },
//{ .x=  21573 ,  .y=  8.33  },
//{ .x=  20446 ,  .y=  9.44  },
//{ .x=  19376 ,  .y= 10.56  },
//{ .x=  18378 ,  .y= 11.67  },
//{ .x=  17437 ,  .y= 12.78  },
//{ .x=  16550 ,  .y= 13.89  },
//{ .x=  15714 ,  .y= 15.00  },
//{ .x=  14925 ,  .y= 16.11  },
//{ .x=  14180 ,  .y= 17.22  },
//{ .x=  13478 ,  .y= 18.33  },
//{ .x=  12814 ,  .y= 19.44  },
//{ .x=  12182 ,  .y= 20.56  },
//{ .x=  11590 ,  .y= 21.67  },
//{ .x=  11030 ,  .y= 22.78  },
//{ .x=  10501 ,  .y= 23.89  },
//{ .x=  10000 ,  .y= 25.00  },
//{ .x=  9526  ,  .y= 26.11  },
//{ .x=  9078  ,  .y= 27.22  },
//{ .x=  8653  ,  .y= 28.33  },
//{ .x=  8251  ,  .y= 29.44  },
//{ .x=  7866  ,  .y= 30.56  },
//{ .x=  7505  ,  .y= 31.67  },
//{ .x=  7163  ,  .y= 32.78  },
//{ .x=  6838  ,  .y= 33.89  },
//{ .x=  6530  ,  .y= 35.00  },
//{ .x=  6238  ,  .y= 36.11  },
//{ .x=  5960  ,  .y= 37.22  },
//{ .x=  5697  ,  .y= 38.33  },
//{ .x=  5447  ,  .y= 39.44  },
//{ .x=  5207  ,  .y= 40.56  },
//{ .x=  4981  ,  .y= 41.67  },
//{ .x=  4766  ,  .y= 42.78  },
//{ .x=  4561  ,  .y= 43.89  },
//{ .x=  4367  ,  .y= 45.00  },
//{ .x=  4182  ,  .y= 46.11  },
//{ .x=  4006  ,  .y= 47.22  },
//{ .x=  3838  ,  .y= 48.33  },
//{ .x=  3679  ,  .y= 49.44  },
//{ .x=  3525  ,  .y= 50.56  },
//{ .x=  3380  ,  .y= 51.67  },
//{ .x=  3242  ,  .y= 52.78  },
//{ .x=  3111  ,  .y= 53.89  },
//{ .x=  2985  ,  .y= 55.00  },
//{ .x=  2865  ,  .y= 56.11  },
//{ .x=  2751  ,  .y= 57.22  },
//{ .x=  2642  ,  .y= 58.33  },
//{ .x=  2538  ,  .y= 59.44  },
//{ .x=  2438  ,  .y= 60.56  },
//{ .x=  2343  ,  .y= 61.67  },
//{ .x=  2252  ,  .y= 62.78  },
//{ .x=  2165  ,  .y= 63.89  },
//{ .x=  2082  ,  .y= 65.00  },
//};
#define TABLE_SIZE 73
dsp_lookup_f_t my_table[TABLE_SIZE] = {
    { .x=  2082 ,  .y= 65.00 },
    { .x=  2165 ,  .y= 63.89 },
    { .x=  2252 ,  .y= 62.78 },
    { .x=  2343 ,  .y= 61.67 },
    { .x=  2438 ,  .y= 60.56 },
    { .x=  2538 ,  .y= 59.44 },
    { .x=  2642 ,  .y= 58.33 },
    { .x=  2751 ,  .y= 57.22 },
    { .x=  2865 ,  .y= 56.11 },
    { .x=  2985 ,  .y= 55.00 },
    { .x=  3111 ,  .y= 53.89 },
    { .x=  3242 ,  .y= 52.78 },
    { .x=  3380 ,  .y= 51.67 },
    { .x=  3525 ,  .y= 50.56 },
    { .x=  3679 ,  .y= 49.44 },
    { .x=  3838 ,  .y= 48.33 },
    { .x=  4006 ,  .y= 47.22 },
    { .x=  4182 ,  .y= 46.11 },
    { .x=  4367 ,  .y= 45.00 },
    { .x=  4561 ,  .y= 43.89 },
    { .x=  4766 ,  .y= 42.78 },
    { .x=  4981 ,  .y= 41.67 },
    { .x=  5207 ,  .y= 40.56 },
    { .x=  5447 ,  .y= 39.44 },
    { .x=  5697 ,  .y= 38.33 },
    { .x=  5960 ,  .y= 37.22 },
    { .x=  6238 ,  .y= 36.11 },
    { .x=  6530 ,  .y= 35.00 },
    { .x=  6838 ,  .y= 33.89 },
    { .x=  7163 ,  .y= 32.78 },
    { .x=  7505 ,  .y= 31.67 },
    { .x=  7866 ,  .y= 30.56 },
    { .x=  8251 ,  .y= 29.44 },
    { .x=  8653 ,  .y= 28.33 },
    { .x=  9078 ,  .y= 27.22 },
    { .x=  9526 ,  .y= 26.11 },
    { .x=  10000 ,  .y= 25.00 },
    { .x=  10501 ,  .y= 23.89 },
    { .x=  11030 ,  .y= 22.78 },
    { .x=  11590 ,  .y= 21.67 },
    { .x=  12182 ,  .y= 20.56 },
    { .x=  12814 ,  .y= 19.44 },
    { .x=  13478 ,  .y= 18.33 },
    { .x=  14180 ,  .y= 17.22 },
    { .x=  14925 ,  .y= 16.11 },
    { .x=  15714 ,  .y= 15.00 },
    { .x=  16550 ,  .y= 13.89 },
    { .x=  17437 ,  .y= 12.78 },
    { .x=  18378 ,  .y= 11.67 },
    { .x=  19376 ,  .y= 10.56 },
    { .x=  20446 ,  .y= 9.44  },
    { .x=  21573 ,  .y= 8.33  },
    { .x=  22770 ,  .y= 7.22  },
    { .x=  24042 ,  .y= 6.11  },
    { .x=  25395 ,  .y= 5.00  },
    { .x=  26834 ,  .y= 3.89  },
    { .x=  28365 ,  .y= 2.78  },
    { .x=  29996 ,  .y= 1.67  },
    { .x=  31732 ,  .y= 0.56  },
    { .x=  33599 ,  .y= -0.56  },
    { .x=  35573 ,  .y= -1.67  },
    { .x=  37676 ,  .y= -2.78  },
    { .x=  39921 ,  .y= -3.89  },
    { .x=  42317 ,  .y= -5.00  },
    { .x=  44874 ,  .y= -6.11  },
    { .x=  47606 ,  .y= -7.22  },
    { .x=  50526 ,  .y= -8.33  },
    { .x=  53647 ,  .y= -9.44  },
    { .x=  57017 ,  .y= -10.56 },
    { .x=  60592 ,  .y= -11.67 },
    { .x=  64419 ,  .y= -12.78 },
    { .x=  68518 ,  .y= -13.89 },
    { .x=  72911 ,  .y= -15.00 }
};


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

void AdcTask() {
	AdcGetChannelVoltage(0);
//	AdcGetChannelVoltage(1);
	AdcGetChannelTemp(0);
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
	printf(" Result    : %.4f V\n", result);
	printf(" ADC%d     : %d\n",channel, AdcGetChannelRaw(channel));
}

void AdcGetChannelTemp(int channel) {
	double R0 = 10000.0;
//	double B = 3435.0;
//	double K0 = 298.0;
//	double res, felc, felb, fela;
//	double result;

	float v1 = 4096.0 - AdcGetChannelRaw(channel);
	float v2 = v1 / AdcGetChannelRaw(channel);
	uint32_t R = R0 / v2;
	float temp = dsp_lookup_f(my_table, R, TABLE_SIZE);

	printf(" ADC%d     : %d",channel ,AdcGetChannelRaw(channel));
	//printf("\n V2      : %.4f", (float) v2);
	printf("\n R        : %d", (int) R);
	printf("\n Tempreature  : %.2f\n", (float) temp);
	//printf("\n RoomTemp: %.2f C", (float) result - 273);

}

void ADC_IRQHandler(void)
{
    g_u32AdcIntFlag = 1;
    ADC->ADSR = ADC_ADSR_ADF_Msk;      /* clear the A/D conversion flag */
}
