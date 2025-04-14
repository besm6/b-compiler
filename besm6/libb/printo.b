printo(n) {
    auto a;

    if ((a = n >> 3)) {
        printo(a);
    }
    writeb((n & 7) + '0');
}
