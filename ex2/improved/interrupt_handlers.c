#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define sampleRate	7*1000000
/*#define tone(x) (x/sampleRate)

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
#define C4		tone(262)*/

uint8_t volatile currentTone = 0;
uint8_t limit = 5;
uint8_t counter = 0;
uint8_t inc = 0;
uint8_t ch0 = 0;
uint8_t ch1 = 1;

void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */
	 
	*TIMER1_IFC = 0x1;
	inc++;
	
	if(counter < limit) {
		if(currentTone == 0) {
			ch0 = 0xFF - inc;
			ch1 = 0xFF - inc;
		} else if(currentTone == 1) {
			ch0 = ~ch0;
			ch1 = ~ch1;
		} else {
			
		}
		
		
		*DAC0_CH0DATA = ch0;
		*DAC0_CH1DATA = ch1;
		
		if(inc % 0xFF == 0) {
			counter++;
		}
	}
}

void update() {
	counter = 0;
	inc = 0;
	
	for(int i = 0; i < 8; i++) {
		bool hasNote = ((~(*GPIO_PC_DIN) >> i) &0x1);
		uint8_t input = ~(*GPIO_PC_DIN);
		if(hasNote) {
			currentTone = i % 3;
			return;
		}
	}
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	/* TODO handle button pressed event, remember to clear pending interrupt */
	update();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	/* TODO handle button pressed event, remember to clear pending interrupt */
	update();
}
