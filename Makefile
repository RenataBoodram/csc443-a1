all : write_blocks_seq write_lines read_ram_seq

CC = gcc
CFLAGS = -O3 -Wall 
CFLAGS += -D_LARGEFILE_SOURCE
CFLAGS += -fno-exceptions
CFLAGS += -finline-functions
CFLAGS += -funroll-loops
CFLAGS += -D_FILE_OFFSET_BITS=64

OSTYPE = $(shell uname)

ifeq ($(OSTYPE),CYGWIN_NT-5.1)
ARCH = -D_CYGWIN_
endif

ifeq ($(OSTYPE),Linux)
ARCH = -D_LINUX_
SOCK = -lnsl -lresolv
endif

#CFLAGS = -g -Wall #-ansi -D_DEBUG_ -D_GNU_SOURCE $(ARCH)

HDRS = part1.h

write_blocks_seq : write_blocks_seq.c write_blocks_seq.o $(HDRS) 
	$(CC) $(CFLAGS) $< -o $@

write_lines : write_lines.c write_lines.o $(HDRS)
	$(CC) $(CFLAGS) $< -o $@

read_ram_seq : read_ram_seq.c read_ram_seq.o read_ram_seq.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o write_blocks_seq write_lines read_ram_seq
