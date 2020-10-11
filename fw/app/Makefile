###############################################################################
# Jakub Kaderka 2020
###############################################################################

#######################
# Project configuration
#######################
# resulting binary name
PROJECT = logger

FAMILY=STM32F0
# check libopencm3/ld/devices.data for proper values
# m0, m0plus, m3, m4, m7
CPU = cortex-m0
# STM32F0 to F2, L0 to L1, G0
FPU = -msoft-float
# STM32F3 to F4, L4
#FPU = -mfloat-abi=hard -mfpu=fpv4-sp-d16
# STM32F7
#FPU = -mfloat-abi=hard -mfpu=fpv5-sp-d16


# sources directory
SRCDIR = src
# path to AFW library
AFW = ../external/AFW
# path to a bootloader binary
BOOTLOADER = ../bl/logger-bl.bin

# linker script (generics in libopencm3/lib/stm32)
LDSCRIPT = $(PROJECT).ld
# STM32CubeMX project to generate board_gpio.h file
MX_PROJECT = $(PROJECT).ioc

# source files to include in build
SOURCES = $(wildcard $(SRCDIR)/*.c) \
	  $(wildcard $(SRCDIR)/*/*.c)

AFW_SOURCES = $(wildcard $(AFW)/sources/hal/*.c) \
	      $(AFW)/sources/drivers/gps.c \
	      $(AFW)/sources/drivers/spi_flash.c \
	      $(AFW)/sources/drivers/ssd1306.c \
	      $(AFW)/sources/modules/log.c \
	      $(AFW)/sources/modules/fw.c \
	      $(AFW)/sources/modules/nmea.c \
	      $(AFW)/sources/modules/ramdisk.c \
	      $(AFW)/sources/modules/msc.c \
	      $(AFW)/sources/modules/uf2.c \
	      $(wildcard $(AFW)/sources/modules/cgui/*.c) \
	      $(AFW)/sources/utils/crc.c \
	      $(AFW)/sources/utils/math.c \
	      $(AFW)/sources/utils/nav.c \
	      $(AFW)/sources/utils/button.c \
	      $(AFW)/sources/utils/ringbuf.c \
	      $(AFW)/sources/utils/time.c

INCLUDES = $(SRCDIR) \
	   $(AFW)/sources

#######################
# Directories and stuff
#######################
BUILD_DIR = bin
MX2BOARD = python $(AFW)/tools/mx2board.py
MX_PATH = /opt/stm32cubemx
BOARD_GPIO_FILE = $(SRCDIR)/board_gpio.h
CODECHECK = cppcheck
OPENCM3_DIR = $(AFW)/sources/external/libopencm3

CODECHECK_FLAGS = --enable=warning,performance -j 4 \
    $(addprefix -I, $(INCLUDES)) \
    --template=gcc --suppress=missingIncludeSystem \
    --suppress=unusedFunction --suppress=unmatchedSuppression \
    --suppress=missingInclude --suppress=va_list_usedBeforeStarted \
    --platform=unix32 -q

SOURCES += $(subst $(AFW)/,,$(AFW_SOURCES))
OBJS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

CODECHECK_FILES	= $(SOURCES) \
		  $(shell find $(INCLUDES) -name '*.h' | grep -v external | grep -v tests)

#######################
# Build configuration
#######################
CSTD = -std=gnu11
OPT = -O0
OPENCM3_LIB = opencm3_$(shell echo $(FAMILY) | tr '[:upper:]' '[:lower:]')
OPENCM3_LIB_PATH = $(OPENCM3_DIR)/lib/lib$(OPENCM3_LIB).a
ARCH_FLAGS = -mthumb $(FPU) -mcpu=$(CPU) -D$(FAMILY)
CFLAGS = -Wall -Wextra -Wstrict-prototypes -Wundef -Wshadow -Wredundant-decls \
	-Wno-missing-field-initializers -Wmissing-prototypes -pedantic \
	-fno-common -ffunction-sections -fdata-sections  \
	-Wimplicit-function-declaration  \
	--specs=nano.specs --specs=nosys.specs \
	$(CSTD) $(OPT) $(ARCH_FLAGS) $(addprefix -I, $(INCLUDES)) \
	-I $(OPENCM3_DIR)/include -g

LDFLAGS = -T$(LDSCRIPT) -L$(OPENCM3_DIR)/lib $(ARCH_FLAGS) -nostartfiles \
	  --specs=nano.specs -Wl,--gc-sections -Wl,-Map=$(PROJECT).map -lc
LDLIBS += -lopencm3_stm32f0 \
	  -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

PREFIX	= arm-none-eabi-
CC	= $(PREFIX)gcc
CXX	:= $(PREFIX)g++
LD	:= $(PREFIX)gcc
AR	:= $(PREFIX)ar
AS	:= $(PREFIX)as
OBJCOPY	:= $(PREFIX)objcopy
OBJDUMP	:= $(PREFIX)objdump
GDB	:= $(PREFIX)gdb
SIZE	:= $(PREFIX)size

#######################
# Build rules
#######################
all: codecheck $(OPENCM3_LIB_PATH) $(PROJECT).elf $(PROJECT)_debug.elf image uf2 size
image: $(PROJECT)_image.bin

size: $(PROJECT).elf
	$(SIZE) $<

$(OPENCM3_LIB_PATH):
	$(MAKE) -C $(AFW)/sources/external/libopencm3

# generate pinmux
$(BOARD_GPIO_FILE): $(MX_PROJECT)
	$(MX2BOARD) -p $(MX_PROJECT) -m $(MX_PATH) -o $(BOARD_GPIO_FILE)

# Need a special rule to have a bin dir
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MD -o $@ -c $<

$(BUILD_DIR)/%.o: $(AFW)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MD -o $@ -c $<

$(PROJECT).elf: $(BOARD_GPIO_FILE) $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin

# add fw header to image to allow flashing it directly next to bootloader
$(PROJECT)_debug.elf: $(PROJECT).elf $(PROJECT).bin $(BOOTLOADER)
	$(AFW)/tools/fw.py --hdr $(PROJECT).bin $(PROJECT)_hdr.bin
	cp $< $@
	$(OBJCOPY) --add-section .header=$(PROJECT)_hdr.bin --set-section-flags .header=alloc,load,data --change-section-address .header=0x08000800 $@ 2> /dev/null
	$(OBJCOPY) --add-section .header2=$(PROJECT)_hdr.bin --set-section-flags .header2=alloc,load,data --change-section-address .header2=0x8010400 $@ 2> /dev/null
	$(OBJCOPY) --add-section .bootloader=$(BOOTLOADER) --set-section-flags .bootloader=alloc,load,data --change-section-address .bootloader=0x08000000 $@ 2> /dev/null

$(PROJECT)_image.bin: $(PROJECT).bin $(BOOTLOADER)
	$(AFW)/tools/fw.py $< $@ --bl $(BOOTLOADER)

uf2: $(PROJECT).bin
	$(AFW)/tools/bin2uf2.py $(PROJECT).bin $(PROJECT).uf2

flash: $(PROJECT).elf
	st-flash write $< 0x8000000

clean:
	@rm -rf $(BUILD_DIR) *.elf *.map *.bin *.uf2

codecheck:
	@$(CODECHECK) $(CODECHECK_FLAGS) $(CODECHECK_FILES)

.PHONY: all clean flash codecheck image
-include $(OBJS:.o=.d)
