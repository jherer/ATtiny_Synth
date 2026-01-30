BUILD ?= HW
MODULES = system services drivers hal platform sim

# SOURCES
SRC_DIRS = $(MODULES:%=src/%)
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
SRCS += src/examples/app_timer1_ctc.c # ADD APP SOURCESs

# INCLUDES
INC_DIRS = $(MODULES:%=src/%/include) include
INCS = $(foreach dir, $(INC_DIRS), $(wildcard $(dir)/*.h))
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS =  $(OBJS:%.o=%.d)


# Tell the makefile these are command names, not files
.PHONY: build run clean

F_CPU	= 16000000UL
INCFLAGS = $(foreach dir, $(INC_DIRS), -I$(dir))

 # compiler flags
CFLAGS	= \
$(INCFLAGS) \
-DF_CPU=$(F_CPU) \
-Wall \
-Wno-unused-function \
-Wno-unused-variable \
-Os \
-std=gnu11 \
-MMD -MP \

LDFLAGS =  # linker flags

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
BUILD_DIR 	= build/mcu
TARGET 		= $(BUILD_DIR)/firmware
SRCS += src/main.c

# Toolchain
OBJCOPY	= avr-objcopy
PORT	= COM6	# COM port may change
MCU		= attiny85
ISP		= stk500v1
BAUD	= 19200
# avrdude.config file path
CONF	= C:\Users\joshu\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf
CC		= avr-gcc

CFLAGS += -mmcu=$(MCU) # add HW compiler flags
LDFLAGS	= -mmcu=$(MCU) # add HW linker flags
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

run: $(TARGET).hex build
	avrdude $(AVRDUDE_FLAGS) -U flash:w:'$<':a

build: $(TARGET).hex

endif


# ----------------------------
#	SIMULATION BUILD
# ----------------------------
ifeq ($(BUILD), SIM)
CFLAGS += -DSIM
BUILD_DIR = build/sim
TARGET = $(BUILD_DIR)/sim.exe
MODULES += sim
SRCS += src/sim_main.c
CC = C:/mingw64/bin/gcc.exe # Compiler

build: $(TARGET)
run: $(TARGET)
	$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
endif


$(BUILD_DIR)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	$(RD) $(BUILD_DIR)



