#
# A more complicated makefile
#

PATH := $(DEVKITARM)/bin:$(PATH)

# --- Project details -------------------------------------------------

PROJ    := gba_input_test
TARGET  := $(PROJ)

SRC_DIRS := .
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS := $(addsuffix .o, $(notdir $(SRCS)))

# --- Build defines ---------------------------------------------------

PREFIX  := arm-none-eabi-
CC      := $(PREFIX)gcc
CXX      := $(PREFIX)g++
LD      := $(PREFIX)g++
OBJCOPY := $(PREFIX)objcopy

ARCH    := -mthumb-interwork -mthumb
SPECS   := -specs=gba.specs

CFLAGS  := $(ARCH) -O2 -Wall -fno-strict-aliasing -I ../morpheus -I $(DEVKITPRO)/libtonc/include
LDFLAGS := $(ARCH) $(SPECS) -L $(DEVKITPRO)/libtonc/lib -L ../morpheus -lmorpheus -ltonc


.PHONY : build clean

# --- Build -----------------------------------------------------------
# Build process starts here!
build: $(TARGET).gba

# Strip and fix header (step 3,4)
$(TARGET).gba : $(TARGET).elf
	$(OBJCOPY) -v -O binary $< $@
	-@gbafix $@

# Link (step 2)
$(TARGET).elf : $(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@

# Compile (step 1)
$(OBJS) : %.c.o : %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJS) : %.cpp.o : %.cpp
	$(CXX) -c $< $(CFLAGS) -o $@

# --- Clean -----------------------------------------------------------

clean :
	@rm -fv *.gba
	@rm -fv *.elf
	@rm -fv *.o

#EOF
