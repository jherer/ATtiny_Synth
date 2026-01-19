# ----------------------------
#	TOOLCHAIN
# ----------------------------
CC		= avr-gcc
OBJCOPY	= avr-objcopy


# ----------------------------
#	MCU Settings
# ----------------------------
PORT	?= COM6	# COM port may change
MCU		= attiny85
F_CPU	= 16000000UL
ISP		= stk500v1
BAUD	= 19200

# avrdude.config file path
CONF	= C:\Users\joshu\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17/etc/avrdude.conf


# ----------------------------
#	Files
# ----------------------------
APP_SRC		= src/examples/app_soft_timer.c # CHANGE APP SOURCE HERE

MAIN_SRC		= src/main.c
SERVICES_SRC	= $(wildcard src/services/*.c)
DRIVERS_SRC 	= $(wildcard src/drivers/*.c) $(wildcard src/drivers/internal/*.c)
HAL_SRC			= $(wildcard src/hal/*.c)

APP_INCLUDE		= include
MAIN_INCLUDE	= include
SERVICES_INCLUDE= include
DRIVERS_INCLUDE	= include src/hal/include src/drivers/internal
HAL_INCLUDE		= include src/hal/include

BUILD_DIR 	= build
TARGET 		= $(BUILD_DIR)/firmware

SRC	= $(HAL_SRC) $(DRIVERS_SRC) $(SERVICES_SRC) $(MAIN_SRC) $(APP_SRC)
OBJ	= $(SRC:src/%.c=$(BUILD_DIR)/%.o)


# ----------------------------
#	Compiler Flags
# ----------------------------
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU)
CFLAGS	+= -Wall -Wno-unused-function -Wno-unused-variable
CFLAGS	+= -Os -std=gnu11

APP_CFLAGS		= $(CFLAGS) $(APP_INCLUDE:%=-I%)
MAIN_CFLAGS		= $(CFLAGS) $(MAIN_INCLUDE:%=-I%)
SERVICES_CFLAGS = $(CFLAGS) $(SERVICES_INCLUDE:%=-I%)
DRIVERS_CFLAGS	= $(CFLAGS) $(DRIVERS_INCLUDE:%=-I%)
HAL_CFLAGS		= $(CFLAGS) $(HAL_INCLUDE:%=-I%)

LDFLAGS	= -mmcu=$(MCU) # linker flags


# ------------------------------
# Build rules
# ------------------------------
.PHONY: all flash clean # Tell the makefile these are command names, not files

all: $(TARGET).hex

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Build app
$(BUILD_DIR)/app/%.o: src/app/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(APP_CFLAGS) -c $< -o $@

# Build main
$(BUILD_DIR)/%.o: src/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(MAIN_CFLAGS) -c $< -o $@

# Build services
$(BUILD_DIR)/services/%.o: src/services/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(SERVICES_CFLAGS) -c $< -o $@
	
# Build driver internals
$(BUILD_DIR)/drivers/internal/%.o: src/drivers/internal/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(DRIVERS_CFLAGS) -c $< -o $@
	
# Build drivers
$(BUILD_DIR)/drivers/%.o: src/drivers/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(DRIVERS_CFLAGS) -c $< -o $@
	
# Build HAL
$(BUILD_DIR)/hal/%.o: src/hal/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(HAL_CFLAGS) -c $< -o $@

flash:$(TARGET).hex
	avrdude -p $(MCU) -P $(PORT) -b $(BAUD) -c $(ISP) -U flash:w:'$<':a

clean:
	rmdir /S /Q build
