/*
 * Write buffered line to stdout.
 */
flush() {
    extrn fout, out_cnt, out_shft, out_buff;

    if (fout) {
        /* write to drum */
        while (out_cnt < 14) {
            writeb(' ');
        }
        wrcard(0, out_buff);
    } else {
        /* write to standard output */
        b$tout(out_buff);
    }

    if (out_cnt | out_shft) {
        /* clear buffer */
        if (out_shft) {
            ++out_cnt;
            out_shft = 0;
        }
        while (out_cnt > 0) {
            out_buff[--out_cnt] = 0;
        }
    }
}

fout;
