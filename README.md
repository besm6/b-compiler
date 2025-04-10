# B compiler for BESM6

This project has an objective to create a B compiler for BESM-6 architecture.
We start with existing B compiler for PDP-7 and modify it for BESM-6.

The calling conventions are similar to Fortran, but arguments are passed
by value rather than by address:

    result = foobar(a, b, c)

In Madlen it looks like this:

       ,xta, a
       ,xts, b
       ,xts, c
    14 ,vtm, -3
    13 ,vjm, foobar
       ,atx, result

Note that the number of arguments (negated) is passed in register 14.
This is needed for functions with a variable number of arguments.
