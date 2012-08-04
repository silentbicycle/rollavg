PROGS = rollavg

CFLAGS += -O2 -std=c99 -Wall -Werror -pedantic

all: ${PROGS}
rollavg: rollavg.c

clean:
	rm -f ${PROGS} *.o *.core
