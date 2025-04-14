/*
 * Read one symbol from input stream.
 * On end of file, return 0.
 */
read() {
    /*
     * MONCARD* - array of 25 words, contains current input line.
     */
    extrn moncard_;
    extrn read_idx, read_len;
    auto ch;

    if (read_idx == 0) {
        if (moncard_ == '*END F') {
            /* End of input data. */
            return (0);
        }
        if (moncard_ == 0) {
            /* No more input data. */
            return (0);
        }

        /* Find end of line. */
        read_len = 79;
        while (read_len >= 0) {
            if (char(&moncard_, read_len) != ' ')
                goto done;
            read_len = read_len - 1;
        }
done:
        read_len = read_len + 1;
    }

    if (read_idx == read_len) {
        /* Read next line. */
        monread_();
        read_idx = 0;
        return ('*n');
    }

    ch = char(&moncard_, read_idx);
    read_idx = read_idx + 1;
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
