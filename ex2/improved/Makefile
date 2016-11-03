# Example Makefile
#
# Exercise 2, TDT4258

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
LINK=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Llib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
BINFLAGS= -r -b binary
OBJFLAGS= --rename-section .data=.rodata 
LINKERSCRIPT=lib/efm32gg.ld
DATAOBJ=ahh.o ahhBig.o apple.o pen.o applePen.o pinapple.o pinapplePen.o ppap.o PPAPfull.o

ex2.bin : ex2.elf
	${OBJCOPY} -O binary $< $@

${DATAOBJ}:
	${LINK} ${BINFLAGS} $*.raw -o $@
	${OBJCOPY} ${OBJFLAGS} $@ 

ex2.elf : ex2.o timer.o dac.o gpio.o interrupt_handlers.o ${DATAOBJ}
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}


%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

	
.PHONY : pretty
pretty :
	-indent *.c *.h

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "ex2.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex 
