B_SOURCES = printd.b printo.b printf.b read.b write.b writeb.b flush.b

B_ASM = $(patsubst %.b,%.madlen,$(B_SOURCES))

ASM_SOURCES = \
        b_div.madlen \
        b_eq.madlen \
        b_ge.madlen \
        b_gt.madlen \
        b_le.madlen \
        b_lt.madlen \
        b_mod.madlen \
        b_mul.madlen \
        b_ne.madlen \
        b_not.madlen \
        b_ret.madlen \
        b_save0.madlen \
        b_save.madlen \
        b_tout.madlen \
        b_true.madlen \
        char.madlen \
        exit.madlen \
        lchar.madlen

.PHONY: all install clean test

all:    libb.obj

install:

clean:
	rm -f *.obj *.s *.nm *.dis *.trace *.bin *.dub *.lst $(B_ASM)

libb.obj: $(ASM_SOURCES) $(B_ASM)
	besmc -c $(ASM_SOURCES) $(B_ASM) -o $@

%.madlen: %.b
	../bbesm < $< > $@
