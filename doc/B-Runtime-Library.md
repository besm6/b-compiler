# B Runtime Library for BESM-6

This document is a comprehensive reference for `besm6/libb/` — the runtime library
linked into every B program compiled for the BESM-6 mainframe.

## Overview

The library contains two kinds of routines:

- **Public API** — functions called directly from B source code: I/O, string/character
  manipulation, and program control.
- **Compiler-support routines** — helpers that the compiler emits calls to automatically;
  they implement the calling convention, arithmetic operators, and relational/logical
  operators. They are not meant to be called from B programs.

### Implementation Languages

Some routines are written in B and compiled to Madlen assembly by `bbesm` at build time:

```
printd.b  printo.b  printf.b  read.b  write.b  writeb.b  flush.b
```

The remaining routines are written directly in Madlen assembly:

```
char.madlen  lchar.madlen  exit.madlen  b_tout.madlen
b_save.madlen  b_save0.madlen  b_ret.madlen  b_true.madlen
b_mul.madlen  b_div.madlen  b_mod.madlen  b_not.madlen
b_eq.madlen  b_ne.madlen  b_lt.madlen  b_le.madlen  b_gt.madlen  b_ge.madlen
```

For the calling convention used across all routines, see
[Besm6-Calling-Conventions.md](Besm6-Calling-Conventions.md).

---

## Shared State

### Output Buffer

The following global variables are shared by all output functions.
They are declared in [writeb.b](../besm6/libb/writeb.b):

| Variable | Declaration | Description |
|----------|-------------|-------------|
| `out_buff[22]` | `out_buff[22]` | Line output buffer, 22 × 48-bit words |
| `out_cnt` | `out_cnt` | Index of the current (partially filled) word (0–21) |
| `out_shft` | `out_shft` | Bit offset of the next byte within the current word (0, 8, 16, 24, 32, 40) |
| `fout` | declared in `flush.b` | If non-zero, `flush` writes to drum via `wrcard` instead of stdout |

BESM-6 words are 48 bits wide. Bytes are packed MSB-first: the first byte of a word sits
at bit offset 40 (shift left by 40), the next at 32, and so on down to offset 0. Six bytes
fit in one word. When `out_shft` would advance past 40 the word is full: `out_cnt`
increments and `out_shft` resets to 0.

### Input State

The following variables are shared by the input subsystem.
They are declared in [read.b](../besm6/libb/read.b):

| Variable | Description |
|----------|-------------|
| `moncard_` | External 25-word array; the current input card (line) buffer |
| `kcount_` | External; standard input position counter managed by the monitor |
| `read_idx` | Index of the next byte to consume from `moncard_` |
| `read_len` | Length of useful data in `moncard_` (trailing spaces stripped) |
| `read_ptr` | Saved value of `kcount_` (for `readdrum`) |
| `read_dev` | Saved device word from `kcount_+1` (for `readdrum`) |

---

## Public API

### `read()` — [read.b](../besm6/libb/read.b)

```
ch = read()
```

Reads one character from standard input. Returns `0` at end of file.

**How it works:**

The BESM-6 monitor provides the current input card (punch card line) in the
25-word buffer `moncard_`. On the first call after a new card is loaded
(`read_idx == 0`), `read` inspects the first byte of `moncard_`:

- `'**'` — end-of-input sentinel; returns `0`.
- `0` — invalid card; returns `0`.
- Otherwise — finds the last non-space byte to set `read_len` (strips trailing spaces).

The function then returns characters one at a time by calling `char(&moncard_, read_idx)`,
advancing `read_idx`. When `read_idx` reaches `read_len` (end of useful data), it:

1. Saves the current input position into `read_ptr` / `read_dev` (from `kcount_`),
   for use by a hypothetical `readdrum()`.
2. Calls `monread_()` to have the monitor read the next card into `moncard_`.
3. Resets `read_idx` to `0`.
4. Returns `'*n'` (newline) to signal the end of the line.

**State read/written:** `read_idx`, `read_len`, `read_ptr`, `read_dev`

**External dependencies:** `moncard_`, `kcount_`, `monread_()` (monitor extracode)

**Calls:** `char`

---

### `writeb(b)` — [writeb.b](../besm6/libb/writeb.b)

```
writeb(b)
```

Appends one byte to the output line buffer.

**How it works:**

The byte is first masked to 8 bits (`b & 0377`). Then:

- If `b == '*n'` (newline, 0x0A): calls `flush()` and returns immediately.
- If `out_cnt == 22` (buffer full): calls `flush()` first.
- Places the byte into `out_buff[out_cnt]` at bit position `40 - out_shft`:

  ```
  out_buff[out_cnt] |= (b << (40 - out_shft))
  ```

- If `out_shft == 40` (the word is now full): sets `out_shft = 0` and increments `out_cnt`.
- Otherwise: advances `out_shft` by 8.

**State read/written:** `out_cnt`, `out_shft`, `out_buff`

**Calls:** `flush`

---

### `flush()` — [flush.b](../besm6/libb/flush.b)

```
flush()
```

Flushes the output line buffer to stdout (or drum), then clears it.

**How it works:**

Output destination depends on `fout`:

- **`fout == 0` (stdout):** Calls `b/tout(out_buff)`. This issues BESM-6 extracode `*71`
  to write the line buffer directly to the terminal/listing output.

- **`fout != 0` (drum):** Pads the buffer to exactly 14 words by calling `writeb(' ')`
  in a loop while `out_cnt < 14`, then calls `wrcard(0, out_buff)` to write one card
  image to drum, then calls `writeb(0)` to append a null terminator word.

After output, clears the buffer:

1. If `out_shft != 0`, increments `out_cnt` (the partial word counts as full for clearing).
2. Zeroes each word of `out_buff` from `out_cnt - 1` down to `0`.
3. Resets `out_cnt = 0` and `out_shft = 0`.

**State read/written:** `fout`, `out_cnt`, `out_shft`, `out_buff`

**Calls:** `writeb` (for padding), `b/tout`, `wrcard` (external)

> **Note:** `writeb` calls `flush` and `flush` calls `writeb` (for drum padding).
> This is not a true recursion: the padding calls to `writeb(' ')` emit space bytes,
> which do not trigger another flush because the buffer is freshly cleared and
> `out_cnt` will not reach 22 during padding.

---

### `write(ch)` — [write.b](../besm6/libb/write.b)

```
write(ch)
```

Appends a packed multi-character word to the output buffer, skipping leading zero bytes.

**How it works:**

A B word can hold up to 6 characters packed MSB-first. `write` scans for the first
non-zero byte by starting at `shift = 40` and stepping down by 8:

```
b = (ch >> shift) & 0377
```

Once a non-zero byte is found (or `shift` reaches 0 and `b = ch & 0377`), it calls
`writeb(b)` for that byte and then for every subsequent byte down to `shift == 0`.

This means `write(0)` emits nothing (all bytes are zero). Use `writeb(0)` to explicitly
write a NUL byte.

**Calls:** `writeb`

---

### `printf(fmt, ...)` — [printf.b](../besm6/libb/printf.b)

```
printf(fmt, arg1, arg2, ...)
```

Formatted output. Interprets the format string `fmt` and outputs the result.

**Format specifiers:**

| Spec | Action |
|------|--------|
| `%d` | Print the next argument as a signed decimal integer (calls `printd`) |
| `%o` | Print the next argument as an octal integer (calls `printo`) |
| `%c` | Print the next argument as a multi-char word (calls `write`) |
| `%s` | Print the next argument as a NUL-terminated string (iterates with `char`+`writeb`) |
| `%%` | Print a literal `%` character |
| `%?` | Unknown specifier: print `%` and re-process `?` from the same position |

**How it works:**

`printf` uses `char(fmt, i)` to iterate over `fmt` byte by byte. A pointer `ap`
is initialised to `&args` (the address of the first variadic argument on the stack);
each `%x` conversion reads `*ap` and advances `ap` by one word.

Plain bytes (not `%` and not `'*0'`) are sent to `writeb`. A `'*0'` byte (NUL)
in the format string terminates the function.

**Calls:** `char`, `writeb`, `printd`, `printo`, `write`

---

### `printd(n)` — [printd.b](../besm6/libb/printd.b)

```
printd(n)
```

Prints a signed decimal integer.

**How it works:**

- If `n < 0`: calls `writeb('-')` and negates `n`.
- Recursively calls `printd(n / 10)` to print more significant digits (if `n / 10 != 0`).
- Calls `writeb((n % 10) + '0')` for the current digit.

**Calls:** `writeb`

---

### `printo(n)` — [printo.b](../besm6/libb/printo.b)

```
printo(n)
```

Prints an unsigned octal integer.

**How it works:**

- Recursively calls `printo(n >> 3)` for more significant octal digits (if `n >> 3 != 0`).
- Calls `writeb((n & 7) + '0')` for the current digit.

Treats `n` as unsigned; there is no sign handling.

**Calls:** `writeb`

---

### `char(str, i)` — [char.madlen](../besm6/libb/char.madlen)

```
ch = char(str, i)
```

Returns the byte at zero-based index `i` of the string (or word array) `str`.

**How it works:**

BESM-6 packs 6 bytes per 48-bit word. Given index `i`:

- **Word index** = `i / 6` — which word in the array.
- **Byte position** = `i % 6` — which byte within that word (0 = most significant).

The division by 6 uses a fixed-point magic-number multiply for speed. The byte is
then extracted by shifting right by `(5 - position) * 8` bits and masking with `0377`.

Bytes are numbered left-to-right: `char("abc", 0)` returns `'a'`,
`char("abc", 1)` returns `'b'`, and so on.

**Dependencies:** none

---

### `lchar(str, i, ch)` — [lchar.madlen](../besm6/libb/lchar.madlen)

```
old = lchar(str, i, ch)
```

Sets byte `i` of string `str` to `ch`; returns `ch`.

**How it works:**

Uses the same `i / 6` / `i % 6` decomposition as `char`. To write the byte:

1. Looks up a precomputed 6-entry mask table (one entry per byte position) to clear the
   target byte in the word.
2. Looks up a precomputed 6-entry shift table to position `ch` at the correct offset.
3. OR's the positioned byte into the masked word and stores the result back.

The mask table entries clear exactly one byte (8 bits) at the appropriate position;
all other bits are `1`. The shift table entries correspond to `(5 - position) * 8`.

**Dependencies:** none

---

### `exit()` — [exit.madlen](../besm6/libb/exit.madlen)

```
exit()
```

Terminates the program immediately. Does not return.

Issues BESM-6 extracode `*74` (terminate job).

**Dependencies:** none

---

### `b/tout(buf)` — [b_tout.madlen](../besm6/libb/b_tout.madlen)

```
b/tout(buf)
```

Low-level output: writes a 22-word line buffer to stdout via BESM-6 extracode `*71`.

Not intended to be called from B programs. Used internally by `flush`.

The extracode `*71` is the BESM-6 "type out" monitor call. The info word at `info`
encodes the channel (12) and the buffer address is passed in r12.

**Dependencies:** none (hardware extracode)

---

## Compiler-Support Routines

These routines are emitted automatically by the compiler. They implement the B calling
convention and the operators that have no direct BESM-6 instruction equivalents.

For a detailed description of the calling convention, see
[Besm6-Calling-Conventions.md](Besm6-Calling-Conventions.md).

---

### `b/save` — [b_save.madlen](../besm6/libb/b_save.madlen)

Called on entry to every B function that has **one or more parameters**.

The compiler emits:

```
   ,its, 13        ; push return address
13 ,vjm, b/save    ; call b/save
```

**Actions:**

1. Adjusts r15 (stack pointer) by the argument count from r14: `r15 += r14`
   (since r14 is negative, this *decrements* r15 past the arguments already on the stack).
2. Saves r7, r6, and the accumulator (which holds return address r13) to the stack.
3. Sets r6 = address of argument #1 (the parameter pointer).
4. Sets r7 = current r15 (the auto-variable pointer, pointing just above the saved registers).
5. Extracts the real return address from the saved value and puts it in r13.
6. Jumps to r13 to continue the function body.

---

### `b/save0` — [b_save0.madlen](../besm6/libb/b_save0.madlen)

Called on entry to every B function with **no parameters**.

The compiler emits:

```
   ,its, 13        ; push return address
13 ,vjm, b/save0   ; call b/save0
```

Identical to `b/save` except that it first increments r15 by 1 to allocate a dummy
argument slot, so that the resulting stack frame layout is uniform with parameterized
functions. This allows `b/ret` to use the same unwind logic regardless of parameter count.

---

### `b/ret` — [b_ret.madlen](../besm6/libb/b_ret.madlen)

Called at every exit point of a B function. The return value must be in the accumulator
before the jump to `b/ret`.

**Actions:**

1. Computes the number of arguments from r6 and r7 to determine how far to unwind r15.
2. Restores saved r6, r7, r13 from the stack frame.
3. Restores r15 to the caller's stack level.
4. Jumps to r13 (the caller's return address).

---

### `b/true` — [b_true.madlen](../besm6/libb/b_true.madlen)

A single word containing the integer value `1`, used as the canonical "true" result
by all relational and logical operators. Because BESM-6 has no load-immediate instruction
for arbitrary values, the operators load this word with `xta b/true`.

---

### Arithmetic Operators

Each routine receives two arguments in the standard way:

- The first operand (`a`) is at the top of the stack (r15 points just past it).
- The second operand (`b`) is in the accumulator.

The result is left in the accumulator; r15 is not changed (the compiler adjusts the
stack after the call). Results are masked to 36 bits to match B integer width.

#### `b/mul` — [b_mul.madlen](../besm6/libb/b_mul.madlen)

Computes `a * b`.

Uses the BESM-6 `a*x` (multiply) instruction with 36-bit normalization via `ntr` and
`a+x =:64`. The final result is masked with `=37 7777 7777 7777` to keep only the
lower 36 bits.

#### `b/div` — [b_div.madlen](../besm6/libb/b_div.madlen)

Computes `a / b`.

BESM-6 division (`a/x`) operates on normalized floating-point representations.
The routine converts both operands to the required form using `ntr` (normalize) and
`avx` (absolute value exchange), performs the division, then extracts the 36-bit integer
result.

#### `b/mod` — [b_mod.madlen](../besm6/libb/b_mod.madlen)

Computes `a % b` (remainder).

Implemented as `a - (a / b) * b`, using the same sign-normalizing sequence as `b/div`.
Both `a` and the intermediate `a / b` are preserved on the stack during the computation.

---

### Relational and Logical Operators

Each routine receives two arguments the same way as the arithmetic operators.
It returns `1` (loaded from `b/true`) if the condition is true, or `0` otherwise.

All seven routines share the same two-branch template:

1. Perform a subtraction or exchange to set the accumulator sign.
2. Branch to `true` on the appropriate sign condition (`uza` = branch if zero,
   `u1a` = branch if non-zero/positive).
3. Fall-through path: load `0` from the accumulator's implicit zero and return.
4. `true` path: load `1` via `xta b/true` and return.

| Routine | Source | B op | Computation | Branch |
|---------|--------|------|-------------|--------|
| `b/not` | [b_not.madlen](../besm6/libb/b_not.madlen) | `!` | `aex` (swap A and X) | `uza`: branch if `b == 0` |
| `b/eq` | [b_eq.madlen](../besm6/libb/b_eq.madlen) | `==` | `aex` (swap A and X, then `a-x`) | `uza`: branch if `a == b` |
| `b/ne` | [b_ne.madlen](../besm6/libb/b_ne.madlen) | `!=` | `aex` | `u1a`: branch if `a != b` |
| `b/lt` | [b_lt.madlen](../besm6/libb/b_lt.madlen) | `<` | `x-a` (compute `stack - acc`) | `u1a`: branch if `a < b` |
| `b/le` | [b_le.madlen](../besm6/libb/b_le.madlen) | `<=` | `a-x` (compute `acc - stack`) | `uza`: branch if `a <= b` |
| `b/gt` | [b_gt.madlen](../besm6/libb/b_gt.madlen) | `>` | `a-x` | `u1a`: branch if `a > b` |
| `b/ge` | [b_ge.madlen](../besm6/libb/b_ge.madlen) | `>=` | `x-a` | `uza`: branch if `a >= b` |

> **Note on `b/eq`:** The `aex` instruction exchanges accumulator A and extension register X.
> For equality, `aex` followed by `uza` tests whether the original stack value (now in A)
> equals the accumulator value (now in X) — effectively `a - b == 0` after the exchange.

---

## Dependency Graph

```
Public API
──────────
  read        ──► char
              ──► monread_()         [external monitor call]

  write       ──► writeb

  writeb      ──► flush

  flush       ──► writeb             [for drum padding only]
              ──► b/tout
              ──► wrcard()           [external drum write]

  printd      ──► writeb
  printo      ──► writeb

  printf      ──► char
              ──► writeb
              ──► printd
              ──► printo
              ──► write

  char        (no dependencies)
  lchar       (no dependencies)
  exit        (no dependencies — extracode *74)
  b/tout      (no dependencies — extracode *71)

Compiler Helpers
────────────────
  b/save      (no dependencies)
  b/save0     (no dependencies)
  b/ret       (no dependencies)
  b/true      (no dependencies)

  b/mul       (no dependencies)
  b/div       (no dependencies)
  b/mod       (no dependencies)

  b/not  ──► b/true
  b/eq   ──► b/true
  b/ne   ──► b/true
  b/lt   ──► b/true
  b/le   ──► b/true
  b/gt   ──► b/true
  b/ge   ──► b/true
```
