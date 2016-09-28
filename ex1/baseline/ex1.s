.syntax unified

.include "efm32gg.s"
.include "vectortable.s"

.section .text
/////////////////////////////////////////////////////////////////////////////
//
// Reset handler
// The CPU will start executing here after a reset
//
/////////////////////////////////////////////////////////////////////////////

	.globl  _reset
	.type   _reset, %function
	.thumb_func

_reset:
	//Set CMU
	ldr r1, =CMU_BASE
	ldr r2, [r1, #CMU_HFPERCLKEN0]
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3
	str r2, [r1, #CMU_HFPERCLKEN0]

	//Set high strength
	ldr r3, =0x2
	ldr r4, =GPIO_PA_BASE
	str r3, [r4, #GPIO_CTRL]

	//Set pins 8-15, output
	ldr r3, =0x55555555
	str r3, [r4, #GPIO_MODEH]

	//Set pins 0-7, input
	ldr r3, =0x33333333
	ldr r4, =GPIO_PC_BASE
	str r3, [r4, #GPIO_MODEL]

	//Enable internal pull-up
	ldr r3, =0xff
	str r3, [r4, #GPIO_DOUT]

	b button_pulling

.thumb_func
	button_pulling:
		ldr r4, =GPIO_PC_BASE
		ldr r5, =GPIO_PA_BASE
	while:
		ldr r6, [r4, #GPIO_DIN]
		lsl r6, r6, #8
		str r6, [r5, #GPIO_DOUT]
		b while

/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////

    .thumb_func
gpio_handler:
	b .  // do nothing

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:
	b .  // do nothing

