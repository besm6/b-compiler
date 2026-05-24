# BCause - B compiler for modern systems

**BCause** is a compiler for the **B** programming language, developed by *Ken Thompson* and *Dennis Ritchie* at *Bell Labs* in *1969*, later getting replaced by **C**. BCause is written in C99 and relies on a minimal set of dependencies, namely `libc` and the GNU binutils.

This repository also includes a `libb.a` implementation, B's standard library. It requires zero dependencies, not even libc.

BCause is implemented as a small single-pass compiler in ~2000 lines of pure C99 code. Therefore, it features small compile times with a very low memory footprint.

### Current Status

- [x] global variables
- [x] functions
- [x] `auto` & `extrn` variables
- [x] control flow statements
- [x] expressions
- [x] `libb.a` standard library
- [ ] optimization
- [ ] nicer error messages

### Compatibility

BCause supports **x86-64 Linux** and **macOS** (x86-64).

| Platform | Assembler / Linker | Notes |
| -------- | ------------------ | ----- |
| Linux x86-64 | GNU `as` + GNU `ld` | Any distro with `binutils` |
| macOS x86-64 | Apple `as` + Apple `ld` | Requires Xcode Command Line Tools |

Because `libb.a` uses raw syscalls, it must be compiled for the target OS. The Makefile handles this automatically.

### Installation

#### Prerequisites

- *Linux:* `gcc` (or any C99 compiler), `make`, `binutils`
- *macOS:* Xcode Command Line Tools — `xcode-select --install`

#### Build

```sh
cd intel64
make
```

To install BCause globally:

```sh
sudo make install
```

> **Warning**
> This requires root/sudo privileges and copies files to `/usr/local`.

### Usage

To compile a B source file (`.b`), use:
```console
$ bcause <your file>
```

To get help, type:
```console
$ bcause --help
```

### Licensing
BCause is licensed under the MIT License. See `LICENSE` in this repository for further information.

### References

- [Bell Labs User's Reference to B](https://www.bell-labs.com/usr/dmr/www/kbman.pdf) by Ken Thompson (Jan. 7, 1972)

- Wikipedia entry: [B (programming language)](https://en.wikipedia.org/wiki/B_(programming_language))
