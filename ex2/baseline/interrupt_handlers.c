#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

uint8_t amplitude = 0;
uint32_t duration = 0;
uint8_t high = 0;
uint8_t mode = 0;

#define SQUARE 0
#define SAW 1
#define REVERSE_SAW 2

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */

	*TIMER1_IFC = 0x1;
	if(high) {
		*DAC0_CH0DATA = 0;
		*DAC0_CH1DATA = 0;
	}else {
		*DAC0_CH0DATA = amplitude;
		*DAC0_CH1DATA = amplitude;
	}

	if (mode == SAW) {
		amplitude++;
		amplitude = amplitude % 0xFF;
	} else if (mode == REVERSE_SAW) {
		if (amplitude == 0) {
			amplitude = 0xFF;
		} else {
			amplitude--;
		}
	} else {
		amplitude = 0xFF;
	}
	high = !high;

	if (!duration) {
		*TIMER1_IEN = 0x0;
		*TIMER1_CMD = 0x0;
	} else {
		duration--;
	}
	/*
	   if(soundMode >= 0) {
	   inc++;

	   if(soundMode == 0) {                         //SQUARE
	   if(inc > 0xF) {
	   *DAC0_CH0DATA = 0xFF;
	   *DAC0_CH1DATA = 0xFF;
	   } else {
	   *DAC0_CH0DATA = 0;
	   *DAC0_CH1DATA = 0;
	   }
	   } else if(soundMode == 1) {          // TRIANGLE
	   if(inc < 0xF) {
	   *DAC0_CH0DATA = inc;
	   *DAC0_CH1DATA = inc;
	   } else {
	   *DAC0_CH0DATA += 0xF*2 - inc;
	   *DAC0_CH1DATA += 0xF*2 - inc;
	   }
	   } else if(soundMode == 2) {          // SAW
	   *DAC0_CH0DATA = 0xFF - inc;
	   *DAC0_CH1DATA = 0xFF - inc;
	   }

	   if(inc % 0xFF == 0) {
	   counter++;
	   if(counter >= duration) {
	   counter = 0;
	   soundMode = -1;
	   }
	   }
	   } */
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
}
