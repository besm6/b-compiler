/*
 * Write buffered line to stdout.
 */
flush() {
    extrn out_cnt, out_shft, out_buff;

    b$tout(out_buff);
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
