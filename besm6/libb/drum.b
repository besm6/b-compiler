/*
 * Initialize output to drum #3.
 */
initdrum()
{
    extrn flgex_;
    auto arg;

    /* write assembly code to drum */
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
    extrn arread_, kcount_;

    /* finish writing assembly code */
    write('**read ');
    write('old*n');
    wriend();

    /* read from drum */
    arread_ = kcount_;
    1[&arread_] = 1[&kcount_];

    kcount_ = 0;
    1[&kcount_] = 010000000027777;
    2[&kcount_] = 1;

    /* skip the next line */
    monread_();
}
