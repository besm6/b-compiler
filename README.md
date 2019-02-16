# B compiler for BESM6

This project has an objective to create a B compiler for BESM-6 architecture.
We start with existing B compiler for PDP-7 and modify it for BESM-6.

Files:
 * b-compiler.b - Sources of B compiler, written in B
 * b-proto.c - A prototypical compiler for the B language, rewritten in C

## Code generation
```
gen('c', cval);     /* consop */            Small non-negative literal (push on stack)

gen('a', csym[1]);                          Read auto variable (push on stack)

gen('u', 1);        /* uadr */              Unary & (address) operation
gen('u', 2);        /* umin */              Unary - (minus) operation
gen('u', 3);        /* uind */              Unary * (index) operation
gen('u', 4);        /* unot */              Unary ! operation

gen('b', 1);        /* asg */               Binary = (assignment) operation
gen('b', 2);        /* bor */               Binary | (or) operation
gen('b', 3);        /* band */              Binary & (and) operation
gen('b', 4);        /* beq */               Binary == operation
gen('b', 5);        /* bne */               Binary != operation
gen('b', 6);        /* ble */               Binary <= operation
gen('b', 7);        /* blt */               Binary < operation
gen('b', 8);        /* bge */               Binary >= operation
gen('b', 9);        /* bgt */               Binary > operation
gen('b', 12);       /* badd */              Binary + (add) operation
gen('b', 13);       /* bmin */              Binary - (subtract) operation
gen('b', 14);       /* bmod */              Binary % (modulo) operation
gen('b', 15);       /* bmul */              Binary * (multiply) operation
gen('b', 16);       /* bdiv */              Binary / (divide) operation

gen('n', 1);        /* mcall */             Call a routine without parameters
gen('n', 2);        /* mark */              Start a list of parameters
gen('n', 3);        /* call */              Call a routine with parameters
gen('n', 4);        /* vector */            Indexing [] operation
gen('n', 5);        /* litrl */             Integer literal (push on stack)
gen('n', 6);        /* goto */              Jump to a label
gen('n', 7);        /* retrn */             Return from current routine

gen('s', nauto);    /* setop */             Set stack offset

gen('y', nauto);    /* aryop */             Declare auto array (on stack)
```
