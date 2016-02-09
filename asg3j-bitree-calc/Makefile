# Brian Lin bjlin
# $Id: Makefile,v 1.4 2013-10-17 15:31:01-07 - - $

JAVASRC    = bitreecalc.java bitree.java symbol_table.java \
linked_stack.java auxlib.java
SOURCES    = ${JAVASRC} Makefile README
ALLSOURCES = ${SOURCES} pbitreecalc.perl
MAINCLASS  = bitreecalc
CLASSES    = ${JAVASRC:.java=.class}
JARCLASSES = ${CLASSES} linked_stack\$$*.class
JARFILE    = bitreecalc
LISTING    = Listing.ps
SUBMITDIR  = cmps012b-wm.f13 asg3

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
