PROG    = b7 bc7
CFLAGS  = -O -Wall -Werror -Wno-multichar

.PHONY: all install clean test

all:    ${PROG} bc.s #b.s

install:

clean:
	rm -f ${PROG} b.s *.nm *.dis *.trace

test:   build
	make -C build btest
	make -C build test

build:
	cmake -B build tests

#
# Build B compiler from B source
#
b7:     b.b
	bcause $< -o $@
	nm -n $@ > b.nm
	objdump -S $@ > b.dis

#
# Build B compiler from C source
#
bc7:    b.c
	$(CC) $(CFLAGS) $< -o $@

bc.s:   bc7 b.b
	./bc7 < b.b > $@

b.s:    b7 b.b
	./b7 < b.b > $@
