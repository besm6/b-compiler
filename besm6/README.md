# B compiler for BESM-6

Files and directories:
 * b-besm.b - Sources of B compiler for BESM-6, written in B
 * libb - Runtime library for B programs
 * tape - Scripts for building a tape image for Dubna
 * tests - Unit tests for B compiler

Missing features:
 * `break` statement
 * `continue` statement
 * `for` statement
 * `do...while` statement
 * `switch` statement
 * Logical operators `&&` and `||`
 * Compound assignment operators `=+`, `=-`, `=*`, `=/`, `=%`, `=<<`, `=<=`, `=<`, `=>>`, `=>=`, `=>`, `=!=`, `===`, `=&`, `=|`
 * Hexadecimal literals
 * Format `%x` in `printf()`
