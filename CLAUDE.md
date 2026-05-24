# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This repository contains two B programming language compilers:

1. **`intel64/`** — *BCause*: A B compiler targeting x86_64 Linux and macOS, written in ~2000 lines of C99. Produces statically linked binaries via `as` + `ld` (GNU on Linux, Apple on macOS).
2. **`besm6/`** — A B compiler targeting the BESM-6 Soviet mainframe, written in B itself. Bootstrapped via a KOI-7 variant of BCause.

The `pdp7/` directory holds a historical reference implementation. The `examples/` directory has sample B programs.

## Build Commands

### intel64 (BCause)

```sh
cd intel64
make          # builds bcause and libb.a
make install  # installs to /usr/local
make clean
```

### intel64 Tests

Tests use GoogleTest (fetched via CMake FetchContent):

```sh
cd intel64
make test           # build and run all tests
make build          # only configure CMake (cmake -B build tests)
make -C build btest # only build test binary
make -C build test  # only run tests
```

Run a single test (from inside `intel64/`):

```sh
cmake -B build tests
make -C build btest
./build/btest --gtest_filter=<TestSuiteName>.<TestName>
```

### besm6

```sh
cd besm6
make          # builds bbesm (B compiler for BESM-6) and libb
make test     # build and run tests
make clean
```

Requires external packages [`dubna`](https://github.com/besm6/dubna) and [`besmc`](https://github.com/besm6/besmc).

## Architecture

### intel64 — Compiler Pipeline

The compiler is a **single-pass, hand-written recursive descent parser** that directly emits AT&T-syntax x86_64 assembly via `fprintf()` calls. There is no AST.

Entry point: `main.c` → parses CLI flags, fills `struct compiler_args`, calls `compile()`.

**`compiler.c`** is the entire compiler:
- `declarations()` — top-level loop; dispatches to `function()`, `vector()`, or `global()`
- `function()` — emits function prologue/epilogue; calls `arguments()` + `statement()`
- `statement()` — handles all control flow (`if`, `while`, `switch`, `case`, `goto`, `return`, `auto`, `extrn`)
- `expression()` — Pratt-style level-based precedence; calls `term()` for primaries
- `term()` — parses unary operations and primary expressions (literals, identifiers, parentheses)
- `postfix()` — handles `[]`, `()`, `++`, `--` after a primary

After all declarations are parsed, `strings()` emits the `.rodata` section.

The compile pipeline after code gen: invokes `as` to assemble the `.s` file, then `ld` (static) to link against `libb.a`. On Linux the GNU assembler/linker are used (`-nostdlib`); on macOS the Apple toolchain is used with `-e _start`.

**`compiler_args`** (defined in `compiler.h`) is the central state struct passed everywhere:
- `locals` / `extrns` — `struct list` dynamic arrays tracking variables in scope
- `stack_offset` — tracks current local variable stack depth
- `strings` — `struct list` accumulating string literal data

**`list.c` / `list.h`** — simple generic dynamic array (`list_push`, `list_clear`, `list_free`).

**`libb.c`** — B standard library (`_start`, `read`, `write`, `writeb`, `printf`, `printd`, `printo`, `char`, `lchar`, `nread`, `nwrite`, `flush`, `exit`). Implemented via raw syscalls (Linux or macOS BSD) using inline assembly — no libc dependency. Macros `B_TYPE` and `B_FN` allow customization.

### KOI-7 Variant

Compiling with `-DKOI7` enables the `ascii_to_koi7()` function that translates ASCII characters to KOI-7 encoding. This is used to bootstrap `bbesm`: the `besm6/Makefile` first builds `bcause-koi7` (the x86_64 compiler with KOI-7 output) from the `intel64/` sources, then uses it to compile `b-besm.b`.

### besm6 — B Compiler Written in B

`b-besm.b` is the BESM-6 compiler source. It follows BESM-6 calling conventions (documented in `doc/Besm6-Calling-Conventions.md`):
- Arguments pushed to stack in order; last argument in accumulator
- r14 = negative count of arguments, r13 = return address
- `b/save` / `b/ret` routines manage frame setup/teardown

`besm6/libb/` contains the runtime library: hand-written Madlen assembly (`.madlen`) for the calling convention helpers (`b/save`, `b/ret`), arithmetic and relational operators (`b/mul`, `b/div`, comparisons, etc.), and character access (`char`, `lchar`); plus B-language source for `printf`, `read`, `writeb`, `flush`, etc. (compiled to `.madlen` by `bbesm` at build time). See `besm6/libb/README.md` for a function index and `doc/B-Runtime-Library.md` for a comprehensive reference.

## B Language Notes

B uses `*` as the escape character (not `\`):
- `*n` = newline, `*t` = tab, `*e` or `*0` = NUL, `*(` = `{`, `*)` = `}`
- Character literals: `'ab'` (multi-char, big-endian packed into one word)
- String literals: `"hello"`

Compound assignments use `=OP` syntax: `=+`, `=-`, `=*`, `=/`, `=%`, `=<<`, `=>>`, `=<`, `=<=`, `=>`, `=>=`, `=!=`, `===`, `=&`, `=|`.

Local variables: `auto x;` or `auto v[10];`  
External references: `extrn printf;`  
Leading `0` in integer literals means octal.

The grammar is formally documented in `doc/grammar.asdl`.
