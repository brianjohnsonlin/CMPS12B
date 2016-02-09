# Brian Lin bjlin
# $Id: Makefile,v 1.4 2013-10-17 15:31:01-07 - - $

JAVASRC    = jxref.java listmap.java intqueue.java misc.java
SOURCES    = ${JAVASRC} Makefile README
ALLSOURCES = ${SOURCES} pxref.perl
MAINCLASS  = jxref
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES} intqueue\$$*.class listmap\$$*.class
JARFILE    = jxref
LISTING    = Listing.ps
SUBMITDIR  = cmps012b-wm.f13 asg2
OUTPUT = tests.output
TESTFILES = test0.in test1.in test2.in test3.in test4.in test5.in

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm -vf Manifest
	chmod +x ${JARFILE}

%.class : %.java
	- checksource $<
	- cid + $<
	javac $<

clean :
	- rm -vf ${JARCLASSES} Manifest

spotless : clean
	- rm -vf ${JARFILE}

ci : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	cid + ${ALLSOURCES}

submit : ${SOURCES}
	submit ${SUBMITDIR} ${SOURCES}

again : ${ALLSOURCES}
	gmake --no-print-directory spotless ci all lis

test: ${MAINCLASS}
	${JARFILE} ${TESTFILES} >${OUTPUT}
	
