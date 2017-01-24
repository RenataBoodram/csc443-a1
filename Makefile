all : write_blocks_seq 

CC = gcc

OSTYPE = $(shell uname)

ifeq ($(OSTYPE),CYGWIN_NT-5.1)
ARCH = -D_CYGWIN_
endif

ifeq ($(OSTYPE),Linux)
ARCH = -D_LINUX_
SOCK = -lnsl -lresolv
endif

CFLAGS = -g -Wall #-ansi -D_DEBUG_ -D_GNU_SOURCE $(ARCH)

HDRS = part1.h

part1 : write_blocks_seq.c write_blocks_seq.o part1.h 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o part1
