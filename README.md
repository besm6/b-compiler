# B compiler for BESM6

This project has an objective to create a B compiler for BESM-6 architecture.
We start with existing B compiler for PDP-7 and modify it for BESM-6.

Files:
 * b-compiler.b - Sources of B compiler, written in B
 * b-proto.c - A prototypical compiler for the B language, rewritten in C
 * hello.b - 'Hello World' demo
 * test1.c - Simple demo to show features of the compiler

It makes sense to support Fortran calling conventions:

    call foobar(a, b, c)

In Madlen it looks like this:

    14,vtm,a
      ,its,14
    14,vtm,b
      ,its,14
    14,vtm,c
      ,its,14
      ,call,foobar
