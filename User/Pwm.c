#include <stdio.h>
#include "NUC029xAN.h"

void PWMA_IRQHandler(void)
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
    PWMA->PIIR |= PWM_PIIR_PWMIF0_Msk;
}
