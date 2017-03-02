SOURCES = write_blocks_seq #write_lines read_blocks_seq read_ram_seq read_blocks_rand read_ram_rand write_blocks_rand write_ram_rand

all : utils $(SOURCES)

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

SOURCECODE = $(patsubst %, %.c, $(SOURCES)) 
OBJS = $(patsubst %, %.o, $(SOURCES))
HDRS = part1.h

utils : $(SOURCECODE) utils.c
	$(CC) -c $(CFLAGS) $^

$(SOURCES) : $(OBJS) utils.o
	$(CC) $(CFLAGS) $@.o utils.o -o $@ 

clean:
	rm -f *.o $(SOURCES) 
