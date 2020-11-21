#
# A more complicated makefile
#

PATH := $(DEVKITARM)/bin:$(PATH)

# --- Project details -------------------------------------------------

PROJ    := libmorpheus
TARGET  := $(PROJ)

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(addsuffix .o, $(basename $(SRCS)))

# --- Build defines ---------------------------------------------------

PREFIX  := arm-none-eabi-
CC      := $(PREFIX)g++
AR      := $(PREFIX)ar
OBJCOPY := $(PREFIX)objcopy

ARCH    := -mthumb-interwork -mthumb
SPECS   := -specs=gba.specs

CFLAGS  := $(ARCH) -O2 -Wall -fno-strict-aliasing -I . -I $(DEVKITPRO)/libtonc/include
ARFLAGS := rvs


.PHONY : build clean

# --- Build -----------------------------------------------------------
# Build process starts here!
build: $(TARGET).a

# Strip and fix header (step 3,4)
#$(TARGET).gba : $(TARGET).elf
#	$(OBJCOPY) -v -O binary $< $@
#	-@gbafix $@

# Link (step 2)
$(TARGET).a : $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Compile (step 1)
$(OBJS) : %.o : %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

# --- Clean -----------------------------------------------------------

clean :
	@rm -fv *.gba
	@rm -fv *.elf
	@rm -fv *.o
	@rm -fv *.a

#EOF
