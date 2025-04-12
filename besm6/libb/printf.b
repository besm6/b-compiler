printf(fmt, args) {
    auto ap, a, c, i, n;

    i = 0;
    ap = &args;
loop:
    while ((c = char(fmt, i)) != '%') {
        if (c == '*e')
            return;
        writeb(c);
        i = i + 1;
    }
    i = i + 1;
    c = char(fmt, i);
    if (c == '%') {
        writeb('%');
        i = i + 1;
        goto loop;
    }
    a = *ap;
    if (c == 'd') {
        printd(a);
    } else if (c == 'o') {
        printo(a);
    } else if (c == 'c') {
        write(a);
    } else if (c == 's') {
        n = 0;
        while ((c = char(a, n)) != '*e') {
            writeb(c);
            n = n + 1;
        }
    } else {
        /* bad format specification, ignore */
        goto loop;
    }
    i = i + 1;
    ap = ap + 1;
    goto loop;
}
