# Brian Lin bjlin

JAVASRC = exit255.java
MAINCLASS = exit255
CLASSES = exit255.class
JARFILE = exit255
JARCLASSES = ${CLASSES}
SOURCES = README Makefile symlink.info files.found grep.submit java.libs exit1.java mkexit1 privacy exit255.java quota.size
SUBMITDIR = cmps012b-wm.f13 lab2

all: ${JARFILE}

${JARFILE}: ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	- rm Manifest
	chmod +x ${JARFILE}

%.class: %.java
	cid + $<
	javac $<

test: ${JARFILE}
	(${JARFILE}; echo $$?) >255.output

remove: ${JARFILE}
	rm ${JARFILE}
	rm ${CLASSES}
	rm 255.output

submit: ${SOURCES}
	submit ${SUBMITDIR} ${SOURCES}
