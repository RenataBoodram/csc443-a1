all : utils write_blocks_seq write_lines read_ram_seq write_blocks_rand write_ram_rand

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

SOURCES = write_blocks_seq.c write_lines.c write_blocks_rand.c
HDRS = part1.h

utils : $(SOURCES) utils.c
	$(CC) -c $(CFLAGS) $^

write_blocks_seq : write_blocks_seq.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

write_lines : write_lines.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

read_ram_seq : read_ram_seq.c read_ram_seq.o read_ram_seq.h
	$(CC) $(CFLAGS) $< -o $@

write_blocks_rand : write_blocks_rand.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

write_ram_rand : write_ram_rand.o utils.o 
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o write_blocks_seq write_lines read_ram_seq write_blocks_rand write_ram_rand
