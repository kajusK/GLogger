PROJECT = logger
BUILD_DIR = bin

SOURCES = ../../sources
MX_PROJECT = $(PROJECT).ioc
BOARD_GPIO_FILE = $(SOURCES)/app/$(PROJECT)/board_gpio.h
# files to build
FILES = $(wildcard $(SOURCES)/app/$(PROJECT)/*.c) \
	 $(wildcard $(SOURCES)/hal/*.c) \
	 $(wildcard $(SOURCES)/drivers/*.c) \
	 $(wildcard $(SOURCES)/utils/*.c) \
	 $(wildcard $(SOURCES)/modules/*.c) \
	 $(wildcard $(SOURCES)/modules/cgui/*.c)
# strip sources path from files to build to allow building when sources contains ..
CFILES = $(subst $(SOURCES)/, , $(FILES))

DEVICE=stm32f070cbt6

CFLAGS += -Wall -Wextra -Wstrict-prototypes -Wundef -Wshadow -Wredundant-decls
CFLAGS += -Wmissing-prototypes -pedantic --specs=nano.specs --specs=nosys.specs
CSTD = -std=gnu11
OPT = -O0

VPATH += $(SOURCES)
INCLUDES += $(patsubst %,-I%, . $(SOURCES)) -I$(SOURCES)/app/$(PROJECT)
OPENCM3_DIR = $(SOURCES)/external/libopencm3
MX_PATH = /opt/stm32cubemx
MX2BOARD = python ../../../tools/mx2board.py

include $(OPENCM3_DIR)/mk/genlink-config.mk
LDFLAGS += -lc
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

CODECHECK	:= cppcheck
CODECHECK_FLAGS := --enable=warning,performance -j 4 \
    $(addprefix -I, $(INCLUDES)) \
    --template=gcc --suppress=missingIncludeSystem \
    --suppress=unusedFunction --suppress=unmatchedSuppression \
    --suppress=missingInclude --suppress=va_list_usedBeforeStarted \
    --platform=unix32 -q
CODECHECK_FILES	:= $(FILES) \
    $(shell find $(SOURCES) -name '*.h' | grep -v $(SOURCES)/external | grep -v $(SOURCES)/tests)

codecheck:
	@$(CODECHECK) $(CODECHECK_FLAGS) $(CODECHECK_FILES)

.PHONY: codecheck

