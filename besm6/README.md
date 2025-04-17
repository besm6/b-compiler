# B compiler for BESM-6

Files and directories:
 * b-besm.b - Sources of B compiler for BESM-6, written in B
 * libb - Runtime library for B programs
 * bootstrap - B compiler for BESM-6, written in C, for bootstraping
 * tests - Unit tests for B compiler

Missing features:
 * `break` statement
 * `continue` statement
 * `for` statement
 * `switch` statement
 * Ternary operator `?:`
 * Compound assignment operators `=+`, `=-`, `=*`, `=/`, `=%`, `=<<`, `=<=`, `=<`, `=>>`, `=>=`, `=>`, `=!=`, `===`, `=&`, `=|`
 * Hexadecimal literals
 * Format `%x` in `printf()`
