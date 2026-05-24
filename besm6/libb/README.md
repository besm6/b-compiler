# libb — B Runtime Library for BESM-6

Runtime library for B programs targeting the BESM-6 mainframe.

For basing, use:

 * `r14` when no other functions are called, and no extracodes
 * `r12` in other cases

## I/O Functions

| Function | Source | Description |
|----------|--------|-------------|
| `read()` | [read.b](read.b) | Read one character from stdin; returns `0` at EOF |
| `write(ch)` | [write.b](write.b) | Append a multi-char word to the output buffer (skips leading zero bytes) |
| `writeb(b)` | [writeb.b](writeb.b) | Append one byte to the output buffer; flushes on newline or full buffer |
| `flush()` | [flush.b](flush.b) | Flush the output line buffer to stdout (or drum when `fout` is set) |
| `printf(fmt, ...)` | [printf.b](printf.b) | Formatted output: `%d` decimal, `%o` octal, `%c` character, `%s` string |
| `printd(n)` | [printd.b](printd.b) | Print a signed decimal integer |
| `printo(n)` | [printo.b](printo.b) | Print an octal integer |
| `b/tout(buf)` | [b_tout.madlen](b_tout.madlen) | Low-level: write a line buffer directly to stdout via extracode `*71` |

## String / Character Functions

| Function | Source | Description |
|----------|--------|-------------|
| `char(str, i)` | [char.madlen](char.madlen) | Return byte `i` of string `str` (left-to-right, 0-based) |
| `lchar(str, i, ch)` | [lchar.madlen](lchar.madlen) | Set byte `i` of string `str` to `ch`; return `ch` |

## Program Control

| Function | Source | Description |
|----------|--------|-------------|
| `exit()` | [exit.madlen](exit.madlen) | Terminate the program (extracode `*74`) |

## Compiler-Support Routines

These are internal helpers emitted by the compiler; they are not called directly from B source.

### Calling Convention

| Routine | Source | Description |
|---------|--------|-------------|
| `b/save` | [b_save.madlen](b_save.madlen) | Save registers on function entry (1+ parameters) |
| `b/save0` | [b_save0.madlen](b_save0.madlen) | Save registers on function entry (0 parameters) |
| `b/ret` | [b_ret.madlen](b_ret.madlen) | Restore registers and return from a B function |
| `b/true` | [b_true.madlen](b_true.madlen) | Constant `1` (logical true value) |

### Arithmetic Operators

| Routine | Source | B operator | Description |
|---------|--------|-----------|-------------|
| `b/mul` | [b_mul.madlen](b_mul.madlen) | `*` | Integer multiply |
| `b/div` | [b_div.madlen](b_div.madlen) | `/` | Integer divide |
| `b/mod` | [b_mod.madlen](b_mod.madlen) | `%` | Integer modulo |

### Relational and Logical Operators

| Routine | Source | B operator | Description |
|---------|--------|-----------|-------------|
| `b/not` | [b_not.madlen](b_not.madlen) | `!` | Logical NOT |
| `b/eq` | [b_eq.madlen](b_eq.madlen) | `==` | Equal |
| `b/ne` | [b_ne.madlen](b_ne.madlen) | `!=` | Not equal |
| `b/lt` | [b_lt.madlen](b_lt.madlen) | `<` | Less than |
| `b/le` | [b_le.madlen](b_le.madlen) | `<=` | Less than or equal |
| `b/gt` | [b_gt.madlen](b_gt.madlen) | `>` | Greater than |
| `b/ge` | [b_ge.madlen](b_ge.madlen) | `>=` | Greater than or equal |
