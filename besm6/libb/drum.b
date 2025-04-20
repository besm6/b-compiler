/*
 * Initialize output to drum #3.
 */
initdrum()
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

    /* disable listing of Madlen code */
    write('**no li');
    write('st*n');

    /* clear erroflg* */
    write('**call ');
    write('blocke');
    write('rr*n');

    /* invoke Madlen */
    write('**madle');
    write('n*n');

    /* disable writlib* when translator finishes */
    flgex_ = 1;
}

/*
 * Invoke assembler from drum.
 */
readdrum()
{
    extrn fout, line, nerror, arread_, kcount_, read_ptr, read_dev;

    /* finish writing assembly code */
    write('**read ');
    write('old*n');
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
