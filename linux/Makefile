CFLAGS = -std=c99 -Wall -Wextra -pedantic -fPIC -g
CFLAGS_LIBB = -nostdlib -ffreestanding \
	-Wno-incompatible-library-redeclaration \
	-Wno-builtin-declaration-mismatch

PROG = bcause
SRC = compiler.c  list.c  main.c
LIB = libb.a

.PHONY: all install clean

all: ${PROG} ${LIB}

install: all
	install ${LIB} /usr/local/lib/${LIB}
	install -m 557 ${PROG} ${HOME}/bin/${PROG}

clean:
	rm -f *.o *.a *.out ${PROG}

${PROG}: ${SRC}
	${CC} ${CFLAGS} ${SRC} -o $@

${LIB}: libb.c
	${CC} ${CFLAGS} ${CFLAGS_LIBB} -c $<
	ar r $@ libb.o
	ranlib $@
