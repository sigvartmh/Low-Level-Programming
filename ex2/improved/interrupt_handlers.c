#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define SETLED(led) ~(1<<led) << 8
#define SW1 1
#define SW2 (1<<1)
#define SW3 (1<<2)
#define SW4 (1<<3)
#define SW5 (1<<4)
#define SW6 (1<<5)
#define SW7 (1<<6)
#define SW8 (1<<7)

extern unsigned char _binary_ppap_raw_start;
extern unsigned char _binary_ppap_raw_end;

extern unsigned char _binary_PPAPfull_raw_start;
extern unsigned char _binary_PPAPfull_raw_end;

extern unsigned char _binary_ahh_raw_start;
extern unsigned char _binary_ahh_raw_end;

extern unsigned char _binary_ahhBig_raw_start;
extern unsigned char _binary_ahhBig_raw_end;

extern unsigned char _binary_apple_raw_start;
extern unsigned char _binary_apple_raw_end;

extern unsigned char _binary_pinapple_raw_start;
extern unsigned char _binary_pinapple_raw_end;

extern unsigned char _binary_applePen_raw_start;
extern unsigned char _binary_applePen_raw_end;

extern unsigned char _binary_pinapplePen_raw_start;
extern unsigned char _binary_pinapplePen_raw_end;

extern unsigned char _binary_pen_raw_start;
extern unsigned char _binary_pen_raw_end;

unsigned char *psound = &_binary_ppap_raw_start;
unsigned char *end = &_binary_ppap_raw_end;

void enableDAC();
void disableDAC();

void startLETimer();
void stopLETimer();

void stopTimer();

void enableSleep();
void disableSleep();

void enableTimer();
void disableTimer();

static void gpioHandler(uint8_t input);

static void PinapplePen()
{
	psound = &_binary_pinapplePen_raw_start;
	end = &_binary_pinapplePen_raw_end;
	enableTimer();
	disableSleep();
}

static void ApplePen()
{
	psound = &_binary_applePen_raw_start;
	end = &_binary_applePen_raw_end;
	enableTimer();
	disableSleep();
}

static void pen()
{
	psound = &_binary_pen_raw_start;
	end = &_binary_pen_raw_end;
	enableTimer();
	disableSleep();
}

static void apple()
{
	psound = &_binary_apple_raw_start;
	end = &_binary_apple_raw_end;
	enableTimer();
	disableSleep();
}

static void pinapple()
{
	psound = &_binary_pinapple_raw_start;
	end = &_binary_pinapple_raw_end;
	enableTimer();
	disableSleep();
}

static void ahh()
{
	psound = &_binary_ahh_raw_start;
	end = &_binary_ahh_raw_end;
	enableTimer();
	disableSleep();
}

static void ahhBig()
{
	psound = &_binary_ahhBig_raw_start;
	end = &_binary_ahhBig_raw_end;
	enableTimer();
	disableSleep();
}

static void PPAPfull()
{
	psound = &_binary_PPAPfull_raw_start;
	end = &_binary_PPAPfull_raw_end;
	enableTimer();
	disableSleep();
}

static void gpioHandler(uint8_t input)
{
	enableDAC();
	if (input == SW1) {
		apple();
	} else if (input == SW4) {	//SW4
		ApplePen();
	} else if (input == SW6) {	//SW6
		ahhBig();
	} else if (input == SW8) {
		PinapplePen();
	} else if (input == SW2) {	//SW1
		ahh();
	} else if (input == SW3) {	//SW3
		pen();
	} else if (input == SW5) {	//SW5
		pinapple();
	} else if (input == SW7) {	//SW5
		PPAPfull();
	};
}

//Enables  EM2
void enableSleep()
{
	*SCR = 0x06;
}

//Enables EM1
void disableSleep()
{
	*SCR = 0x00;
}

/* LETIMER0 interrupt handler */
void __attribute__ ((interrupt)) LETIMER0_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */

	*LETIMER0_IFC = 0x1f;
	*LETIMER0_CMD = 0x4;
	*GPIO_PA_DOUT = ~(1 << 1 | 1 << 4 | 1 << 6) << 8;

}

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler()
{
	/*
	   TODO feed new samples to the DAC
	   remember to clear the pending interrupt by writing 1 to TIMER1_IFC
	 */

	*TIMER1_IFC = 0x1;
	*DAC0_CH0DATA = *psound;
	*DAC0_CH1DATA = *psound;

	psound++;

	if (psound == end) {
		disableTimer();
		enableSleep();
		disableDAC();
	}
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	*GPIO_IFC = 0xff;
	uint8_t input = ~(*GPIO_PC_DIN);
	gpioHandler(input);
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = 0xff;
	uint8_t input = ~(*GPIO_PC_DIN);
	gpioHandler(input);
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
}

/* Debug functionality */
void __attribute__ ((interrupt)) HardFault_Handler()
{
	*GPIO_PA_DOUT = ~(1 << 0 | 1 << 3 | 1 << 7) << 8;
}

void __attribute__ ((interrupt)) MemManage_Handler()
{
	*GPIO_PA_DOUT = ~(1 << 1 | 1 << 4 | 1 << 6) << 8;
}

void __attribute__ ((interrupt)) UsageFault_Handler()
{
	*GPIO_PA_DOUT = ~(1 << 1 | 1 << 4 | 1 << 6) << 8;
}

void __attribute__ ((interrupt)) SVC_Handler()
{
	*GPIO_PA_DOUT = ~(1 << 1 | 1 << 4 | 1 << 6) << 8;
}

void __attribute__ ((interrupt)) EMU_IRQHandler()
{
	*GPIO_PA_DOUT = ~(1 << 1 | 1 << 4 | 1 << 6) << 8;
}
