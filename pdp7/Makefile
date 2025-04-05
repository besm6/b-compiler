PROG    = b7
SRC     = b.b

.PHONY: all install clean test

all:    ${PROG} #b.s

install:

clean:
	rm -f ${PROG} b.s *.nm *.dis *.trace

test:   build
	make -C build btest
	make -C build test

build:
	cmake -B build tests

${PROG}: ${SRC}
	bcause $< -o $@
	nm -n $@ > b.nm
	objdump -S $@ > b.dis

b.s:    ${PROG} ${SRC}
	./${PROG} < ${SRC} > $@
