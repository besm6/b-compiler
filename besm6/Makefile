PROG    = bbesm
CFLAGS  = -O -Wall -Werror -Wno-multichar

.PHONY: all install clean test

all:    $(PROG)
	make -C libb $@

install: $(PROG)
	install -m 555 ${PROG} /usr/local/bin/${PROG}

clean:
	make -C libb $@
	rm -f ${PROG} bcause-koi7 *.o *.a *.s *.nm *.dis *.trace

test:   all build
	make -C build btest
	ctest -j1 --test-dir build

build:
	cmake -B build tests

#
# Build B compiler from B source
#
bbesm:  b-besm.b bcause-koi7 libb.a
	./bcause-koi7 b-besm.b -L. -o $@

#
# Build B compiler for x86_64
#
BCAUSE_SRC = ../intel64/compiler.c ../intel64/list.c ../intel64/main.c
bcause-koi7: $(BCAUSE_SRC)
	$(CC) $(CFLAGS) -DKOI7 $(BCAUSE_SRC) -o $@

#
# Special version of B library for BESM-6.
# Functions read() converts from UTF-8 to KOI-7.
# Functions writeb() converts from KOI-7 to UTF-8.
#
libb.a: libb.c
	${CC} ${CFLAGS} -ffreestanding -c $<
	ar r $@ libb.o
	ranlib $@
