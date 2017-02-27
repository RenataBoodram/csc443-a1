#SOURCES = write_blocks_seq write_lines read_blocks_seq read_ram_seq read_blocks_rand read_ram_rand write_blocks_rand write_ram_rand
SOURCES = main

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

utils : $(SOURCECODE) utils.c merge_external.c
	$(CC) -c $(CFLAGS) $^

#merge_external : main.c merge_external.c
#	$(CC) -c $(CFLAGS) $^ 

$(SOURCES) : $(OBJS) utils.o merge_external.o
	$(CC) $(CFLAGS) $@.o utils.o merge_external.o -o disk_sort 

clean:
	rm -f *.o $(SOURCES) 
