
ASSN = 4
CLASS= cs143
CLASSDIR= ../..
LIB= -L/usr/pubsw/lib -lfl 
AR= gar
ARCHIVE_NEW= -cr
RANLIB= gar -qs

SRC= semant.cc semant.h cool-tree.h cool-tree.handcode.h good.cl bad.cl README
CSRC= semant-phase.cc symtab_example.cc  handle_flags.cc  ast-lex.cc ast-parse.cc utilities.cc stringtab.cc dumptype.cc tree.cc cool-tree.cc
TSRC= mycoolc mysemant cool-tree.aps
CGEN=
HGEN=
LIBS= lexer parser cgen
CFIL= semant.cc ${CSRC} ${CGEN}
LSRC= Makefile
OBJS= ${CFIL:.cc=.o}
OUTPUT= good.output bad.output


CPPINCLUDE= -I. -I${CLASSDIR}/include/PA${ASSN} -I${CLASSDIR}/src/PA${ASSN}

FFLAGS = -d8 -ocool-lex.cc
BFLAGS = -d -v -y -b cool --debug -p cool_yy
ASTBFLAGS = -d -v -y -b ast --debug -p ast_yy

CC=g++
CFLAGS=-g -Wall -Wno-unused -Wno-write-strings -Wno-deprecated ${CPPINCLUDE} -DDEBUG
FLEX=flex ${FFLAGS}
BISON= bison ${BFLAGS}
DEPEND = ${CC} -MM ${CPPINCLUDE}

source: ${SRC} ${TSRC} ${LIBS} lsource

lsource: ${LSRC}

${OUTPUT}: semant
	@rm -f ${OUTPUT}
	./mysemant good.cl >good.output 2>&1 
	-./mysemant bad.cl >bad.output 2>&1 

compile:	semant change-prot

change-prot:
	@-chmod 660 ${SRC} ${OUTPUT}

SEMANT_OBJS := ${filter-out symtab_example.o,${OBJS}}

semant:  ${SEMANT_OBJS} lexer parser cgen
	${CC} ${CFLAGS} ${SEMANT_OBJS} ${LIB} -o semant

symtab_example: symtab_example.cc 
	${CC} ${CFLAGS} symtab_example.cc ${LIB} -o symtab_example

.cc.o:
	${CC} ${CFLAGS} -c $<

dotest:	semant good.cl bad.cl
	@echo "\nRunning semantic checker on good.cl\n"
	-./mysemant good.cl
	@echo "\nRunning semantic checker on bad.cl\n"
	-./mysemant bad.cl

${LIBS}:
	${CLASSDIR}/etc/link-object ${ASSN} $@

${TSRC} ${CSRC}:
	-ln -s ${CLASSDIR}/src/PA${ASSN}/$@ $@

${HSRC}:
	-ln -s ${CLASSDIR}/include/PA${ASSN}/$@ $@

clean :
	-rm -f ${OUTPUT} *.s core ${OBJS} semant cgen symtab_example parser lexer *~ *.a *.o

clean-compile:
	@-rm -f core ${OBJS} ${LSRC}

%.d: %.cc ${SRC}
	${SHELL} -ec '${DEPEND} $< | sed '\''s/\($*\.o\)[ :]*/\1 $@ : /g'\'' > $@'

-include ${CFIL:.cc=.d}

