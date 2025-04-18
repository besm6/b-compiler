/*
 * Append one byte to output buffer.
 */
writeb(b) {
    extrn out_cnt, out_shft, out_buff;
    auto p;

    b = b & 0377;
    if (b == '*n') {
        flush();
        return;
    }

    if (out_cnt == 22) {
        flush();
    }

    p = &out_buff[out_cnt];
    *p = *p | (b << (40 - out_shft));

    if (out_shft == 40) {
        /* next word */
        out_shft = 0;
        ++out_cnt;
    } else {
        /* next byte */
        out_shft = out_shft + 8;
    }
}

out_cnt;
out_shft;
out_buff[22];
