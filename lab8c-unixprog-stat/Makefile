# Brian Lin bjlin
# $Id: Makefile,v 1.5 2013-05-01 13:21:00-07 - - $

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = ci clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu99
MKDEPS    = gcc -MM
GRIND     = valgrind --leak-check=full

CSOURCE   = l8stat.c
OBJECTS   = ${CSOURCE:.c=.o}
EXECBIN   = l8stat
SOURCES   = ${CSOURCE} ${MKFILE}
LISTSRC   = ${SOURCES} ${DEPSFILE}
OUTPUT    = output*.lis

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

%.o : %.c
	cid + $<
	${GCC} -c $<

ci : ${SOURCES}
	cid + ${SOURCES} test*.data

lis : ${SOURCES} test
	mkpspdf ${LISTSRC} ${OUTPUT}

clean :
	- rm ${OBJECTS} ${DEPSFILE} core ${OUTPUT}

spotless : clean
	- rm ${EXECBIN}

test : ${EXECBIN}
	- runprogram.perl -x output1.lis -0test1.data ${EXECBIN}
	- runprogram.perl -x output2.lis ${EXECBIN} test*.data
	- ${GRIND} ${EXECBIN} <test1.data >output3.lis 2>&1

deps : ${CSOURCE}
	@ echo "# ${DEPSFILE} created `date`" >${DEPSFILE}
	${MKDEPS} ${CSOURCE} | sort | uniq >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${MAKE} --no-print-directory deps


again :
	gmake spotless deps ci all lis

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif

