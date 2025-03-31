
CFLAGS      = -O -Werror
NO_WARNINGS = -Wno-multichar \
              -Wno-implicit-int \
              -Wno-implicit-function-declaration \
              -Wno-unused-result \
              -Wno-return-type \
              -Wno-parentheses

all:    b hello.s b.s test1.s

b:      b-proto.c
	gcc $(CFLAGS) $(NO_WARNINGS) $< -o $@

clean:
	rm -f b hello.s b.s

hello.s: b hello.b
	./b hello.b hello.s

b.s:    b b-compiler.b
	./b b-compiler.b b.s

test1.s: b test1.b
	./b test1.b test1.s
