PROJECT = bl
BUILD_DIR = bin

SOURCES = ../../sources
MX_PROJECT = $(PROJECT).ioc
BOARD_GPIO_FILE = $(SOURCES)/app/board_gpio.h
# files to build
FILES = $(wildcard $(SOURCES)/app/$(PROJECT)/*.c) \
	$(SOURCES)/hal/flash.c \
	$(SOURCES)/hal/usb_msc.c \
	$(SOURCES)/utils/crc.c \
	$(SOURCES)/drivers/ramdisk.c \
	$(SOURCES)/modules/fw.c
# strip sources path from files to build to allow building when sources contains ..
CFILES = $(subst $(SOURCES)/, , $(FILES))

DEVICE=stm32f042f6p6

CFLAGS += -Wall -Wextra -Wstrict-prototypes -Wundef -Wshadow -Wredundant-decls
CFLAGS += -Wmissing-prototypes -pedantic --specs=nano.specs --specs=nosys.specs
CSTD = -std=gnu11
OPT = -O0

VPATH += $(SOURCES)
INCLUDES += $(patsubst %,-I%, . $(SOURCES)) -I $(SOURCES)/app/$(PROJECT)
OPENCM3_DIR = $(SOURCES)/external/libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
LDFLAGS += -lc
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
