# B compiler for BESM6

This project has an objective to create a B compiler for BESM-6 architecture.
We start with existing B compiler for PDP-7 and modify it for BESM-6.

It makes sense to support Fortran calling conventions:

    z = foobar(a, b, c)

In Madlen it looks like this:

    14,vtm,a
      ,ita,14
    14,vtm,b
      ,its,14
    14,vtm,c
      ,its,14
   :  ,call,foobar
      ,   ,3        -- number of arguments
      ,atx,z
