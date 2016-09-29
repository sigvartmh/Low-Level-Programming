#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define sampleRate	7*1000000
#define tone(x) (x/sampleRate)

#define C5 		tone(523)
#define B4		tone(494)
#define Bb4		tone(466)
#define A4		tone(440)
#define Ab4		tone(415)
#define G4		tone(392)
#define Gb4		tone(370)
#define F4		tone(349)
#define E4		tone(330)
#define Eb4		tone(311)
#define D4		tone(294)
#define Db4		tone(277)
#define C4		tone(262)

uint8_t currentTone = 0;
uint8_t limit = 1;
uint8_t counter = 0;
uint8_t inc = 0;
uint8_t ch0 = 0;
uint8_t ch1 = 1;
uint8_t tones[] = {C4, Db4, D4, Eb4, E4, F4, Gb4, G4, Ab4, A4, Bb4, B4, C5};
/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
	*TIMER1_IFC = 0x1;
	inc++;
	(*GPIO_PA_DOUT) &= ~(1 << inc);
	
	//ch0 = 0xFF - inc;
	//ch1 = 0xFF - inc;
	
	ch0 = ~ch0;
	ch1 = ~ch1;
		
	*DAC0_CH0DATA = ch0;
	*DAC0_CH1DATA = ch1;
	
	if(counter < limit) {
		ch0 = 0xFF - inc;
		ch1 = 0xFF - inc;
		
		*DAC0_CH0DATA = ch0;
		*DAC0_CH1DATA = ch1;
		
		if(inc % 0xFF == 0) {
			counter++;
		}
	} else {
		generateSound((currentTone++) % 13, 100);
	}

}

void generateSound(uint8_t tone, uint8_t length) {
	*TIMER1_TOP = tones[tone];
	counter = 0;
	limit = length;
}

/*uint8_t sin(uint8_t x) {
	if()
}*/

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	/* TODO handle button pressed event, remember to clear pending interrupt */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	/* TODO handle button pressed event, remember to clear pending interrupt */
}
