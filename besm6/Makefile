PROG    = bbesm bcbesm
CFLAGS  = -O -Wall -Werror -Wno-multichar

.PHONY: all install clean test

all:    $(PROG)

install:

clean:
	rm -f ${PROG} *.s *.nm *.dis *.trace

test:   build
	make -C build btest
	ctest -j1 --test-dir build

build:
	cmake -B build tests

#
# Build B compiler from B source
#
bbesm:  b-besm.b
	bcause $< -o $@
	#nm -n $@ > b.nm
	#objdump -S $@ > b.dis

#
# Build B compiler from C source
#
bcbesm: b-proto.c
	$(CC) $(CFLAGS) $< -o $@
