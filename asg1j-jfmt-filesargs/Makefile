# Brian Lin bjlin
# $Id: Makefile,v 1.2 2013-09-24 17:12:34-07 - - $

JAVASRC    = jfmt.java
SOURCES    = ${JAVASRC} Makefile README
MAINCLASS  = jfmt
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES}
JARFILE    = jfmt
SUBMITDIR  = cmps012b-wm.f13 asg1
LISTING    = Listing.ps

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class : %.java
	cid + $<
	javac $<
	checksource $<

clean :
	- rm ${JARCLASSES} test.output

spotless : clean
	- rm ${JARFILE}

ci : ${SOURCES}
	cid + ${SOURCES} test.input
	checksource ${SOURCES}

lis : all
	${JARFILE} test.input >test.output
	mkpspdf ${LISTING} ${SOURCES} test.input test.output

submit : ${SOURCES} ci
	submit ${SUBMITDIR} ${SOURCES}

again : ${SOURCES}
	gmake --no-print-directory spotless lis
	
