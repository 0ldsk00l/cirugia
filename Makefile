CC = cc
CFLAGS = -Wall -g -Icore

OBJS = core/hash.o
OBJS += core/header.o
OBJS += core/rom.o

OBJS += cli/cirugia.o

all: cirugia

cirugia: $(OBJS)
	$(CC) -o cirugia $(OBJS)

clean:
	rm -f $(OBJS) cirugia
