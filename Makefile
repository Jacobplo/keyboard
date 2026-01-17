CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m3 -mthumb -MMD -MP
LDFLAGS ?= -Tlink.ld --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = $(wildcard src/*.c)
INCLUDE = -Ilib/cmsis-device-f1/Include -Ilib/CMSIS_5/CMSIS/Core/Include
DEFINE  = -DSTM32F103xB

BUILD_DIR := build
OBJ := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))


.PHONY: default
default: build


-include $(OBJ:.o=.d)


# Files
$(BUILD_DIR)/%.o: src/%.c
	arm-none-eabi-gcc $(CFLAGS) $(INCLUDE) $(DEFINE) -c $< -o $@

$(BUILD_DIR)/startup.o:startup.s
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/firmware.elf: $(OBJ) $(BUILD_DIR)/startup.o link.ld
	arm-none-eabi-gcc $(OBJ) $(BUILD_DIR)/startup.o $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(DEFINE) -o $@

$(BUILD_DIR)/firmware.bin: $(BUILD_DIR)/firmware.elf
	arm-none-eabi-objcopy -O binary $< $@


# PHONYs
.PHONY: build
build: $(BUILD_DIR)/firmware.bin

.PHONY: flash
flash: $(BUILD_DIR)/firmware.bin
	st-flash --reset write $(BUILD_DIR)/firmware.bin 0x8000000


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*
