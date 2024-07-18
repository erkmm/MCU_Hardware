#include <stdio.h>
#include "NUC029xAN.h"

void PwmInit() {
    PWMA->PPR = (PWMB->PPR & ~(PWM_PPR_CP01_Msk)) | (24 << PWM_PPR_CP01_Pos);

    PWMA->CSR = (PWMA->CSR & ~(PWM_CSR_CSR0_Msk)) | (PWM_CLK_DIV_4 << PWM_CSR_CSR0_Pos);
    PWMA->PCR = 0
    		|PWM_PCR_CH0EN_Msk
    		|PWM_PCR_CH0MOD_Msk;
    PWMA->POE |= PWM_POE_PWM0_Msk;
	PWMA->PIER = PWM_PIER_PWMIE0_Msk;

	PWMA->CSR = (PWMA->CSR & ~(PWM_CSR_CSR1_Msk)) | (PWM_CLK_DIV_4 << PWM_CSR_CSR1_Pos);
    PWMA->PCR = 0
    		|PWM_PCR_CH1EN_Msk
    		|PWM_PCR_CH1MOD_Msk;
    PWMA->POE |= PWM_POE_PWM1_Msk;
	PWMA->PIER = PWM_PIER_PWMIE1_Msk;}

void PwmTask(){
	PwmChannelSetValue(0, 30);
	PwmChannelSetValue(1, 90);
}

void PwmChannelSetValue(int channel, int value){
	// Ensure value is within a valid range (0 to 100)
	    if (value < 0 || value > 100) {
	        return; // Invalid value, ignore or handle error
	    }

	    // Set CNR and CMR based on the channel
	    if (channel == 0) {
	        PWMA->CNR0 = 100;   // Set the counter register to 100
	        PWMA->CMR0 = value; // Set the comparator register to the desired value
	    } else if (channel == 1) {
	        PWMA->CNR1 = 100;   // Set the counter register to 100
	        PWMA->CMR1 = value; // Set the comparator register to the desired value
	    }
	}

/*void PWMA_IRQHandler(void)
{
    static int toggle = 0;

    // Update PWMA channel 0 period and duty
    if(toggle == 0)
    {
        PWMA->CNR0 = 50;
        PWMA->CMR0 = 50;
    }
    else
    {
        PWMA->CNR0 = 50;
        PWMA->CMR0 = 0;
    }
    toggle ^= 1;
    // Clear channel 0 period interrupt flag;
    PWMA->PIIR |= PWM_PIIR_PWMIF0_Msk;}*/
