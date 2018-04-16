
#CROSS_COMPILE = arm-none-eabi-
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

CFLAGS := -Wall -Os
CFLAGS += -I $(shell pwd)/include  -I $(shell pwd)/src

LDFLAGS := -lpthread -lrt
export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := logm

obj-y += test/
obj-y += src/



all : 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.log")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(shell find -name "*.log")
	rm -f $(TARGET)
	