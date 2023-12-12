CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	piantarepali.exe

piantarepali.exe: piantarepali.o DBGpthread.o
	gcc ${CFLAGS} -o piantarepali.exe piantarepali.o DBGpthread.o ${LIBRARIES}

piantarepali.o: piantarepali.c DBGpthread.h printerror.h
	gcc -c ${CFLAGS} piantarepali.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f piantarepali.o DBGpthread.o piantarepali.exe


