# B Compiler Collection

This repository contains compilers for the **B programming language** — the direct predecessor to C, created by Ken Thompson and Dennis Ritchie at Bell Labs in 1969. Two compilers are provided:

* **BCause** (`intel64/`) — runs B programs on modern x86-64 Linux and macOS.
* **bbesm** (`besm6/`) — runs B programs on the BESM-6, a Soviet mainframe computer (via the [Dubna](https://github.com/besm6/dubna) emulator).

## What is the B language?

B is a simple, typeless language: every variable holds one machine-size word, with no distinction between integers and pointers. You declare local variables with `auto` and reference globals with `extrn`. Escape sequences use `*` instead of `\` (for example, `*n` is a newline and `*t` is a tab). B was the direct ancestor of C and a fascinating snapshot of early Unix history.

Good starting points for learning B:

* [A Tutorial Introduction to the Language B](doc/btut.pdf) by B. W. Kernighan
* [User's Reference to B](doc/kbman.pdf) by K. Thompson

## BCause — B for modern Linux (`intel64/`)

BCause is a small, single-pass compiler (~2000 lines of C99). It translates B source files to x86-64 assembly, then calls the system assembler (`as`) and linker (`ld`) to produce a standalone binary. On Linux the GNU toolchain is used; on macOS the Apple toolchain is used.

### Prerequisites

**Linux:** a standard C toolchain — `gcc` (or any C99 compiler), `make`, and `binutils` (`as` and `ld`). Available on any typical Linux distribution.

**macOS:** Xcode Command Line Tools, which provide `cc`, `make`, `as`, and Apple `ld`. Install with:

```sh
xcode-select --install
```

### Build

```sh
cd intel64
make
```

This produces the `bcause` compiler binary and `libb.a` (the B standard library).

To install globally:

```sh
sudo make install   # copies bcause to /usr/local/bin and libb.a to /usr/local/lib
```

### Usage

```sh
bcause hello.b              # compile, assemble, and link → a.out
bcause hello.b -o hello     # same, but output named hello
./hello
```

Additional flags:

| Flag | Effect |
| ---- | ------ |
| `-S` | Stop after generating assembly (`.s` file) |
| `-c` | Stop after assembling (`.o` file); do not link |
| `-o <file>` | Write output to `<file>` |
| `-L<dir>` | Look for `libb.a` in `<dir>` |
| `-save-temps` | Keep intermediate `.s` and `.o` files |

### Run the tests

```sh
cd intel64
make test
```

The test suite uses GoogleTest (downloaded automatically by CMake on first run).

## bbesm — B for BESM-6 (`besm6/`)

`bbesm` is a B compiler written in B itself, targeting the BESM-6 Soviet mainframe. It runs under the [Dubna](https://github.com/besm6/dubna) operating system emulator.

### Dependencies

Install both of these first:

* [dubna](https://github.com/besm6/dubna) — BESM-6 system emulator
* [besmc](https://github.com/besm6/besmc) — BESM-6 assembler

### Building bbesm

```sh
cd besm6
make
```

### Testing bbesm

```sh
cd besm6
make test
```

> **Note:** `bbesm` does not yet support `break`, `continue`, `for`, `do...while`, `switch`, the logical operators `&&` and `||`, compound assignment operators (like `=+`), hexadecimal literals, or the `%x` format in `printf()`. See [besm6/README.md](besm6/README.md) for the full list.

## Example Programs

The `examples/` directory contains several ready-to-run B programs:

| File | Description |
| ---- | ----------- |
| `hello.b` | Print "Hello, World!" using `write()` |
| `helloworld.b` | Same, using `printf()` |
| `fibonacci.b` | Print Fibonacci numbers |
| `fizzbuzz.b` | Classic FizzBuzz (1–100) |
| `e-2.b` | Compute e−2 to ~4000 decimal digits |

Here is the simplest one:

```b
/* The following program will print 'Hello World' to stdout. */

main() {
    write('Hello,');
    write(' World');
    write('!*n');
}
```

Compile and run it:

```sh
cd intel64
bcause ../examples/hello.b -o hello
./hello
```

## Documentation

* [B Runtime Library for BESM-6](doc/B-Runtime-Library.md) — comprehensive reference for all routines in `besm6/libb/`
* [Calling conventions](doc/Besm6-Calling-Conventions.md) for BESM-6
* [User's Reference to B](doc/kbman.pdf) by K. Thompson
* [A Tutorial Introduction to The Language B](doc/btut.pdf) by B. W. Kernighan
* [Users' Reference to B on MH-TSS](doc/bref.pdf) by S. C. Johnson
* [The Development of the C Language](doc/chistory.pdf) by Dennis M. Ritchie

## License

Both compilers are released under the MIT License. See the `LICENSE` file in each subdirectory for details.
