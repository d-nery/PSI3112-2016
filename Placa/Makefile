# Escola Politecnica da Universidade de Sao Paulo
# PSI312 - Laboratorio de Circuitos Eletricos - 2016
#
# Multimetro Digital Microcontrolado
#
# Turmas 7 e 8 - Grupo 1
# 	Alexandre Henrique Costa Rossi   9348950
# 	Arthur Gandelman                 9349176
# 	Arthur Kenji Furuko Sakai        9349217
# 	Daniel Nery Silva de Oliveira    9349051
# 	Mateus Almeida Barbosa           9349072
# 	Rubens Funabashi                 9348939

# Adaptado de https://github.com/AndSk/mbed_gcc_makefile/tree/feat/support_all_mbed_libraries



# File: Makefile for all mbed supported platforms (ARM GCC)
#
# Copyright (c) 10.2013, 0xc0170
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LINKER_NAME     = MKL25Z4
STARTUP_NAME    = startup_MKL25Z4
SYSTEM_NAME     = system_MKL25Z4
TARGET_BOARD    = TARGET_KL25Z
TARGET_VENDOR   = TARGET_Freescale
TARGET_FAMILY   = TARGET_KLXX
TARGET_SPECIFIC = TARGET_KL25Z
#TARGET_POST_BUILD_CHECK = ./frdm_kl25z_check_lock.py
CPU             = cortex-m0plus

# path to the mbed library
MBED_PATH = .

#Enabled/Disable mbed libraries
USE_DSP  = false
USE_FAT  = false
USE_RTOS = true
USE_USB  = false

# app headers directories
INC_DIRS = include

# app source directories
SRC_DIRS = src

# toolchain specific
TOOLCHAIN = arm-none-eabi-
CC        = $(TOOLCHAIN)gcc
CXX       = $(TOOLCHAIN)g++
AS        = $(TOOLCHAIN)gcc -x assembler-with-cpp
LD        = $(TOOLCHAIN)gcc
OBJCP     = $(TOOLCHAIN)objcopy
AR        = $(TOOLCHAIN)ar

# application specific
TARGET           = multimetro
INSTRUCTION_MODE = thumb
TARGET_EXT       = elf

LD_SCRIPT  = $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/$(LINKER_NAME).ld
CC_SYMBOLS = -D$(TARGET_BOARD) -DTOOLCHAIN_GCC_ARM -DNDEBUG

LIB_DIRS = $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM
LIBS     = -lmbed

ifeq ($(USE_DSP), true)
	LIBS += -ldsp -lcmsis_dsp
	LIB_DIRS += $(MBED_PATH)/dsp/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM
endif

ifeq ($(USE_FAT), true)
	LIBS += -lfat
	LIB_DIRS += $(MBED_PATH)/fat/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM
endif

ifeq ($(USE_RTOS), true)
	LIBS += -lrtos -lrtx
	LIB_DIRS += $(MBED_PATH)/rtos/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM
endif

ifeq ($(USE_USB), true)
	LIBS += -lUSBDevice
	LIB_DIRS += $(MBED_PATH)/usb/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM
endif

LIBS += -lstdc++ -lsupc++ -lm -lgcc -lc -lnosys

MBED_OBJ  = $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/cmsis_nvic.o
MBED_OBJ += $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/$(STARTUP_NAME).o
MBED_OBJ += $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/retarget.o
MBED_OBJ += $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/$(SYSTEM_NAME).o
MBED_OBJ += $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/board.o

ifneq ("$(wildcard $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/mbed_overrides.o)","")
	MBED_OBJ += $(MBED_PATH)/mbed/$(TARGET_BOARD)/TOOLCHAIN_GCC_ARM/mbed_overrides.o
endif

# MBED directories
INC_DIRS += $(MBED_PATH)/mbed
INC_DIRS += $(MBED_PATH)/mbed/$(TARGET_BOARD)
INC_DIRS += $(MBED_PATH)/mbed/$(TARGET_BOARD)/$(TARGET_VENDOR)/$(TARGET_FAMILY)
INC_DIRS += $(MBED_PATH)/mbed/$(TARGET_BOARD)/$(TARGET_VENDOR)/$(TARGET_FAMILY)/$(TARGET_SPECIFIC)

ifeq ($(USE_DSP), true)
	INC_DIRS += $(MBED_PATH)/dsp
endif

ifeq ($(USE_FAT), true)
	INC_DIRS += $(MBED_PATH)/fat
        INC_DIRS += $(MBED_PATH)/fat/ChaN
endif

ifeq ($(USE_RTOS), true)
		INC_DIRS += $(MBED_PATH)/rtos/TARGET_CORTEX_M
        INC_DIRS += $(MBED_PATH)/rtos
endif

ifeq ($(USE_USB), true)
        INC_DIRS += $(MBED_PATH)/usb/USBAudio
        INC_DIRS += $(MBED_PATH)/usb/USBDevice
        INC_DIRS += $(MBED_PATH)/usb/USBHID
        INC_DIRS += $(MBED_PATH)/usb/USBMIDI
        INC_DIRS += $(MBED_PATH)/usb/USBMSD
        INC_DIRS += $(MBED_PATH)/usb/USBSerial
endif

OUT_DIR = build

INC_DIRS_F = -I. $(patsubst %, -I%, $(INC_DIRS))
LIB_DIRS_F = $(patsubst %, -L%, $(LIB_DIRS))

ifeq ($(strip $(OUT_DIR)), )
	OBJ_FOLDER =
else
	OBJ_FOLDER = $(strip $(OUT_DIR))/
endif

COMPILER_OPTIONS  = -g -ggdb -Os -Wall -fno-strict-aliasing -fno-rtti
COMPILER_OPTIONS += -ffunction-sections -fdata-sections -fno-exceptions -fno-delete-null-pointer-checks
COMPILER_OPTIONS += -fmessage-length=0 -fno-builtin -m$(INSTRUCTION_MODE)
COMPILER_OPTIONS += -mcpu=$(CPU) -MMD -MP $(CC_SYMBOLS)

DEPEND_OPTS = -MF $(OBJ_FOLDER)$(@F:.o=.d)

# Flags
CFLAGS   = $(COMPILER_OPTIONS) $(DEPEND_OPTS) $(INC_DIRS_F) -std=gnu99 -c
CXXFLAGS = $(COMPILER_OPTIONS) $(DEPEND_OPTS) $(INC_DIRS_F) -std=gnu++11 -c
ASFLAGS  = $(COMPILER_OPTIONS) $(INC_DIRS_F) -c

# Linker options
LD_OPTIONS  = -mcpu=$(CPU) -m$(INSTRUCTION_MODE) -Os $(LIB_DIRS_F) -T $(LD_SCRIPT) $(INC_DIRS_F)
LD_OPTIONS += -specs=nano.specs
#use this if %f is used, by default it's commented
LD_OPTIONS += -u _printf_float -u _scanf_float
LD_OPTIONS += -Wl,-Map=$(OBJ_FOLDER)$(TARGET).map,--gc-sections

OBJCPFLAGS  = -O ihex

ARFLAGS = cr

RM = rm -rf

USER_OBJS =
C_SRCS    =
S_SRCS    =
C_OBJS    =
S_OBJS    =

# All source/object files inside SRC_DIRS
C_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
C_OBJS := $(patsubst %.c,$(OBJ_FOLDER)%.o,$(notdir $(C_SRCS)))

CPP_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
CPP_OBJS := $(patsubst %.cpp,$(OBJ_FOLDER)%.o,$(notdir $(CPP_SRCS)))

S_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))
S_OBJS := $(patsubst %.s,$(OBJ_FOLDER)%.o,$(notdir $(S_SRCS)))

VPATH := $(SRC_DIRS)

$(OBJ_FOLDER)%.o : %.c
	@echo 'Building file: $(@F)'
	@echo 'Invoking: MCU C Compiler'
	$(CC) $(CFLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '

$(OBJ_FOLDER)%.o : %.cpp
	@echo 'Building file: $(@F)'
	@echo 'Invoking: MCU C++ Compiler'
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '

$(OBJ_FOLDER)%.o : %.s
	@echo 'Building file: $(@F)'
	@echo 'Invoking: MCU Assembler'
	$(AS) $(ASFLAGS) $< -o $@
	@echo 'Finished building: $(@F)'
	@echo ' '

all: create_outputdir $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) print_info # post_build_check

create_outputdir:
	$(shell mkdir $(OBJ_FOLDER) 2>/dev/null)

# Tool invocations
$(OBJ_FOLDER)$(TARGET).$(TARGET_EXT): $(LD_SCRIPT) $(C_OBJS) $(CPP_OBJS) $(S_OBJS) $(MBED_OBJ)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	$(LD) $(LD_OPTIONS) $(CPP_OBJS) $(C_OBJS) $(S_OBJS) $(MBED_OBJ) $(LIBS) -o $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	@echo 'Removing entire out directory'
	$(RM) $(TARGET).$(TARGET_EXT) $(TARGET).bin $(TARGET).map $(OBJ_FOLDER)*.* $(OBJ_FOLDER)
	@echo ' '

print_info:
	@echo 'Printing size'
	$(TOOLCHAIN)size --totals $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT)
	$(TOOLCHAIN)objcopy -O srec $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) $(OBJ_FOLDER)$(TARGET).s19
	$(TOOLCHAIN)objcopy -O binary -v $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) $(OBJ_FOLDER)$(TARGET).bin
	$(TOOLCHAIN)objdump -D $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) > $(OBJ_FOLDER)$(TARGET).lst
	$(TOOLCHAIN)nm $(OBJ_FOLDER)$(TARGET).$(TARGET_EXT) > $(OBJ_FOLDER)$(TARGET)-symbol-table.txt

# post_build_check:
	# $(TARGET_POST_BUILD_CHECK) $(OBJ_FOLDER)$(TARGET).bin

check-syntax: check-syntax-c check-syntax-cpp

check-syntax-c:
	$(CC) $(CFLAGS) -o /dev/null -S $(CHK_SOURCES)

check-syntax-cpp:
	$(CXX) $(CXXFLAGS) -o /dev/null -S $(CHK_SOURCES)

install:
	@echo "Installing binary to /run/media/dnery/MBED/"
	@echo "Change path in makefile"
	@cp $(OBJ_FOLDER)$(TARGET).bin /run/media/dnery/MBED/

.PHONY: all clean print_info check-syntax install
