################################################################################
# Makefile for the LPC1343 / eStick2 by Martin Horauer
#

SRC += main.c
SRC += 

################################################################################
# Sytem files
SYSPATH = Libraries/SYS/src
LIBRARYPATH = Libraries/Library/src
DRVPATH = Libraries/Driver/LPC17xx/src

INCPATH = -I"Libraries/SYS/inc"
INCPATH += -I"Libraries/Driver/LPC17xx/inc"
INCPATH += -I"Libraries/Library/inc"

BUILDPATH = build/

CSRC = $(SYSPATH)/core_cm3.c
CSRC += $(SYSPATH)/NVICInit.c
CSRC += $(SYSPATH)/system_LPC17xx.c
CSRC += $(SYSPATH)/libnosys_gnu.c

CSRC += $(DRVPATH)/debug_frmwrk.c
#CSRC += $(DRVPATH)/lpc17xx_gpdma.c
#CSRC += $(DRVPATH)/lpc17xx_pinsel.c
#CSRC += $(DRVPATH)/lpc17xx_systick.c
#CSRC += $(DRVPATH)/lpc17xx_adc.c
#CSRC += $(DRVPATH)/lpc17xx_gpio.c
#CSRC += $(DRVPATH)/lpc17xx_pwm.c
#CSRC += $(DRVPATH)/lpc17xx_timer.c
#CSRC += $(DRVPATH)/lpc17xx_can.c
#CSRC += $(DRVPATH)/lpc17xx_i2c.c
#CSRC += $(DRVPATH)/lpc17xx_qei.c
#CSRC += $(DRVPATH)/lpc17xx_uart.c
#CSRC += $(DRVPATH)/lpc17xx_clkpwr.c
#CSRC += $(DRVPATH)/lpc17xx_i2s.c
#CSRC += $(DRVPATH)/lpc17xx_rit.c
#CSRC += $(DRVPATH)/lpc17xx_wdt.c
#CSRC += $(DRVPATH)/lpc17xx_dac.c
#CSRC += $(DRVPATH)/lpc17xx_libcfg_default.c
#CSRC += $(DRVPATH)/lpc17xx_rtc.c
#CSRC += $(DRVPATH)/lpc17xx_emac.c
#CSRC += $(DRVPATH)/lpc17xx_mcpwm.c
#CSRC += $(DRVPATH)/lpc17xx_spi.c
#CSRC += $(DRVPATH)/lpc17xx_exti.c
CSRC += $(DRVPATH)/lpc17xx_nvic.c
#CSRC += $(DRVPATH)/lpc17xx_ssp.c
CSRC += $(DRVPATH)/ledDriver.c

CSRC += $(LIBRARYPATH)/timer.c
CSRC += $(LIBRARYPATH)/led.c
CSRC += $(LIBRARYPATH)/uart.c
CSRC += $(LIBRARYPATH)/circularbuffer.c
CSRC += $(LIBRARYPATH)/adc.c
CSRC += $(LIBRARYPATH)/dac.c
CSRC += $(LIBRARYPATH)/capcom.c
CSRC += $(LIBRARYPATH)/pwm.c

ASRC = $(SYSPATH)/startup_LPC17xx.asm

LDFILE = LPC1758.ld
LDPATH = Linker

################################################################################
# definitions
TARGET = firmware
MCU = cortex-m3
FORMAT = binary

CROSS_COMPILE = arm-none-eabi-
AS = $(CROSS_COMPILE)gcc
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
FLASHTOOL = flash_lpc1343.sh

OPT = 0
DEBUG = dwarf-2

OPTFLAGS = -O$(OPT)
CFLAGS += -fmessage-length=0
CFLAGS += -Wall
CFLAGS += -Wa,-adhlns=$(<:%.c=%.o.lst)
CFLAGS += -g$(DEBUG)

ALL_CFLAGS = $(INCPATH) $(CFLAGS) $(OPTFLAGS) -mcpu=$(MCU) -mthumb -g3 
ALL_CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

ALL_AFLAGS = -x assembler-with-cpp -Wall -Wa,-adhlns="$(<:%.asm=%.o.lst)"
ALL_AFLAGS += -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" 
ALL_AFLAGS += -mcpu=$(MCU) -mthumb -g3 -g$(DEBUG)

LD_FLAGS = 

################################################################################
# Compile: create object files from C source files.

all: start $(TARGET).elf $(TARGET).hex $(TARGET).lst $(TARGET).siz crc end


start:
	@echo "--------------------------------------------------------------------"
	@echo "Starting Build process ..."
	@echo "--------------------------------------------------------------------"

end:
	@echo "--------------------------------------------------------------------"
	@echo "End of Build process ..."
	@echo "--------------------------------------------------------------------"

# Compile: create object files from C source files.
.IGNORE: %.asm
$(BUILDPATH)%.o : %.c
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Building file: $(@:%.o=%.c)"
	mkdir -p `dirname $@`
	#TMPNAME=`echo ${$(@:%.o=%.c)#*/}`
	$(CC) -c $(ALL_CFLAGS) -o $@ `echo "$(@:%.o=%.c)" | sed 's,^[^/]*/,,'`
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Compile: create object files from ASM source files.
.IGNORE: %.c
$(BUILDPATH)%.o : %.asm
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Building file: $(@:%.o=%.asm)"
	mkdir -p `dirname $@`
	$(CC) $(ALL_AFLAGS) -o $@ `echo "$(@:%.o=%.asm)" | sed 's,^[^/]*/,,'`
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
$(TARGET).elf : $(SRC:%.c=$(BUILDPATH)%.o) $(CSRC:%.c=$(BUILDPATH)%.o) $(ASRC:%.asm=$(BUILDPATH)%.o)
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Linking file ... $<"
	$(LD) -T"./$(LDPATH)/$(LDFILE)" -nostartfiles -L"./$(LDPATH)" $(LD_FLAGS) -Wl,-Map,$(TARGET).map -mcpu=$(MCU) -mthumb -g3 -g$(DEBUG) $^ -o $@
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""


# Create final output files (.hex, .eep) from ELF output file.
$(TARGET).hex: $(TARGET).elf
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Creating HEX file ... $@"
	$(OBJCOPY) -O $(FORMAT) $< $@
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Create extended listing file from ELF output file.
$(TARGET).lst: $(TARGET).elf
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Creating Listing ... $@"
	$(OBJDUMP) -h -S $< > $@
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Create size information
$(TARGET).siz: $(TARGET).elf
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Size output ... $<"
	$(SIZE) --format=berkely -x -t $<
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# Program the eStick2
debug:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo " Starting debug.                                "
	./run_debug.sh
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

# Calculate only the CRC
crc:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Patching the CRC of the HEX file ..."
	rm -f firmware.bin
	cp firmware.hex firmware.bin
	crc firmware.bin
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

run:
	@echo ""
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo "Flashing file to device ..."
	./run.sh
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""

# print some usage message
help:
	@echo "UNIX                                            "
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo "To build the project type:                      "
	@echo "make"
	@echo ""
	@echo "To flash the debugger type:"
	@echo "make debug"
	@echo ""
	@echo "To clean the project type:"
	@echo "make clean"
	@echo ""
	@echo "WINDOWS                                         "
	@echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
	@echo ""
	@echo "To build the project type:                      "
	@echo "cs-make"
	@echo ""
	@echo "To patch the HEX file type:"
	@echo "make crc"
	@echo ""
	@echo "To clean the project type:"
	@echo "make clean"
	@echo ""

.PHONY: all Makefile

ifdef SystemRoot
   RM = del /Q
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
   endif
endif


clean:
	#$(RM) *~ *.lst *.o *.d *.map 
	$(RM) $(TARGET).hex $(TARGET).elf #$(TARGET).bin
	#$(RM) $(SYSPATH)/*.o $(SYSPATH)/*.lst $(SYSPATH)/*.d
	#$(RM) $(LIBRARYPATH)/*.o $(LIBRARYPATH)/*.lst $(LIBRARYPATH)/*.d
	#$(RM) $(DRVPATH)/*.o $(DRVPATH)/*.lst $(DRVPATH)/*.d
	$(RM) -R $(BUILDPATH)*

