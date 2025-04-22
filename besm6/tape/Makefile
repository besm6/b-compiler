TAPE    = b.7

all:    tape.bin
	cp $< $(TAPE)

clean:
	rm -f $(TAPE) *.bin source.txt *.lst

tape.bin: mktape.dub source.txt libb.bin
	rm -f $@
	dubna mktape.dub > tape.lst

source.txt: ../bbesm ../b-besm.b ../drum.b
	../bbesm < ../b-besm.b > $@
	../bbesm < ../drum.b >> $@

../bbesm:
	make -C .. all

libb.bin: ../libb/libb.obj
	ln -s $< $@

../libb/libb.obj:
	make -C ../libb all
