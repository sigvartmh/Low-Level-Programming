#include <stdint.h>
#include <stdbool.h>
#include "efm32gg.h"

#define   TIMER_FREQUENCY 14000000
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

void setTimer(uint32_t frequency)
{
	*TIMER1_TOP = TIMER_FREQUENCY / (2 * frequency);	//due to nyquist-theorem about sampling
	*TIMER1_IEN = 0x1;
	*TIMER1_CMD = 0x1;
};
