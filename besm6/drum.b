/*
 * Initialize output to drum #3.
 */
init()
{
    extrn fout, flgex_;
    auto arg;

    /* print to stdout */
    fout = 0;
    printf("B compiler for BESM-6, version 04/2025*n");

    /* write assembly code to drum */
    fout = 1;
    arg = 030000;
    wbegin(0, &arg);

    /* clear erroflg* */
    printf("**call blockerr*n");

    /* invoke Madlen, disable listing */
    printf("**madlen,L=0*n");

    /* disable writlib* when translator finishes */
    flgex_ = 1;
}

/*
 * Invoke assembler from drum.
 */
finish()
{
    extrn fout, line, nerror, arread_, kcount_, read_ptr, read_dev;

    /* finish writing assembly code */
    printf("**read old*n");
    wriend();

    /* print to stdout */
    fout = 0;
    printf("Compiled %d lines of code, found %d errors*n", line - 1, nerror);

    /* read from drum */
    arread_ = read_ptr;
    1[&arread_] = read_dev;

    kcount_ = 0;
    1[&kcount_] = 010000000027777;
    2[&kcount_] = 1;

    /* skip the next line */
    monread_();
}
