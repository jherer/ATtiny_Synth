# Build mode: hardware (HW) or simulation (SIM)
BUILD ?= HW # Hardware build by default

# Names of modules used as source and include directories
MODULES = system services drivers hal platform

# Source file paths
SRC_DIRS = $(MODULES:%=src/%)
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
SRCS += $(wildcard src/app/*.c) # ADD APP SOURCES

# Include directory paths
INC_DIRS = include

# Build directory in case build not specified
BUILD_DIR = build

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:%.o=%.d)

# Tell the makefile these are command names, not files
.PHONY: build run clean


# Compiler flags
F_CPU	= 16000000UL
INCFLAGS = $(foreach dir, $(INC_DIRS), -I$(dir))

CFLAGS	= \
$(INCFLAGS) \
-DF_CPU=$(F_CPU) \
-Wall \
-Wno-unused-function \
-Wno-unused-variable \
-Os \
-std=gnu11 \
-MMD -MP \

LDFLAGS =  # Linker flags


# Cross-compatible remove directory command
ifeq ($(OS),Windows_NT)
	RD = powershell.exe -Command "Remove-Item -Recurse" -Path
else
    RD = rm -rf
endif


# ----------------------------
#	HARDWARE BUILD
# ----------------------------
ifeq ($(BUILD), HW)
CFLAGS += -DHW
BUILD_DIR 	= build/hardware
TARGET 		= $(BUILD_DIR)/firmware
SRCS += src/main_hw.c # Add hardware main to sources

# Toolchain
OBJCOPY	= avr-objcopy
PORT	= COM6	# COM port may change based on USB driver setup or which USB port you use
MCU		= attiny85 # Microcontroller
ISP		= stk500v1 # In-circuit serial programmer method
BAUD	= 19200 # Baudrate

# avrdude.config file path
CONF	= C:\Users\joshu\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf
CC		= avr-gcc # Compiler command

CFLAGS += -mmcu=$(MCU) # Add HW compiler flags
LDFLAGS	= -mmcu=$(MCU) # Add HW linker flags

 # avrdude flags
AVRDUDE_FLAGS = -p$(MCU) \
-P$(PORT) \
-b$(BAUD) \
-c$(ISP) \
-C$(CONF)

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Flash firmware to microcontroller
run: $(TARGET).hex build
	avrdude $(AVRDUDE_FLAGS) -U flash:w:'$<':a

build: $(TARGET).hex

clean:
	$(RD) $(BUILD_DIR)/*

endif


# ----------------------------
#	SIMULATION BUILD
# ----------------------------
ifeq ($(BUILD), SIM)
CFLAGS += -DSIM
BUILD_DIR = build/sim
TARGET = $(BUILD_DIR)/sim.exe
MODULES += sim
SRCS += src/main_sim.c # Add simulation main to sources
CC = C:/mingw64/bin/gcc.exe # Compiler

build: $(TARGET)

run: $(TARGET)
	$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	$(RD) $(BUILD_DIR)/*

endif


$(BUILD_DIR)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)


