# https://stackoverflow.com/a/30142139


# basename for binary
BASEFN = beanbros
# Final binary
BIN = $(BASEFN).nex
# Put all auto generated stuff to this build dir.
BLDIR = ./build
# Path to CSPECT emulator
CSPECT = $(HOME)/z88dk/cspect/CSpect.exe

CXX = clang++
CXX_FLAGS = -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow

# compile options
C_OPTS := +zxn -clib=new -O3 -Wall
# C_OPTS := +zxn -clib=new -v -O3 --list
# C_OPTS := +zxn -v -O3 -compiler=sdcc

# link options
L_OPTS := +zxn -subtype=nex -O3 -m -s -clib=new -startup=31 -create-app
# L_OPTS := +zxn -subtype=nex -v -O3 -m -s -clib=new -startup=31 -create-app
# L_OPTS := +zxn -subtype=nex -v -O3 -m -s -startup=31 -create-app

# List of all .c source files.
C_SRCS := $(wildcard *.c)
# List of all .cpp source files.
ASM_SRCS := $(wildcard *.asm)

# All .c.o files go to build dir.
C_OBJS := $(C_SRCS:%.c=$(BLDIR)/%.c.o)
# All .c.o files go to build dir.
ASM_OBJS := $(ASM_SRCS:%.asm=$(BLDIR)/%.asm.o)

# Build target for every single object file.
$(BLDIR)/%.c.o : %.c
	zcc $(C_OPTS) -c $< -o $@

$(BLDIR)/%.asm.o : %.asm
	zcc $(C_OPTS) -c $< -o $@

# Actual target of the binary - depends on all .o files.
$(BLDIR)/$(BIN) : $(C_OBJS) $(ASM_OBJS)
	zcc $(L_OPTS) $(C_OBJS) $(ASM_OBJS) -o $(BLDIR)/$(BASEFN)

all : $(BLDIR)/$(BIN)
	@du -b $(BLDIR)/*.bin $(BLDIR)/*.nex

re : clean all

run : all
	$(CSPECT) -w2 -vsync -zxnext -mmc=./assets/ $(BLDIR)/$(BIN)

rerun : clean run

.PHONY : clean
clean :
#	rm -f $(BLDIR)/$(BIN) $(BLDIR)/$(BASEFN).* $(C_OBJS) $(ASM_OBJS)
	rm -f $(BLDIR)/*.o $(BLDIR)/$(BASEFN)*

cleanasm :
	rm -f $(BLDIR)/*.asm.o

