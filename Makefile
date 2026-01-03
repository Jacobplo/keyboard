CLFAGS ?= -Wall -mcpu-cortex-m3
LDFLAGS ?= -T link.ld -nostdlib

SOURCES = src/main.c

build: firmware.bin

firmware.elf: $(SOURCES) link.ld
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm -f firmware.*
