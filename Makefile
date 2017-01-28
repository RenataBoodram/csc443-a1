all : write_blocks_seq write_lines read_ram_seq write_blocks_rand

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

#utils.o : utils.o $(HDRS)
#	$(CC) -c $(CFLAGS) $<

write_blocks_seq : utils.o write_blocks_seq.c write_blocks_seq.o $(HDRS) 
	$(CC) $(CFLAGS) $< -o $@

write_lines : utils.o write_lines.c write_lines.o $(HDRS)
	$(CC) $(CFLAGS) $< -o $@

read_ram_seq : read_ram_seq.c read_ram_seq.o read_ram_seq.h
	$(CC) $(CFLAGS) $< -o $@

write_blocks_rand : write_blocks_rand.c write_blocks_rand.o $(HDRS) 
	$(CC) $(CFLAGS) $< -o $@

utils.o : utils.c $(HDRS)
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o write_blocks_seq write_lines read_ram_seq write_blocks_rand
