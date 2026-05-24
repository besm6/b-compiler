PROG    = bcause
SRC     = compiler.c list.c main.c
LIB     = libb.a
CFLAGS  = -O -g -Wall -Wextra -Wshadow

# On macOS the compiler inserts stack-protection stubs that reference
# __stack_chk_guard/__stack_chk_fail from libSystem.  Since B programs are
# linked statically without libSystem, disable SSP for the runtime library.
uname_s := $(shell uname -s)
ifeq ($(uname_s),Darwin)
LIBB_EXTRA = -fno-stack-protector
endif

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
	${CC} ${CFLAGS} -ffreestanding ${LIBB_EXTRA} -c $<
	ar r $@ libb.o
	ranlib $@
