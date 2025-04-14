/*
 * Read one symbol from input stream.
 * On end of file, return 0.
 */
read() {
    /*
     * MONCARD* - array of 25 words, contains current input line.
     */
    extrn moncard_;
    extrn read_idx;
    auto ch;

    if (read_idx == 0) {
        if (moncard_ == '*END F')
            return (0);
    }
    ch = char(&moncard_, read_idx);
    if (ch == '*n') {
        /* Got newline - read next line. */
        monread_();
        read_idx = 0;
    } else if (ch == 0) {
        /* Got byte \0 - stop there. */
    } else {
        read_idx = read_idx + 1;
        if (read_idx >= 80) {
            /* Read next line. */
            monread_();
            read_idx = 0;
        }
    }
    return (ch);
}

/*
 * Index of next byte in MONCARD* array.
 */
read_idx;
