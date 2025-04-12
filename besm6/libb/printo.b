printo(n) {
    auto a;

    if (n < 0) {
        writeb('-');
        n = -n;
    }
    if ((a = n / 8)) {
        printo(a);
    }
    writeb((n & 7) + '0');
}
