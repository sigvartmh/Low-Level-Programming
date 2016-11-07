#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/
/* The period between sound samples, in clock cycles */
#define   TIMER_FREQUENCY 14000000

/* these should have been put in a header file used by interrupt_handlers.c and ex2.c */
//define buttons
#define SW1 1
#define SW2 (1<<1)
#define SW3 (1<<2)
#define SW4 (1<<3)
#define SW5 (1<<4)
#define SW6 (1<<5)
#define SW7 (1<<6)
#define SW8 (1<<7)

//define waves
#define SQUARE 0
#define SAW 1
#define REVERSE_SAW 2

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupTimer(uint32_t period);
void setTimer(uint32_t frequency);
void setupDAC();
void setupNVIC();

void pollButtons();
void introSong();
void pew();
void boop();
void boing();

/* sourced from interrupt handler to control duration */
extern uint32_t duration;
extern uint8_t amplitude;
extern uint8_t mode;

/*Your code will start executing here */

void pollButtons()
{
	uint8_t input = (~(*GPIO_PC_DIN));
	*GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
	if (input == SW1) {
		mode = SAW;
		boing();
	} else if (input == SW2) {
		mode = SQUARE;
		pew();
	} else if (input == SW3) {
		mode = REVERSE_SAW;
		boop();
	} else if (input == SW4) {
		mode = SAW;
		pew();
	} else if (input == SW5) {
		mode = SQUARE;
		boing();
	} else if (input == SW6) {
		mode = REVERSE_SAW;
		pew();
	} else if (input == SW7) {
		mode = SQUARE;
		boop();
	} else if (input == SW8) {
		mode = SAW;
		boop();
	}
}

void introSong()
{
	uint32_t freq_list[] = { 640, 240, 750, 340, 245, 640, 10, 350, 10 };
	uint8_t len = 9;
	for (uint8_t i = 0; i < len; i++) {
		duration = freq_list[i] / 3;
		setTimer(freq_list[i]);
		while (duration != 0) ;
	}

}

void pew()
{
	uint32_t freq_list[] = { 120, 240, 750, 650, 750, 750 };
	uint8_t len = 6;
	for (uint8_t i = 0; i < len; i++) {
		duration = freq_list[i] / 20;
		setTimer(freq_list[i]);
		while (duration != 0) ;
	}
}

void boop()
{
	uint32_t freq_list[] = { 640, 540, 740, 340, 270, 150, 20, 10 };
	uint8_t len = 8;
	for (uint8_t i = 0; i < len; i++) {
		duration = freq_list[i] / 10;
		setTimer(freq_list[i]);
		while (duration != 0) ;
	}

}

void boing()
{
	uint32_t freq_list[] = { 100, 240, 740, 240, 170 };
	uint8_t len = 5;
	for (uint8_t i = 0; i < len; i++) {
		duration = freq_list[i] / 5;
		setTimer(freq_list[i]);
		while (duration != 0) ;
	}

}

int main(void)
{

	/* Call the peripheral setup functions */
	setupGPIO();
	setupDAC();
	setupTimer(0);
	//*TIMER1_CMD = 0;
	/* Enable interrupt handling */
	setupNVIC();

	/* TODO for higher energy efficiency, sleep while waiting for interrupts
	   instead of infinite loop for busy-waiting
	 */
	introSong();

	while (1) {
		//input = *GPIO_PC_DIN << 8;    /* Enable internal pull-up */
		/* Enable internal pull-up */
		pollButtons();
	};

	return 0;
}

void setupNVIC()
{
	/* TODO use the NVIC ISERx registers to enable handling of interrupt(s)
	   remember two things are necessary for interrupt handling:
	   - the peripheral must generate an interrupt signal
	   - the NVIC must be configured to make the CPU handle the signal
	   You will need TIMER1, GPIO odd and GPIO even interrupt handling for this
	   assignment.
	 */
	*ISER0 = 0x802;
	*ISER0 |= (1 << 12);
}

/* if other interrupt handlers are needed, use the following names: 
   NMI_Handler
   HardFault_Handler
   MemManage_Handler
   BusFault_Handler
   UsageFault_Handler
   Reserved7_Handler
   Reserved8_Handler
   Reserved9_Handler
   Reserved10_Handler
   SVC_Handler
   DebugMon_Handler
   Reserved13_Handler
   PendSV_Handler
   SysTick_Handler
   DMA_IRQHandler
   GPIO_EVEN_IRQHandler
   TIMER0_IRQHandler
   USART0_RX_IRQHandler
   USART0_TX_IRQHandler
   USB_IRQHandler
   ACMP0_IRQHandler
   ADC0_IRQHandler
   DAC0_IRQHandler
   I2C0_IRQHandler
   I2C1_IRQHandler
   GPIO_ODD_IRQHandler
   TIMER1_IRQHandler
   TIMER2_IRQHandler
   TIMER3_IRQHandler
   USART1_RX_IRQHandler
   USART1_TX_IRQHandler
   LESENSE_IRQHandler
   USART2_RX_IRQHandler
   USART2_TX_IRQHandler
   UART0_RX_IRQHandler
   UART0_TX_IRQHandler
   UART1_RX_IRQHandler
   UART1_TX_IRQHandler
   LEUART0_IRQHandler
   LEUART1_IRQHandler
   LETIMER0_IRQHandler
   PCNT0_IRQHandler
   PCNT1_IRQHandler
   PCNT2_IRQHandler
   RTC_IRQHandler
   BURTC_IRQHandler
   CMU_IRQHandler
   VCMP_IRQHandler
   LCD_IRQHandler
   MSC_IRQHandler
   AES_IRQHandler
   EBI_IRQHandler
   EMU_IRQHandler
*/
