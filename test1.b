empty {
}

ret1 {
    return (1);
}

arith {
    return (2 + 3 - 4 * 5 / 6 % 7);
}

extvar {
    extrn x;
    return (x);
}

autovar {
    auto a;
    auto b 10;
    return (a + b);
}

param1(p) {
    return (p);
}

params(a, b, c) {
    return (a + b - c);
}

label() {
    extrn x;
loop:
    x = 1;
    goto loop;
}

x;
y 1;
z -2;
array [10]
    1, 2, 3;
