PROG    = bcause
SRC     = compiler.c list.c main.c
LIB     = libb.a
CFLAGS  = -O -g -Wall -Wextra -Wshadow

.PHONY: all install clean

all: ${PROG} ${LIB}

install: all
	install ${LIB} /usr/local/lib/${LIB}
	install -m 555 ${PROG} /usr/local/bin/${PROG}

clean:
	rm -f *.o *.a *.out ${PROG}
	rm -rf build

test:   build
	make -C build btest
	make -C build test

build:
	cmake -B build tests

${PROG}: ${SRC}
	${CC} ${CFLAGS} ${SRC} -o $@

${LIB}: libb.c
	${CC} ${CFLAGS} -ffreestanding -c $<
	ar r $@ libb.o
	ranlib $@
