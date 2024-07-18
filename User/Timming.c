#include <stdio.h>
#include "NUC029xAN.h"

uint32_t sayac=-1;
uint32_t timer;
uint32_t timer2;

void TMR0_IRQHandler(void) {
	if ((TIMER0->TISR & (1 << 0) ? 1 : 0) == 1) {
		/* Clear Timer0 time-out interrupt flag */
		TIMER0->TISR |= (1 << 0);
		P37 = !P37;
		sayac++;
	}
}

void TimmingPortInit(){
	TIMER0->TCSR = 0
					| TIMER_TCSR_IE_Msk
					| TIMER_PERIODIC_MODE
					| TIMER_TCSR_CEN_Msk
					| (25 << TIMER_TCSR_PRESCALE_Pos);

	TIMER1->TCSR = 0
			        | TIMER_TCSR_IE_Msk
					| TIMER_CONTINUOUS_MODE
					| TIMER_TCSR_CEN_Msk
					| (25 << TIMER_TCSR_PRESCALE_Pos);

	TIMER0->TCMPR = 86400; // 1ms period
	P3->PMD |= (1 << GPIO_PMD_PMD7_Pos); //P3.7 output mode
	P37=0;
	NVIC_EnableIRQ(TMR0_IRQn);
}
void TimmingTask() {
//		TimmingGetMs();
//		TimmingDiffMs();
	}

void TimmingGetMs() {
	TIMER0->TCSR |= TIMER_TCSR_TDR_EN_Msk;
	TIMER1->TCSR |= TIMER_TCSR_TDR_EN_Msk;

	timer = TIMER0->TDR + sayac*86400;
	timer2 = TIMER1->TDR;
	printf("\ntimer is %d ms\n",timer);
	//printf("sayac is %d\n",sayac);
	//printf("timer2 is %d ms\n",timer2);
}

void TimmingDiffMs() {
	uint32_t diff = timer- timer2;
	//printf("difference is %d ms\n",diff);
	}
