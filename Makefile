CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m3 -mthumb $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = $(wildcard src/*.c)
INCLUDE = -Ilib/cmsis-device-f1/Include -Ilib/CMSIS_5/CMSIS/Core/Include
DEFINE  = -DSTM32F103xB

default: clean flash

build: firmware.bin

firmware.elf: $(SOURCES) link.ld
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(DEFINE) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

clean:
	rm -f firmware.*
