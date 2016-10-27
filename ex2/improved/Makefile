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
OBJFLAGS= -I binary -O elf32-littlearm --rename-section .data=.rodata 
LINKERSCRIPT=lib/efm32gg.ld
RAWDATA=ahhh.raw ahhBig.raw ppap.raw apple.raw pinapple.raw pen.raw ApplePen.raw PinapplePen.raw PPAPfull.raw
DATAOBJ=ahhh.o ahhBig.o ppap.o apple.o pen.o ApplePen.o pinapple.o PinapplePen.o PPAPfull.o

ex2.bin : ex2.elf
	${OBJCOPY} -O binary $< $@

ex2.elf : ex2.o timer.o dac.o gpio.o interrupt_handlers.o ppap.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS}

#${DATAOBJ} : ${RAWDATA} 
#	${LINK} ${BINFLAGS} $< -o $@ 
#	${OBJCOPY} ${OBJFLAGS} $< $@
#%.o : %.raw
#	${LINK} ${BINFLAGS} -c $< -o $@

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
