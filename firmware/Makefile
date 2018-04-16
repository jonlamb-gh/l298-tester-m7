###
# GNU ARM Embedded Toolchain
TOOLCHAIN=/opt/gcc-arm-none-eabi-6-2017-q2-update
TOOLCHAIN_BIN=$(TOOLCHAIN)/bin

CC=$(TOOLCHAIN_BIN)/arm-none-eabi-gcc
LD=$(TOOLCHAIN_BIN)/arm-none-eabi-ld
AR=$(TOOLCHAIN_BIN)/arm-none-eabi-ar
AS=$(TOOLCHAIN_BIN)/arm-none-eabi-as
CP=$(TOOLCHAIN_BIN)/arm-none-eabi-objcopy
OD=$(TOOLCHAIN_BIN)/arm-none-eabi-objdump
SIZE=$(TOOLCHAIN_BIN)/arm-none-eabi-size
GDB=$(TOOLCHAIN_BIN)/arm-none-eabi-gdb

###
# Directory Structure
BINDIR=bin
INCDIR=include
SRCDIR=src

###
# Find source files
ASOURCES=$(shell find -L $(SRCDIR) -name '*.s')
CSOURCES+=$(shell find -L $(SRCDIR) -name '*.c')

# Find header directories for the project (not the vendor packages)
INC=$(shell find -L . -path ./include/vendor -prune -o -path ./src/vendor -prune -o -name '*.h' -exec dirname {} \; | uniq)
INCLUDES:=$(INC:%=-I%)
# Add vendor include directories
INC=$(shell find -L ./include/vendor -name '*.h' -exec dirname {} \; | uniq)
INC+=$(shell find -L ./src/vendor -name '*.h' -exec dirname {} \; | uniq)
INCLUDES+=$(INC:%=-isystem%)

# Create object list
OBJECTS=$(ASOURCES:%.s=%.o)
OBJECTS+=$(CSOURCES:%.c=%.o)
# Define output files ELF & IHEX
BINELF=firmware.elf
BINHEX=firmware.hex

###
# MCU FLAGS
MCFLAGS=-mcpu=cortex-m7 -mthumb -mlittle-endian
MCFLAGS+=-mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb-interwork
# COMPILE FLAGS
DEFS=-DARM_MATH_CM7 -DUSE_HAL_DRIVER
DEFS+=-DSTM32F767xx -DSTM32F7XX -DSTM32F7_NUCLEO -DUSE_FULL_LL_DRIVER
CFLAGS=-c $(MCFLAGS)
CFLAGS+=-std=gnu99 -Wall -Wextra -Wimplicit-function-declaration -Wstrict-prototypes -Wundef -Wshadow
CFLAGS+=-Werror
CFLAGS+=$(DEFS) $(INCLUDES)
# LINKER FLAGS
LDSCRIPT_DEBUG=linker/stm32f7_link.ld
LDSCRIPT=linker/stm32f7_link.ld
LDFLAGS=-T $(LDSCRIPT) --specs=nosys.specs $(MCFLAGS)
LDFLAGS+=../vendor/STM32Cube_FW_F7_V1.8.0/Drivers/CMSIS/Lib/GCC/libarm_cortexM7lfsp_math.a
# print out memory usage
LDFLAGS+=-Wl,--print-memory-usage

###
# Optimizations
OPT?='O1 O2 O3 O4 O6 O7' # O5 disabled by default, because it breaks code

# only when running 'make release-memopt'
ifneq ($(findstring release-memopt,$(MAKECMDGOALS)),)
ifneq ($(filter O1,$(OPT)),)
CXXFLAGS+=-fno-exceptions # Uncomment to disable exception handling
DEFS+=-DNO_EXCEPTIONS # The source code has to comply with this rule
endif

ifneq ($(filter O2,$(OPT)),)
CFLAGS+=-Os # Optimize for size https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
CXXFLAGS+=-Os
LDFLAGS+=-Os # Optimize for size https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
endif

ifneq ($(filter O3,$(OPT)),)
CFLAGS+=-ffunction-sections -fdata-sections # Place each function or data item into its own section in the output file
CXXFLAGS+=-ffunction-sections -fdata-sections # -||-
LDFLAGS+=-Wl,-gc-sections # Remove isolated unused sections
endif

ifneq ($(filter O4,$(OPT)),)
CFLAGS+=-fno-builtin # Disable C++ exception handling
CXXFLAGS+=-fno-builtin # Disable C++ exception handling
endif

ifneq ($(filter O5,$(OPT)),)
CFLAGS+=-flto # Enable link time optimization
CXXFLAGS+=-flto # Enable link time optimization
LDFLAGS+=-flto # Enable link time optimization
endif

ifneq ($(filter O6,$(OPT)),)
CXXFLAGS+=-fno-rtti # Disable type introspection
endif

ifneq ($(findstring O7,$(OPT)),)
LDFLAGS+=--specs=nano.specs # Use size optimized newlib
endif
endif

###
# Build Rules
.PHONY: all release release-memopt debug clean flash st-semihost gdb

all: release

release: $(BINDIR)/$(BINHEX)

release-memopt: release

debug: CFLAGS+=-ggdb -DBUILD_TYPE_DEBUG
debug: LDFLAGS+=-ggdb
debug: LDSCRIPT=$(LDSCRIPT_DEBUG)
debug: release

$(BINDIR)/$(BINHEX): $(BINDIR)/$(BINELF)
	$(CP) -O ihex $< $@
	@echo "Objcopy from ELF to IHEX complete!\n"

$(BINDIR)/$(BINELF): $(OBJECTS)
	@mkdir -p bin
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	@echo "Linking complete!\n"
	$(SIZE) $(BINDIR)/$(BINELF)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

%.o: %.s
	$(CC) $(CFLAGS) $< -o $@
	@echo "Assambled "$<"!\n"

clean:
	rm -f $(OBJECTS) $(BINDIR)/$(BINELF) $(BINDIR)/$(BINHEX)

flash:
	st-flash --format ihex write $(BINDIR)/$(BINHEX)

st-semihost:
	st-util --semihosting

gdb:
	$(GDB) --command=gdb/gdbinit $(BINDIR)/$(BINELF)
