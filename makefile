CC := avr-gcc
PORT ?= COM6
MCU := attiny85
TARGET := attiny85
#FREQ_CPU := 16000000LU
ISP := stk500v1
CONF = C:\Users\joshu\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf
BAUD = 19200

SRC_DIRS = src/hal src/drivers src/services src/app
INCLUDES = -Iinclude/hal -Iinclude/drivers -Iinclude/services
BUILD = build

# Gather all .c files recursively
SOURCES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
# Convert source files to object files in BUILD folder (flat)
OBJECTS := $(patsubst %.c, $(BUILD)/%.o, $(notdir $(SOURCES)))

CFLAGS := $(INCLUDES) -Os -mmcu=$(MCU)# -DF_CPU=$(FREQ_CPU))

# Detect OS
ifeq ($(OS),Windows_NT)
    MKDIR = if not exist $(BUILD) mkdir $(BUILD)
    RM = rmdir /s /q $(BUILD)
else
    MKDIR = mkdir -p $(BUILD)
    RM = rm -rf $(BUILD)
endif

all: $(BUILD)/main.hex
	@echo Flashing to port $(PORT)
	avrdude -v -C$(CONF) -c$(ISP) -p$(TARGET) -P$(PORT) -b$(BAUD) -U flash:w:'$(BUILD)/main.hex':a 

$(BUILD)/main.hex: $(BUILD)/main.elf
	avr-objcopy -O ihex $< $@

$(BUILD)/main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Compile each source file to an object in BUILD
$(BUILD)/%.o: $(BUILD)
	$(CC) $(CFLAGS) -c $(firstword $(filter %/$*.c,$(SOURCES))) -o $@

# Create build directory if it doesn't exist
$(BUILD):
	$(MKDIR)

clean:
	$(RM)

.PHONY: all flash clean