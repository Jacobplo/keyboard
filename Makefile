CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m3 -mthumb -MMD -MP
LDFLAGS ?= -Tlink.ld --specs=nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES = $(wildcard src/*.c) $(wildcard lib/tinyusb/src/*.c) $(wildcard lib/tinyusb/src/portable/st/stm32_fsdev/fsdev_common.c) \
					$(wildcard lib/tinyusb/src/portable/st/stm32_fsdev/dcd_stm32_fsdev.c) $(wildcard lib/tinyusb/src/device/*.c) \
					$(wildcard lib/tinyusb/src/class/hid/hid_device.c) $(wildcard lib/tinyusb/src/common/*.c)
INCLUDE = -Isrc/ -Ilib/cmsis-device-f1/Include -Ilib/CMSIS_5/CMSIS/Core/Include -Ilib/tinyusb/src \
					-Ilib/tinyusb/src/portable/st/stm32_fsdev -Ilib/tinyusb/src/device -Ilib/tinyusb/src/class/hid
DEFINE  = -DSTM32F103xB

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
DEP_DIR := $(BUILD_DIR)/deps

OBJ  := $(SOURCES:%.c=$(OBJ_DIR)/%.o)
DEPS := $(SOURCES:%.c=$(DEP_DIR)/%.d)


.PHONY: default
default: build

# Include dependency files if they exist
-include $(DEPS)


# Build object files and dependency files (.o and .d)
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@) $(dir $(DEP_DIR)/$*)
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
