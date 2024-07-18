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
    CLK_EnableModuleClock(ADC_MODULE);
    CLK_EnableModuleClock(I2C0_MODULE);

    CLK_SetModuleClock(PWM01_MODULE, CLK_CLKSEL1_PWM01_S_HIRC, MODULE_NoMsk);
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));
    CLK_SetModuleClock(ADC_MODULE,CLK_CLKSEL1_ADC_S_HIRC,CLK_CLKDIV_ADC(1));

    CLK->APBCLK = 0
		    |CLK_APBCLK_UART0_EN_Msk
	        |CLK_APBCLK_TMR0_EN_Msk
	        |CLK_APBCLK_TMR1_EN_Msk
		    |CLK_APBCLK_PWM01_EN_Msk
		    |CLK_APBCLK_PWM23_EN_Msk
			|CLK_APBCLK_ADC_EN_Msk;

	CLK->CLKSEL1 = 0
			|CLK_CLKSEL1_UART_S_Msk //TRM sayfa 213  clock select HIRC
	        |(0 << CLK_CLKSEL1_TMR0_S_Pos)
			|(0 << CLK_CLKSEL1_TMR1_S_Pos)
			|(3<<CLK_CLKSEL1_ADC_S_Pos)
	        |CLK_CLKSEL1_PWM01_S_HXT
	    	|CLK_CLKSEL1_PWM23_S_HXT;

	/* Update System Core Clock */
	/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and cyclesPerUs automatically. */
	SystemCoreClockUpdate();

	/* Set P3 multi-function pins for UART0 RXD and TXD  */
    SYS->P1_MFP = 0
    		|(SYS_MFP_P10_AIN0)
			|(SYS_MFP_P11_AIN1);

	SYS->P2_MFP = 0
			|SYS_MFP_P21_PWM1
			|SYS_MFP_P20_PWM0;

	SYS->P3_MFP = 0
			|SYS_MFP_P30_RXD0
			|SYS_MFP_P31_TXD0
            |SYS_MFP_P37_GPIO
	        |SYS_MFP_P34_SDA0
			|SYS_MFP_P35_SCL0;

	/* Lock protected registers */
	SYS_LockReg();
}

int main() {
	SYS_Init();

	SerialPortInit();

	AdcInit();

	PwmInit();

	TimmingPortInit();

//NVIC_EnableIRQ(UART0_IRQn);
	while (1){
		TimmingTask();
		AdcTask();
		PwmTask();
		SerialPortTask();
		for(volatile uint32_t t = 0 ;t < 1000000;t++);
	};
}

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
