SOURCES = q1 q2#main distribution

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

utils : $(SOURCECODE) utils.c merge_external.c disk_sort.c distribution.c q1.c q2.c
	$(CC) -c $(CFLAGS) $^

q1 : q1.o utils.o merge_external.o disk_sort.o
	$(CC) $(CFLAGS) $@.o utils.o merge_external.o disk_sort.o -o q1 

q2 : q2.o utils.o merge_external.o disk_sort.o 
	$(CC) $(CFLAGS) $@.o utils.o merge_external.o disk_sort.o distribution.o -o q2

clean:
	rm -f *.o $(SOURCES)
