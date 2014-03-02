PATH:=$(HOME)/gcc-arm/bin:$(PATH)
OPTIMIZATION = s

# lpc810 / lpc811 / lpc812
MCU=lpc812

#########################################################################
PROJECT=template
LDSCRIPT=core/$(MCU).ld


#SRC=$(wildcard core/*.c) $(wildcard *.c *.cpp) $(wildcard adafruit/*.c adafruit/*.cpp)
SRC=$(wildcard core/*.c) main.cpp WProgram.cpp swm.c src/conways_life.cc src/random.cc $(wildcard adafruit/*.c adafruit/*.cpp)
CSRC=$(patsubst %.cc,%.c,$(patsubst %.cpp,%.c,$(SRC)))
#$(info $(CSRC))

OBJECTS=$(patsubst %,.bin/%,$(CSRC:.c=.o))
DEPS=$(patsubst %,.bin/%,$(CSRC:.c=.d))
LSTFILES=$(patsubst %,.bin/%,$(CSRC:.c=.lst))

CONLYFLAGS = -std=gnu99 -Wstrict-prototypes
CXXONLYFLAGS = -fno-exceptions -fno-rtti
#  Compiler Options
GCFLAGS = -mcpu=cortex-m0plus -mthumb -O$(OPTIMIZATION) -I. -Icore -Idrivers/inc -Iadafruit
# Warnings
GCFLAGS += -Wno-strict-aliasing -Wundef -Wall -Wextra -Wunreachable-code 
# Optimizazions
GCFLAGS +=  -fstrict-aliasing -fsingle-precision-constant -funsigned-char -funsigned-bitfields -fshort-enums -fno-builtin -ffunction-sections -fno-common -fdata-sections 
# Debug stuff
GCFLAGS += -Wa,-adhlns=.bin/$(<:.c=.lst) -g

#LDFLAGS = -T$(LDSCRIPT) -nostartfiles -lstdc++_s -lg_s -lsupc++_s -lnosys
#LDFLAGS  = -T$(LDSCRIPT) -nostartfiles  -Wl,--gc-section -lstdc++_s -lg_s -lsupc++_s -lnosys
LDFLAGS  = -T$(LDSCRIPT) -nostartfiles  -Wl,--gc-section

#  Compiler/Linker Paths
GCC = arm-none-eabi-gcc
GXX = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
REMOVE = rm -f
SIZE = arm-none-eabi-size

#########################################################################


all: .bin .bin/core firmware.bin stats

.bin:
	mkdir .bin

.bin/core .bin/adafruit .bin/src:
	mkdir $^

firmware.bin: .bin/$(PROJECT).elf Makefile
	@$(OBJCOPY) -R .stack -O binary .bin/$(PROJECT).elf firmware.bin

.bin/$(PROJECT).elf: $(OBJECTS) Makefile
	@echo -e "  \033[1;34mLD \033[0m (\033[1;33m$(OBJECTS)\033[0m) -> $(PROJECT).elf"
	@$(GXX) -o .bin/$(PROJECT).elf $(OBJECTS) $(GCFLAGS) $(LDFLAGS) 

stats: .bin/$(PROJECT).elf 
	@$(SIZE) .bin/$(PROJECT).elf

clean:
	@echo -e "  \033[1;34mCleanup\033[0m $<"
	@$(REMOVE) $(OBJECTS)
	@$(REMOVE) $(DEPS)
	@$(REMOVE) $(LSTFILES)
	@$(REMOVE) firmware.bin
	@$(REMOVE) .bin/$(PROJECT).elf
	@$(REMOVE) -d .bin/core
	@$(REMOVE) -d .bin

-include $(DEPS)

#########################################################################

.bin/%.o: %.c Makefile 
	@echo -e "  \033[1;34mGCC\033[0m $<"
	@$(GCC) $(GCFLAGS) $(CONLYFLAGS) -o $@ -c $<
	@$(GCC) $(GCFLAGS) $(CONLYFLAGS) -MM $< > $*.d.tmp
	@sed -e 's|.*:|.bin/$*.o:|' < $*.d.tmp > .bin/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> .bin/$*.d
	@rm -f $*.d.tmp

.bin/%.o: %.cpp Makefile 
	@echo -e "  \033[1;34mGXX\033[0m $<"
	@$(GXX) $(GCFLAGS) $(CXXONLYFLAGS) -o $@ -c $<
	@$(GXX) $(GCFLAGS) $(CXXONLYFLAGS) -MM $< > $*.d.tmp
	@sed -e 's|.*:|.bin/$*.o:|' < $*.d.tmp > .bin/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> .bin/$*.d
	@rm -f $*.d.tmp

.bin/%.o: %.cc Makefile 
	@echo -e "  \033[1;34mGXX\033[0m $<"
	@$(GXX) $(GCFLAGS) $(CXXONLYFLAGS) -o $@ -c $<
	@$(GXX) $(GCFLAGS) $(CXXONLYFLAGS) -MM $< > $*.d.tmp
	@sed -e 's|.*:|.bin/$*.o:|' < $*.d.tmp > .bin/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
		sed -e 's/^ *//' -e 's/$$/:/' >> .bin/$*.d
	@rm -f $*.d.tmp

#########################################################################

flash: all
	tools/isp.py
#	-/Applications/lpcxpresso_5.1.2_2065/lpcxpresso/bin/dfu-util -d 0x471:0xdf55 -c 0 -t 2048 -R -D /Applications/lpcxpresso_5.1.2_2065/lpcxpresso/bin/LPCXpressoWIN.enc
#	sleep 1
#	/Applications/lpcxpresso_5.1.2_2065/lpcxpresso/bin/crt_emu_cm3_gen -wire=winusb -pLPC812 -vendor=NXP -flash-load-exec=firmware.bin

debug: all
	-/Applications/lpcxpresso_5.1.2_2065/lpcxpresso/bin/dfu-util -d 0x471:0xdf55 -c 0 -t 2048 -R -D /Applications/lpcxpresso_5.1.2_2065/lpcxpresso/bin/LPCXpressoWIN.enc
	sleep 1
	arm-none-eabi-gdb --eval-command="target extended-remote |/Applications/lpcxpresso_5.1.2_2065/lpcxpresso/bin/crt_emu_cm3_gen -wire=winusb -pLPC812 -vendor=NXP" .bin/$(PROJECT).elf


.PHONY : clean all flash stats debug

