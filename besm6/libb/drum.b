/*
 * Initialize output to drum #3.
 */
initdrum()
{
    extrn flgex_;
    auto arg;

    arg = 030000;
    wbegin(0, &arg);
    write('**no li');
    write('st*n');
    write('**madle');
    write('n*n');

    /* disable writlib* */
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
