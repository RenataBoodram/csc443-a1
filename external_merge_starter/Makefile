SOURCES = main distribution

all : utils $(SOURCES)

CC = gcc
CFLAGS = -g -O3 -Wall 
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

main : $(OBJS) utils.o merge_external.o
	$(CC) $(CFLAGS) $@.o utils.o merge_external.o -o disk_sort 

distribution : $(OBJS) utils.o merge_external.o
	$(CC) $(CFLAGS) $@.o utils.o merge_external.o -o distribution 

clean:
	rm -f *.o $(SOURCES) 
