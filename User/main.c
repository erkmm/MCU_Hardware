#include <stdio.h>
#include "NUC029xAN.h"

void SYS_Init(void) {

	/* Unlock protected registers */
	SYS_UnlockReg();

	/* If the macros do not exist in your project, please refer to the related clk.h in Header folder of the tool package */
	/* Enable clock source */
	CLK_EnableXtalRC(
			CLK_PWRCON_OSC10K_EN_Msk | CLK_PWRCON_OSC22M_EN_Msk
					| CLK_PWRCON_XTL12M_EN_Msk);

	/* Waiting for clock source ready */
	CLK_WaitClockReady(
			CLK_CLKSTATUS_OSC10K_STB_Msk | CLK_CLKSTATUS_OSC22M_STB_Msk
					| CLK_CLKSTATUS_XTL12M_STB_Msk);

	/* Disable PLL first to avoid unstable when setting PLL */
	CLK_DisablePLL();

	/* Set PLL frequency */
	CLK->PLLCON = (CLK->PLLCON & ~(0x000FFFFFUL)) | 0x0000C22EUL;

	/* Waiting for PLL ready */
	CLK_WaitClockReady(CLK_CLKSTATUS_PLL_STB_Msk);

	/* Set HCLK clock */
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

	/* Enable IP clock */
    CLK_EnableModuleClock(PWM01_MODULE);
    CLK_EnableModuleClock(UART0_MODULE);

    CLK_SetModuleClock(PWM01_MODULE, CLK_CLKSEL1_PWM01_S_HIRC, MODULE_NoMsk);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));

	CLK->APBCLK = 0
		   |CLK_APBCLK_UART0_EN_Msk
	       |CLK_APBCLK_TMR0_EN_Msk
		   |CLK_APBCLK_PWM01_EN_Msk
		   |CLK_APBCLK_PWM23_EN_Msk;

	CLK->CLKSEL1 = 0
			|CLK_CLKSEL1_UART_S_Msk //TRM sayfa 213  clock select HIRC
	        | (0 << CLK_CLKSEL1_TMR0_S_Pos);

    CLK->CLKSEL2 = 0
    		|CLK_CLKSEL1_PWM01_S_HXT
    		|CLK_CLKSEL1_PWM23_S_HXT;

//	A = 0x0502;
//	A = (A & 0xF0FF);
//	A = A & ~(0xF << 8);

	/* Update System Core Clock */
	/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and cyclesPerUs automatically. */
	SystemCoreClockUpdate();

	/* Set P3 multi-function pins for UART0 RXD and TXD  */
    SYS->P2_MFP |= (SYS_MFP_P20_PWM0);
	SYS->P3_MFP = 0
			|SYS_MFP_P30_RXD0
			|SYS_MFP_P31_TXD0
            |SYS_MFP_P35_GPIO
	        |SYS_MFP_P34_T0;

	/* Lock protected registers */
	SYS_LockReg();
}
int main() {
	SYS_Init();

	UART_Init();
    PWMA->PPR = (PWMB->PPR & ~(PWM_PPR_CP01_Msk)) | (24 << PWM_PPR_CP01_Pos);
    PWMA->CSR = (PWMA->CSR & ~(PWM_CSR_CSR0_Msk)) | (PWM_CLK_DIV_4 << PWM_CSR_CSR0_Pos);
    PWMA->PCR |= PWM_PCR_CH0MOD_Msk;
    PWMA->CMR0 = 99;
    PWMA->CNR0 = 199;
    PWMA->POE |= PWM_POE_PWM0_Msk;
    PWMA->PCR |= PWM_PCR_CH0EN_Msk;
	PWMA->PIER = PWM_PIER_PWMIE0_Msk;


	TIMER0->TCSR = 0
				| TIMER_TCSR_IE_Msk
				| (2 << TIMER_TCSR_MODE_Pos)
				| TIMER_TOGGLE_MODE
				| TIMER_TCSR_CEN_Msk
				| (25 << TIMER_TCSR_PRESCALE_Pos);

	TIMER0->TCMPR = 86400; // 1ms period
	P3->PMD |= (1 << GPIO_PMD_PMD5_Pos); //P3.5 output mode

	UART0->IER = 0
			| UART_IER_RDA_IEN_Msk
			| UART_IER_THRE_IEN_Msk;
	P35=0;

	NVIC_EnableIRQ(PWMA_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_EnableIRQ(TMR0_IRQn);

	while (1);
}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
