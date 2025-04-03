/* The following program will print 'Hello World' to stdout. */

main() {
    extrn x;
    auto y;

    printf("global -x = %d, expect %d*n", -x, -42);
    x = 0;
    printf("global !x = %d, expect %d*n", !x, 1);
    printf("global &x = %d, expect address*n", &x);
    y = 987;
    x = &y;
    printf("global **x = %d, expect %d*n", *x, y);

    printf("local -y = %d, expect %d*n", -y, -987);
    y = 0;
    printf("local !y = %d, expect %d*n", !y, 1);
    printf("local &y = %d, expect address*n", &y);
    x = 42;
    y = &x;
    printf("local **y = %d, expect %d*n", *y, x);
}

x 42;
