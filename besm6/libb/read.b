/*
 * Read one symbol from input stream.
 * On end of file, return 0.
 */
read() {
    /*
     * MONCARD* - array of 25 words, contains current input line.
     * KCOUNT* - array of 23 words, contains position of standard input.
     */
    extrn moncard_, kcount_;
    extrn read_idx, read_len, read_ptr, read_dev;
    auto ch;

    if (read_idx == 0) {
        ch = moncard_ >> 40;
        if (ch == '**') {
            /* End of input data. */
            return (0);
        }
        if (ch == 0) {
            /* Bad input data. */
            return (0);
        }

        /* Find end of line. */
        read_len = 79;
        while (read_len >= 0) {
            if (char(&moncard_, read_len) != ' ')
                goto done;
            --read_len;
        }
done:
        ++read_len;
    }

    if (read_idx == read_len) {
        /* Save read pointer for readdrum(). */
        read_ptr = kcount_;
        read_dev = 1[&kcount_];

        /* Read next line. */
        monread_();
        read_idx = 0;
        return ('*n');
    }

    ch = char(&moncard_, read_idx);
    ++read_idx;
    return (ch);
}

/*
 * Index of next byte in MONCARD* array.
 */
read_idx;

/*
 * Length of data in MONCARD* array.
 */
read_len;

/*
 * Saved position of standard input.
 */
read_ptr;
read_dev;
