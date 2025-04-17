printf(fmt, args) {
    auto ap, a, c, i, n;

    i = 0;
    ap = &args;
loop:
    while ((c = char(fmt, i)) != '%') {
        if (c == '*0')
            return;
        writeb(c);
        ++i;
    }
    ++i;
    c = char(fmt, i);
    if (c == '%') {
        writeb('%');
        ++i;
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
        while ((c = char(a, n)) != '*0') {
            writeb(c);
            ++n;
        }
    } else {
        /* bad format specification, ignore */
        goto loop;
    }
    ++i;
    ++ap;
    goto loop;
}
