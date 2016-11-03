#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to setup the timer */
void setupTimer(uint16_t period)
{
	/*
	   TODO enable and set up the timer

	   1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0
	   2. Write the period to register TIMER1_TOP
	   3. Enable timer interrupt generation by writing 1 to TIMER1_IEN
	   4. Start the timer by writing 1 to TIMER1_CMD

	   This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.
	 */

	*CMU_HFPERCLKEN0 |= (1 << 6);
	*TIMER1_TOP = period;
	*TIMER1_IEN = 1;
	*TIMER1_CMD = 1;
}

void setupLETimer(){
	/* Enable Low Frequency Clock Tree */
	*CMU_HFPERCLKEN0 |= 0x200;
	*CMU_LFACLKEN0   |= (1 << 2);
	/* 32.768Hz Low Frequency (RC) External Oscillator */
	// LFXO clock selected
	*CMU_LFCLKSEL |= (1 << 1);
	*CMU_OSCENCMD |= 0x100; 
	/* Low Frequency A Prescaler Register 0 */
	*CMU_LFAPRESC0 |= (0b101 << 8); //F=LFACLK/32
	
	*LETIMER0_CMD = 0x2;
	*LETIMER0_CTRL |= 0x200;
	*LETIMER0_COMP0 = 1;
	*LETIMER0_IFC = 0x1f; 
	*LETIMER0_IEN = 0x4;
}

void startLETimer(){
	*LETIMER0_CMD = 0x1;
}

void stopLETimer(){
	*LETIMER0_CMD = 0x2;
}
