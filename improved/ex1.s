.syntax unified

.include "efm32gg.s"
.include "exception_vectors.s"

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

	//Set low strength
	mov r3, #1
	ldr r4, =GPIO_PA_BASE
	str r3, [r4, #GPIO_CTRL]

	//Set pins 8-15, output
	ldr r3, =0x55555555
	str r3, [r4, #GPIO_MODEH]

	//Set all pins high
	mov r5, #0xff00
	orr r3, r3, r5
	str r3, [r4, #GPIO_DOUT]

	//Set pins 0-7, input
	ldr r3, =0x33333333
	ldr r4, =GPIO_PC_BASE
	str r3, [r4, #GPIO_MODEL]

	//Enable internal pull-up
	ldr r3, =0xff
	str r3, [r4, #GPIO_DOUT]



    bl main

main:
	//Disable RAM-Block 1-3
	ldr r1, =EMU_BASE
	mov r2, #0x07
	str r2, [r1, #EMU_MEMCTRL]

    //Enable sleep CORTEX-M3 system register
	ldr r1, =SCR
    mov r2, #6
	str r2, [r1]

    //Enable interrupt
	ldr r7, =0x802
	ldr r6, =ISER0
	str r7, [r6, #0]

    //Extipsell
	ldr r1, =0x22222222
	ldr r2, =GPIO_BASE
	str r1, [r2, #GPIO_EXTIPSELL]

	//Set rise/fall
	ldr r1, =0xff
	str r1, [r2, #GPIO_EXTIRISE]
	str r1, [r2, #GPIO_EXTIFALL]
	str r1, [r2, #GPIO_IEN]

    //Setup output register
    loop:
        wfi
        ldr r3, =GPIO_PA_BASE
	    mov r5, #0xff00
	    orr r3, r3, r5
	    str r3, [r4, #GPIO_DOUT]
        b loop


/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
//
/////////////////////////////////////////////////////////////////////////////



    .thumb_func
gpio_handler:
    //Clear interrupt
	ldr r7, [r2, #GPIO_IF]
	str r7, [r2, #GPIO_IFC]

    ldr r4, =GPIO_PC_BASE
	ldr r6, [r4, #GPIO_DIN]
	lsl r6, r6, #8

    ldr r5, =GPIO_PA_BASE
    str r6, [r5, #GPIO_DOUT]
    bx lr

/////////////////////////////////////////////////////////////////////////////

    .thumb_func
dummy_handler:
	b .  // do nothing

