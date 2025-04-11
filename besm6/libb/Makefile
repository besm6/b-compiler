B_SOURCES =
# TODO: lchar.b printn.b printf.b

B_ASM = $(patsubst %.b,%.madlen,$(B_SOURCES))

ASM_SOURCES = \
        b_ret.madlen \
        b_save0.madlen \
        b_save.madlen \
        char.madlen \
        exit.madlen \
        flush.madlen \
        writeb.madlen \
        write.madlen \

.PHONY: all install clean test

all:    libb.obj

install:

clean:
	rm -f *.obj *.s *.nm *.dis *.trace *.bin *.dub *.lst $(B_ASM)

libb.obj: $(ASM_SOURCES) $(B_ASM)
	besmc -c $(ASM_SOURCES) $(B_ASM) -o $@

%.madlen: %.b
	../bbesm < $< > $@
