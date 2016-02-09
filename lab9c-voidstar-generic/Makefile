# Brian Lin bjlin

MKFILE   = Makefile
GCC      = gcc -g -O0 -Wall -Wextra -std=gnu99
CSOURCES = numsort.c linesort.c inssort.c
CHEADER  = inssort.h
OBJECTS  = ${CSOURCES:.c=.o}
EXECBIN  = numsort linesort
SOURCES  = ${CSOURCES} ${CHEADER} ${MKFILE}
SUBMIT   = submit cmps012b-wm.f13 lab9

all : ${EXECBIN}

numsort: numsort.o inssort.o
	${GCC} numsort.o inssort.o -o numsort

linesort: linesort.o inssort.o
	${GCC} linesort.o inssort.o -o linesort

%.o : %.c
	cid + $<
	${GCC} -c $<

ci : ${SOURCES}
	cid + ${SOURCES}
	- checksource ${SOURCES}

clean :
	- rm ${OBJECTS}

spotless : clean
	- rm ${EXECBIN}

submit : ${SOURCES}
	${SUBMIT} ${SOURCES} README

again :
	gmake spotless ci all lis
