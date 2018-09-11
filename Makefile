
#CROSS_COMPILE = arm-poky-linux-gnueabi-
#CROSS_CFLAGS = -march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=/opt/sdk/262/sysroots/cortexa9hf-neon-poky-linux-gnueabi
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

# Do not print "Entering directory ..."
MAKEFLAGS += --no-print-directory

TARGET := logm
TOOLTARGET := toolsync

CFLAGS := -Wall -Os $(CROSS_CFLAGS)
CFLAGS += -I $(shell pwd)/include  -I $(shell pwd)/src

LDFLAGS := -lpthread -lrt -Wl,-Map,$(TARGET).map
TOOLLDFLAGS := -lpthread -lrt

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

obj-y += test/
obj-y += src/
TOOLPATH := $(TOPDIR)/tool


all : tools
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(CFLAGS) -o $(TARGET) built-in.o $(LDFLAGS) 

toolsync :$(TOOLPATH)/logm_unlock.c
	$(CC) $(CFLAGS) $^ -o $(TOOLPATH)/$@  $(TOOLLDFLAGS)

tools : toolsync

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.log")
	rm -f $(TARGET)
	rm -f $(TOOLPATH)/$(TOOLTARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(shell find -name "*.log")
	rm -f $(TARGET)
	rm -f $(TOOLPATH)/$(TOOLTARGET)