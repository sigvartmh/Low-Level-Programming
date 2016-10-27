#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void enableDAC();
void disableDAC();

extern unsigned char _binary_ppap_raw_start;
extern unsigned char _binary_ppap_raw_end;

unsigned char *psound = &_binary_ppap_raw_start;
unsigned char *end = &_binary_ppap_raw_end;

/*

extern unsigned char _binary_PPAPfull_raw_start;
extern unsigned char _binary_PPAPfull_raw_end;

extern unsigned char _binary_ahhh_raw_start;
extern unsigned char _binary_ahhh_raw_end;

extern unsigned char _binary_ahhBig_raw_start;
extern unsigned char _binary_ahhBig_raw_end;

extern unsigned char _binary_apple_raw_start;
extern unsigned char _binary_apple_raw_end;

extern unsigned char _binary_pinapple_raw_start;
extern unsigned char _binary_pinapple_raw_end;

extern unsigned char _binary_ApplePen_raw_start;
extern unsigned char _binary_ApplePen_raw_end;

extern unsigned char _binary_PinapplePen_raw_start;
extern unsigned char _binary_PinapplePen_raw_end;

extern unsigned char _binary_pen_raw_start;
extern unsigned char _binary_pen_raw_end;

*/

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
	
	if(psound == end){
		*TIMER1_IEN = 0;
		disableDAC();
	}
	
}

static void ppap(){
	psound = &_binary_ppap_raw_start;
	end = &_binary_ppap_raw_end;
	*TIMER1_IEN = 1;
}
/*

static void PinapplePen(){
	psound = &_binary_PinapplePen_raw_start;
	end = &_binary_PinapplePen_raw_end;
	*TIMER1_IEN = 1;
}
static void ApplePen(){
	psound = &_binary_ApplePen_raw_start;
	end = &_binary_ApplePen_raw_end;
	*TIMER1_IEN = 1;
}

static void pen(){
	psound = &_binary_pen_raw_start;
	end = &_binary_pen_raw_end;
	*TIMER1_IEN = 1;
}

static void apple(){
	psound = &_binary_apple_raw_start;
	end = &_binary_apple_raw_end;
	*TIMER1_IEN = 1;
}
static void pinapple(){
	psound = &_binary_pinapple_raw_start;
	end = &_binary_pinapple_raw_end;
	*TIMER1_IEN = 1;
}
// ahh sound 
static void ahh(){
	psound = &_binary_ahhh_raw_start;
	end = &_binary_ahhh_raw_end;
	*TIMER1_IEN = 1;
}
//ahh sound 
static void ahhBig(){
	psound = &_binary_ahhBig_raw_start;
	end = &_binary_ahhBig_raw_end;
	*TIMER1_IEN = 1;
}
// ppap sound 
static void ppap(){
	psound = &_binary_ppap_raw_start;
	end = &_binary_ppap_raw_end;
	*TIMER1_IEN = 1;
}

static void PPAPfull(){
	psound = &_binary_PPAPfull_raw_start;
	end = &_binary_PPAPfull_raw_end;
	*TIMER1_IEN = 1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler()
{
	/* TODO handle button pressed event, remember to clear pending interrupt */
	*GPIO_IFC = 0xff;
	
	uint8_t input = ~(*GPIO_PC_DIN);
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
}


/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler()
{
	*GPIO_IFC = 0xff;
	*GPIO_PA_DOUT = *GPIO_PC_DIN << 8;
	uint8_t input = ~(*GPIO_PC_DIN);
	if(input == 2){	//SW2
		*GPIO_PA_DOUT = ~(1<<0|1<<3|1<<7) << 8;
		enableDAC();
		ppap();
	}else if(input == 0x8){ //SW4
		disableDAC();
	}
	/* TODO handle button pressed event, remember to clear pending interrupt */
}

void __attribute__ ((interrupt))  HardFault_Handler()
{
	*GPIO_PA_DOUT = ~(1<<0|1<<3|1<<7) << 8;
}

void __attribute__ ((interrupt))  MemManage_Handler()
{
	*GPIO_PA_DOUT = ~(1<<1|1<<4|1<<6) << 8;
}

void __attribute__ ((interrupt)) UsageFault_Handler()
{
	*GPIO_PA_DOUT = ~(1<<1|1<<4|1<<6) << 8;
}

void __attribute__ ((interrupt)) SVC_Handler()
{
	*GPIO_PA_DOUT = ~(1<<1|1<<4|1<<6) << 8;
}

void __attribute__ ((interrupt)) EMU_IRQHandler()
{
	*GPIO_PA_DOUT = ~(1<<1|1<<4|1<<6) << 8;
}
/*
static void gpioHandler(uint8_t input){

	if(input == 2){	//SW2
		ahh();
	}else if(input == 0x8){ //SW4
		ApplePen();
	}else if(input == 0x20){ //SW4
		ahhBig();
	}else if(input == 0x80){
		PinapplePen();
	}else if(input == 1){	//SW1
		apple();
	}else if(input == 0x4){ //SW3
		pen();
	}else if(input == 0x10){ //SW5
		pinapple();
	}else if(input == 0x40){ //SW5
		PPAPfull();
	};
}*/
