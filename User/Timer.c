#include <stdio.h>
#include "NUC029xAN.h"

void TMR0_IRQHandler(void) {
	if ((TIMER0->TISR & (1 << 0) ? 1 : 0) == 1) {
		/* Clear Timer0 time-out interrupt flag */
		TIMER0->TISR |= (1 << 0);
		P35 = !P35;	}
}
