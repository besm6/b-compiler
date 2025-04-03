/* The following program will print 'Hello World' to stdout. */

main() {
    extrn x, printf;
    auto y;

    y = x + 100;
    printf("x = %d, y = %d*n", x, y);
    if (x)
        printf("if (x) WRONG*n");
    else
        printf("if (x) Correct*n");

    if (y)
        printf("if (y) Correct*n");
    else
        printf("if (y) WRONG*n");

    if (!x)
        printf("if (!x) Correct*n");
    else
        printf("if (!x) WRONG*n");

    if (!y)
        printf("if (!y) WRONG*n");
    else
        printf("if (!y) Correct*n");

    while (!x) {
        printf("while (!x) x = %d*n", x);
        x = 42;
    }
}

x;
