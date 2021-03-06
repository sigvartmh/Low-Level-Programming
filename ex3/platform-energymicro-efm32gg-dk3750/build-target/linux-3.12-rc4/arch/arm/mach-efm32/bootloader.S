#if defined(CONFIG_OF)
#define EFM32_USE_OF
#endif
#define EFM32_DT_IN_SRAM

/* UART1 */
#define UARTBASE 0x4000e400
#define UARTLOCATION 2

	.thumb

	.section ".bootloader","ax"

	/* M3 Vector table */
	.int 0x10001000		@ Initial SP value
	.int reset + 1		@ Reset

reset:
	/* init external RAM, serial port, EBI and stuff */
	adr	r0, reginit
1:
	ldr	r1, [r0]
	ldr	r2, [r0, #4]
	str	r2, [r1]
	add	r0, #8
	cmp	r0, #(reginit_end)
	blo	1b


	/* init some BC registers */
	adr	r0, bcinit
1:
	ldrh	r1, [r0]
	ldrh	r2, [r0, #2]
	add	r1, r1, #0x80000000
	strh	r2, [r1]
	add	r0, #4
	cmp	r0, #(bcinit_end)
	blo	1b

	/* give mux some time to enable the level shifter */
	ldr	r0, =0x4000
1:	subs	r0, r0, #1
	bne	1b

	ldr	r0, =(UARTBASE + 0x34)
	mov	r1, 0x55
	str	r1, [r0]

	/* Zero PSRAM */
	ldr	r0, =(0x16000000)
	ldr	r1, =(0x16400000)
	mov	r2, #0
	mov	r3, #0
	mov	r4, #0
	mov	r5, #0
1:	stmia	r0!, {r2-r5}
	cmp	r0, r1
	bcc	1b

	/* assert zeroing succeeded */
	ldr	r6, =(UARTBASE + 0x34)
	mov	r7, #'<'
	str	r7, [r6]
	mov	r7, #'*'
	ldr     r0, =(0x16000000)
1:	ldmia	r0!, {r2-r5}
	orr	r2, r3
	orr	r4, r5
	orr	r2, r4
	cmp	r2, #0
	it	ne
	strne	r7, [r6]
	cmp	r0, r1
	bcc	1b
	mov	r7, #'>'
	str	r7, [r6]

#if defined(EFM32_USE_OF) && defined(EFM32_DT_IN_SRAM)
#define dtbaddr 0x10000000
	ldr	r0, =(dtbaddr)
	ldr	r1, =(dtb)
	adr	r2, dtbend
	subs	r2, r2, r1
	bl	_memcpy
#endif
	/* detect machine type; easy we know this is an efm32gg_dk3750 */
	movw	r0, #0
	movw	r1, #0xf11	@ machid for efm32gg_dk3750
#ifdef EFM32_USE_OF
#ifdef EFM32_DT_IN_SRAM
	ldr	r2, =(dtbaddr)
#else
	adr	r2, dtb
#endif
#else
	adr	r2, ataglist
#endif
	movw	r3, #0
	movw	r4, #0
	movw	r5, #0
	movw	r6, #0
	movw	r7, #0

	b	stext

#ifdef EFM32_DT_IN_SRAM
_memcpy:
	@ copies r2 bytes from r1 to r0 with r2 > 0
1:
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	subs	r2, #1
	bhi	1b
	bx	lr
#endif

	.ltorg

	.align	3
	/* register value pairs to initialize the machine */
	.type reginit, %object
reginit:
	/* clocks */
	.int	0x400C8020, 0x00000004		@ CMU_OSCENCMD |= HFXOEN

	.int	0x400c8008, 0x00000100		@ CMU_HFPERCLKDIV, reset default
	.int	0x43900814, 0x00000001		@ CMU_HFCORECLKEN0 |= EBI via bitband
	.int	0x439008b4, 0x00000001		@ CMU_HFPERCLKEN0 |= GPIO via bitband
	.int	0x43900884, 0x00000001		@ CMU_HFPERCLKEN0 |= USART1 via bitband
	.int	0x43900890, 0x00000001		@ CMU_HFPERCLKEN0 |= UART1 via bitband

	/* XXX */
	.int	0x439008c0, 0x00000001		@ CMU_HFPERCLKEN0 |= ADC via bitband

	/* pinmuxing */
	.int	0x40006000, 0x00000000		@ GPIO_PA_CTRL, reset default
	.int	0x40006014, 0x0000807f		@ GPIO_PA_DOUTCLR; EBI AD8..15 set dataout to 0
	.int	0x40006004, 0x04444444		@ GPIO_PA_MODEL; EBI AD9..15 set mode=pushpull
	.int	0x40006008, 0x40000000		@ GPIO_PA_MODEH; EBI AD8 set mode=pushpull
	.int	0x40006024, 0x00000000		@ GPIO_PB_CTRL, reset default
	.int	0x40006038, 0x00008000		@ GPIO_PB_DOUTCLR; EBI mode on PB15 MCU_EBI_CONNECT (0)
	.int	0x40006038, 0x0000007f		@ GPIO_PB_DOUTCLR; EBI A16-22
	.int	0x40006028, 0x04444444		@ GPIO_PB_MODEL; EBI A16-22
	.int	0x40006034, 0x00000200		@ GPIO_PB_DOUTSET; set UART_TX to avoid false start
	.int	0x4000602c, 0x40000140		@ GPIO_PB_MODEH; MCU_EBI_CONNECT -> output, UART_TX, UART_RX

	.int	0x40006048, 0x00000000		@ GPIO_PC_CTRL, reset default
	.int	0x4000605c, 0x00000001		@ GPIO_PC_DOUTCLR; PC11 (EBI_ALE)
	.int	0x40006050, 0x00004000		@ GPIO_PC_MODEH; PC11: Push-pull output
	.int	0x4000606c, 0x00000000		@ GPIO_PD_CTRL, reset default
	.int	0x4000607c, 0x00003e08		@ GPIO_PD_DOUTSET, EBI CS0-3, spiconnect set dataout to 1; ETH_SPI_#CS (D3)
	.int	0x40006080, 0x00000007		@ GPIO_PD_DOUTCLR, ETH_SPI_{TX, RX, CLK}
	.int	0x40006070, 0x00004414		@ GPIO_PD_MODEL; ETH_SPI_
	.int	0x40006074, 0x00444440		@ GPIO_PD_MODEH; EBI CS0-3, spiconnect set mode=pushpull
	.int	0x40006090, 0x00000000		@ GPIO_PE_CTRL, reset default
	.int	0x400060a0, 0x00000001		@ GPIO_PE_DOUTSET; FPGA irq line
	.int	0x400060a4, 0x0000ff00		@ GPIO_PE_DOUTCLR; EBI AD0..7 set dataout to 0
	.int	0x40006094, 0x00000002		@ GPIO_PE_MODEL; FPGA irq line set mode=inputpull
	.int	0x40006098, 0x44444444		@ GPIO_PE_MODEH; EBI AD0..7 set mode=pushpull

	.int	0x400060b4, 0x00000000		@ GPIO_PF_CTRL, reset default
	.int	0x400060c8, 0x000003c0		@ GPIO_PF_DOUTCLR; EBI Wen+Ren set dataout to 0
	.int	0x400060b8, 0x44000000		@ GPIO_PF_MODEL; EBI Byte Lane 0 support BL0/BL1
	.int	0x400060bc, 0x00000044		@ GPIO_PF_MODEH; EBI WEN, REN

	.int	0x40006100, 0x00000004		@ GPIO_EXTIPSELL: select port E for irq 0
	.int	0x4000610c, 0x00000001		@ GPIO_EXTIFALL: trigger for falling FPGA irq line
	.int	0x4000611c, 0x0000ffff		@ ? GPIO_IFC: clear all irqs
	.int	0x40006110, 0x00000001		@ GPIO_IEN: enable irq 0

	/* EBI */
	.int	0x40008000, 0x4f00d051		@ EBI_CTRL, enable ITS, mode0=mode2=mode3=D16A16ALE, bl0-3, noidle[023]
	/* EBI PSRAM */
	.int	0x40008028, 0x10000000		@ EBI_ADDRTIMING2; HALFALE
	.int	0x4000802c, 0x20000400		@ EBI_RDTIMING2; Prefetch, StrobeCycles = 4, HoldCycles = SetupCycles = 0
	.int	0x40008030, 0x00000200		@ EBI_WRTIMING2; StrobeCycles = 2, HoldCycles = SetupCycles = 0
	.int	0x40008034, 0x00000008		@ EBI_POLARITY2, ARDY_, ALE, WE_, RE_, CS_, BL_

	/* Board Control FPGA */
	.int	0x40008004, 0x10000303		@ EBI_ADDRTIMING; HALFALE, HoldCycles = SetupCycles = 3
	.int	0x40008008, 0x00030703		@ EBI_RDTIMING; StrobeCycles = 7, HoldCycles = SetupCycles = 3
	.int	0x4000800c, 0x00030703		@ EBI_WRTIMING; StrobeCycles = 7, HoldCycles = SetupCycles = 3
	.int	0x40008010, 0x00000008		@ EBI_POLARITY, ARDY_, ALE, WE_, RE_, CS_, BL_

	/* external NOR flash */
	.int	0x40008038, 0x10000000		@ EBI_ADDRTIMING3; HALFALE, HoldCycles = SetupCycles = 0
	.int	0x4000803c, 0x00000700		@ EBI_RDTIMING3; StrobeCycles = 7, HoldCycles = SetupCycles = 0
	.int	0x40008040, 0x00000200		@ EBI_WRTIMING3; StrobeCycles = 2, HoldCycles = SetupCycles =0
	.int	0x40008044, 0x00000008		@ EBI_POLARITY3, ARDY_, ALE, WE_, RE_, CS_, BL_

	.int	0x40008014, 0x105e00bb		@ EBI_ROUTE
	.int	0x40008000, 0x4f00dd51		@ EBI_CTRL, enable ITS, mode0=mode2=mode3=D16A16ALE, bl0-3, noidle[023], bank[023]en

	.int	UARTBASE + 0x00, 0x00000000		@ UART1_CTRL
	.int	UARTBASE + 0x04, 0x00001005		@ UART1_FRAME
	.int	UARTBASE + 0x14, 0x00001900		@ UART1_CLKDIV
	.int	UARTBASE + 0x0c, 0x00000c04		@ UART1_CMD
	.int	UARTBASE + 0x54, 0x00000003 + (UARTLOCATION << 8)		@ UART1_ROUTE
	.int	0x400c8024, 0x00000002		@ CMU_CMD = HFCLKSEL_HFXO

reginit_end:
	.size reginit, . - reginit

	.align	3
	/* register value pairs to initialize the board controller */
	.type bcinit, %object
bcinit:
	.short	0x0018, 0x1300			@ enable UART mux and ETH
	.short	0x0014, 0x0001			@ /
	.short	0x001a, 0x0001			@ ETH

bcinit_end:
	.size bcinit, . - bcinit

	.align 3
#ifdef EFM32_USE_OF
	.type dtb, %object
dtb:
	.incbin "arch/arm/boot/dts/efm32gg-dk3750.dtb"
dtbend:
	.size dtb, . - dtb
	.align 3

#else /* ifdef CONFIG_OF */

	.type ataglist, %object
ataglist:
	/* ATAG_CORE */
	.int	0x00000005		/* .size */
	.int	0x54410001		/* .tag = ATAG_CORE */
	.int	0x00000001		/* .flags */
	.int	0x00001000		/* .pagesize */
	.int	0x000000ff		/* .rootdev */
	/* ATAG_MEM */
	.int	0x00000004		/* .size */
	.int	0x54410002		/* .tag = ATAG_MEM */
	.int	0x00400000		/* .size = 4 MiB */
	.int	0x16000000		/* .start = SRAM_BASE */
	/* ATAG_CMDLINE */
cmdline:
	.int	(cmdline_end - cmdline) >> 2	/* .size */
	.int	0x54410009		/* .tag = ATAG_CMDLINE */
	.asciz	"console=ttyefm4,115200 ignore_loglevel ihash_entries=64 dhash_entries=64 rootfstype=romfs init=/linuxrc uclinux.physaddr=0x18000000 root=/dev/mtdblock0 earlyprintk"
	.align	2, 0
cmdline_end:
	/* ATAG_NONE */
	.int	0x00000000	/* .size */
	.int	0x00000000	/* .tag = ATAG_NONE */
ataglist_end:
	.size ataglist, . - ataglist
#endif /* ifdef CONFIG_OF / else */
