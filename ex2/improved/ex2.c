#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "efm32gg.h"

/* 
  TODO calculate the appropriate sample period for the sound wave(s) 
  you want to generate. The core clock (which the timer clock is derived
  from) runs at 14 MHz by default. Also remember that the timer counter
  registers are 16 bits.
*/

/* The period between sound samples, in clock cycles */
/* Samples are 8000Hz */
/* CPU Hz = 48 MHz */
/* TIMER Hz = 14 MHz */
#define SAMPLE_PERIOD 325

/* Declaration of peripheral setup functions */
void setupGPIO();
void setupGPIOInterrupt();
void setupTimer(uint32_t period);
void setupDAC();
void setupLETimer();
void disableDAC();
void enableDAC();
void setupNVIC();

volatile uint32_t input = 0;

void disableRAM(uint8_t type)
{
	*EMU_MEMCTRL = type;
}

/* Your code will start executing here */
int main(void)
{
	/* Call the peripheral setup functions */
	setupGPIO();
	setupGPIOInterrupt();
	setupDAC();
	enableDAC();
	setupTimer(SAMPLE_PERIOD);

	/* Enable interrupt handling */
	setupNVIC();
	/* Disable RAM-block 1-3 */
	disableRAM(0x07);
	/* Sleep mode will be enabled once the start song is finshed */
	__asm("wfi");
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
	*ISER0 = 0x802;		//Enable GPIO odd and even interrupt handeling
	*ISER0 |= (1 << 12);	//Enable Timer1 interupt
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
