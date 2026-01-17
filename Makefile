CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m3 -mthumb -MMD -MP
LDFLAGS ?= -Tlink.ld --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = $(wildcard src/*.c)
INCLUDE = -Ilib/cmsis-device-f1/Include -Ilib/CMSIS_5/CMSIS/Core/Include
DEFINE  = -DSTM32F103xB

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
DEP_DIR := $(BUILD_DIR)/deps

OBJ  := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS := $(patsubst src/%.c,$(DEP_DIR)/%.d,$(SOURCES))


.PHONY: default
default: build

# Include dependency files if they exist
-include $(DEPS)


# Build object files and dependency files (.o and .d)
$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(OBJ_DIR) $(DEP_DIR)
	arm-none-eabi-gcc $(CFLAGS) $(INCLUDE) $(DEFINE) -c $< -o $@ -MF $(DEP_DIR)/$*.d

$(OBJ_DIR)/startup.o:startup.s
	mkdir -p $(OBJ_DIR)
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@


# Build flashable firmware
$(BUILD_DIR)/firmware.elf: $(OBJ) $(OBJ_DIR)/startup.o link.ld
	arm-none-eabi-gcc $(OBJ) $(OBJ_DIR)/startup.o $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(DEFINE) -o $@

$(BUILD_DIR)/firmware.bin: $(BUILD_DIR)/firmware.elf
	arm-none-eabi-objcopy -O binary $< $@


.PHONY: build
build: $(BUILD_DIR)/firmware.bin

.PHONY: flash
flash: $(BUILD_DIR)/firmware.bin
	st-flash --reset write $(BUILD_DIR)/firmware.bin 0x8000000

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
