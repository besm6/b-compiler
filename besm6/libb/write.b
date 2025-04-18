/*
 * Append a multi-char to the stdout buffer.
 */
write(ch) {
    auto shift, b;

    shift = 40;
    while (shift > 0) {
        b = (ch >> shift) & 0377;
        if (b)
            goto putchar;
        shift = shift - 8;
    }
    b = ch;
putchar:
    writeb(b);
    if (shift > 0) {
        shift = shift - 8;
        b = ch >> shift;
        goto putchar;
    }
}
