#include <fstream>

#include "fixture.h"

TEST_F(besm6, unary_negate)
{
    auto output = compile_and_run(R"(

        main() {
            auto v;
            v = -70;
            writeb(-v); /* 70 is ASCII 'F' */
            v = 177;
            writeb(-v); /* 256-177 -> 79 is ASCII 'O' */
            flush();
        }
    )");
    const std::string expect = "FO\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, if_else)
{
    auto output = compile_and_run(R"(

        main() {
            auto v;
            v = 123;
            if (v)
                write('Yes*n');
            else
                write('No*n');
            v = 0;
            if (v)
                write('Yes*n');
            else
                write('No*n');
        }
    )");
    const std::string expect = "YES\nNO\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, unary_logical_not)
{
    auto output = compile_and_run(R"(

        main() {
            auto v;
            v = 123;
            if (!v)
                write('Yes*n');
            else
                write('No*n');
            v = 0;
            if (!v)
                write('Yes*n');
            else
                write('No*n');
        }
    )");
    const std::string expect = "NO\nYES\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, unary_indirection)
{
    auto output = compile_and_run(R"(

        main() {
            extrn letters;
            auto p;

            p = letters;
            *p = 'q';
            write(*p);
            p = p + 1;
            write(*p);
            p = p + 1;
            write(*p);
            flush();
        }
        letters[] 'a', 'b', 'c';
    )");
    const std::string expect = "QBC\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, shift_left_const)
{
    auto output = compile_and_run(R"(

        main() {
            extrn x;

            x = 01234567;
            printf("octal %o << 9 = %o*n", x, x << 9);
            x = -01234567;
            printf("octal %o << 6 = %o*n", x, x << 6);
        }
        x;
    )");
    const std::string expect = R"(OCTAL 1234567 << 9 = 1234567000
OCTAL 37777776543211 << 6 = 3777777654321100
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, shift_left_var)
{
    auto output = compile_and_run(R"(

        main() {
            extrn x, y;

            x = 01234567;
            y = 9;
            printf("octal %o << %d = %o*n", x, y, x << y);
            x = -01234567;
            y = 6;
            printf("octal %o << %d = %o*n", x, y, x << y);
        }
        x;
        y;
    )");
    const std::string expect = R"(OCTAL 1234567 << 9 = 1234567000
OCTAL 37777776543211 << 6 = 3777777654321100
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, shift_right_const)
{
    auto output = compile_and_run(R"(

        main() {
            extrn x;

            x = 01234567;
            printf("octal %o >> 9 = %o*n", x, x >> 9);
            x = 'ABCDEF';
            printf("octal %o >> 6 = %o*n", x, x >> 6);
        }
        x;
    )");
    const std::string expect = R"(OCTAL 1234567 >> 9 = 1234
OCTAL 2024110321042506 >> 6 = 20241103210425
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, shift_right_var)
{
    auto output = compile_and_run(R"(

        main() {
            extrn x, y;

            x = 01234567;
            y = 9;
            printf("octal %o >> %d = %o*n", x, y, x >> y);
            x = 'ABCDEF';
            y = 6;
            printf("octal %o >> %d = %o*n", x, y, x >> y);
        }
        x;
        y;
    )");
    const std::string expect = R"(OCTAL 1234567 >> 9 = 1234
OCTAL 2024110321042506 >> 6 = 20241103210425
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, relational_equal)
{
    auto output = compile_and_run(R"(

        main() {
            if (2 == 3)
                write('True*n');
            else
                write('False*n');
            if (3 == 3)
                write('True*n');
            else
                write('False*n');
        }
    )");
    const std::string expect = R"(FALSE
TRUE
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, relational_not_equal)
{
    auto output = compile_and_run(R"(

        main() {
            if (2 != 3)
                write('True*n');
            else
                write('False*n');
            if (3 != 3)
                write('True*n');
            else
                write('False*n');
        }
    )");
    const std::string expect = R"(TRUE
FALSE
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, relational_less_than)
{
    auto output = compile_and_run(R"(

        main() {
            if (2 < 3)
                write('True*n');
            else
                write('False*n');
            if (3 < 3)
                write('True*n');
            else
                write('False*n');
            if (3 < 2)
                write('True*n');
            else
                write('False*n');
        }
    )");
    const std::string expect = R"(TRUE
FALSE
FALSE
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, relational_less_or_equal)
{
    auto output = compile_and_run(R"(

        main() {
            if (2 <= 3)
                write('True*n');
            else
                write('False*n');
            if (3 <= 3)
                write('True*n');
            else
                write('False*n');
            if (3 <= 2)
                write('True*n');
            else
                write('False*n');
        }
    )");
    const std::string expect = R"(TRUE
TRUE
FALSE
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, relational_greater_than)
{
    auto output = compile_and_run(R"(

        main() {
            if (2 > 3)
                write('True*n');
            else
                write('False*n');
            if (3 > 3)
                write('True*n');
            else
                write('False*n');
            if (3 > 2)
                write('True*n');
            else
                write('False*n');
        }
    )");
    const std::string expect = R"(FALSE
FALSE
TRUE
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, relational_greater_or_equal)
{
    auto output = compile_and_run(R"(

        main() {
            if (2 >= 3)
                write('True*n');
            else
                write('False*n');
            if (3 >= 3)
                write('True*n');
            else
                write('False*n');
            if (3 >= 2)
                write('True*n');
            else
                write('False*n');
        }
    )");
    const std::string expect = R"(FALSE
TRUE
TRUE
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, binary_multiply)
{
    auto output = compile_and_run(R"(

        main() {
            writeb(2 * 3 + '0');
            writeb(3 * 3 + '0');
            writeb('*n');
        }
    )");
    const std::string expect = "69\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, binary_divide)
{
    auto output = compile_and_run(R"(

        main() {
            writeb(18 / 3 + '0');
            writeb(207 / 23 + '0');
            writeb('*n');
        }
    )");
    const std::string expect = "69\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, unary_operators)
{
    auto output = compile_and_run(R"(
        main() {
            extrn x;
            auto y;

            printf("global -x = %d, expect %d*n", -x, -42);
            x = 0;
            printf("global !x = %d, expect %d*n", !x, 1);
            y = 987;
            x = &y;
            printf("global **x = %d, expect %d*n", *x, y);

            printf("local -y = %d, expect %d*n", -y, -987);
            y = 0;
            printf("local !y = %d, expect %d*n", !y, 1);
            x = 42;
            y = &x;
            printf("local **y = %d, expect %d*n", *y, x);
        }

        x 42;
    )");
    const std::string expect = R"(GLOBAL -X = -42, EXPECT -42
GLOBAL !X = 1, EXPECT 1
GLOBAL *X = 987, EXPECT 987
LOCAL -Y = -987, EXPECT -987
LOCAL !Y = 1, EXPECT 1
LOCAL *Y = 42, EXPECT 42
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, negation_in_conditional_context)
{
    auto output = compile_and_run(R"(
        main() {
            extrn x;
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
    )");
    const std::string expect = R"(X = 0, Y = 100
IF (X) CORRECT
IF (Y) CORRECT
IF (!X) CORRECT
IF (!Y) CORRECT
WHILE (!X) X = 0
)";
    EXPECT_EQ(output, expect);
}

//
// Operators ++ and -- are not supported by PDP-7 version of B compiler.
//
TEST_F(besm6, DISABLED_postfix_increment_decrement)
{
    auto output = compile_and_run(R"(
        incr(x) {
            printf("increment %d*n", x++);
            return (x);
        }

        add(a, b) {
            printf("add %d + %d*n", a, b);
            return (a + b);
        }

        decr(x) {
            printf("decrement %d*n", x--);
            return (x);
        }

        sub(a, b) {
            printf("subtract %d - %d*n", a, b);
            return (a - b);
        }

        main() {
            printf("%d*n", incr(42));
            printf("%d*n", add(42, 123));
            printf("%d*n", decr(42));
            printf("%d*n", sub(42, 123));
        }
    )");
    const std::string expect = R"(INCREMENT 42
43
ADD 42 + 123
165
DECREMENT 42
41
SUBTRACT 42 - 123
-81
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, assign_operators)
{
    auto output = compile_and_run(R"(
        asg_local(x) {
            auto result;

            printf("assign local %d*n", x);
            result = x;
            return (result);
        }

        asg_global(x) {
            extrn g;

            printf("assign global %d*n", x);
            g = x;
        }

        main() {
            extrn g;

            printf("%d*n", asg_local(42));
            asg_global(42);
            printf("%d*n", g);
        }

        g;
    )");
    const std::string expect = R"(ASSIGN LOCAL 42
42
ASSIGN GLOBAL 42
42
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, global_scalars)
{
    auto output = compile_and_run(R"(
        a;
        b 123;
        c 'foo';

        main() {
            extrn a, b, c;

            printf("a = %d*n", a);
            printf("b = %d*n", b);
            printf("c = '%c'*n", c);
        }
    )");
    const std::string expect = R"(A = 0
B = 123
C = 'FOO'
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, local_vector)
{
    auto output = compile_and_run(R"(
        main() {
            auto a;
            auto b 3;
            auto c;

            b = &b + 1;
            b[0] = 123;
            b[1] = 'local';
            b[2] = "string";
            printf("local b = %d, '%c', *"%s*"*n", b[0], b[1], b[2]);
            printf("offset a = %d*n", &a - &a);
            printf("offset b = %d*n", &b - &a);
            printf("offset c = %d*n", &c - &a);
        }
    )");
    const std::string expect = R"(LOCAL B = 123, 'LOCAL', "STRING"
OFFSET A = 0
OFFSET B = 1
OFFSET C = 5
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, global_vector)
{
    auto output = compile_and_run(R"(
        g[3] 123, -345, 'foo';

        main() {
            extrn g;

            printf("global = %d, %d, '%c'*n", g[0], g[1], g[2]);
            printf("address = %d, %d, %d*n", &g[0] - &g, &g[1] - &g, &g[2] - &g);
        }
    )");
    const std::string expect = R"(GLOBAL = 123, -345, 'FOO'
ADDRESS = 1, 2, 3
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, local_allocation)
{
    auto output = compile_and_run(R"(
        main() {
            auto p;
            auto a, b;
            auto c 1;
            auto d, e;

            p = &a;
            a = 11;
            b = 22;
            c = &c + 1;
            c[0] = 33;
            d = 44;
            e = 55;
            printf("%d %d %d %d %d*n", &a - &p, &b - &p, &c - &p, &d - &p, &e - &p);
            printf("%d %d %d %d %d %d*n", a, b, c - &c, c[0], d, e);
            printf("%d %d %d %d %d %d*n", p[0], p[1], p[2] - &c, p[3], p[4], p[5]);
        }
    )");
    const std::string expect = R"(1 2 3 5 6
11 22 1 33 44 55
11 22 1 33 44 55
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, binary_operators)
{
    auto output = compile_and_run(R"(
        x 42;

        main() {
            extrn x;
            auto y;

            y = 345;
            printf("%d + %d -> %d*n", x, y, x + y);
            printf("%d + %d -> %d*n", y, x, y + x);

            printf("%d - %d -> %d*n", x, y, x - y);
            printf("%d - %d -> %d*n", y, x, y - x);

            printf("%d ** %d -> %d*n", x, y, x * y);
            printf("%d ** %d -> %d*n", y, x, y * x);

            printf("%d / %d -> %d*n", x, y, x / y);
            printf("%d / %d -> %d*n", y, x, y / x);

            printf("%d %% %d -> %d*n", x, y, x % y);
            printf("%d %% %d -> %d*n", y, x, y % x);

            printf("%d < %d -> %d*n", x, y, x < y);
            printf("%d < %d -> %d*n", y, x, y < x);

            printf("%d <= %d -> %d*n", x, y, x <= y);
            printf("%d <= %d -> %d*n", y, x, y <= x);

            printf("%d > %d -> %d*n", x, y, x > y);
            printf("%d > %d -> %d*n", y, x, y > x);

            printf("%d >= %d -> %d*n", x, y, x >= y);
            printf("%d >= %d -> %d*n", y, x, y >= x);

            printf("%d == %d -> %d*n", x, y, x == y);
            printf("%d == %d -> %d*n", y, x, y == x);

            printf("%d != %d -> %d*n", x, y, x != y);
            printf("%d != %d -> %d*n", y, x, y != x);

            printf("%d & %d -> %d*n", x, y, x & y);
            printf("%d & %d -> %d*n", y, x, y & x);

            printf("%d | %d -> %d*n", x, y, x | y);
            printf("%d | %d -> %d*n", y, x, y | x);
        }
    )");
    const std::string expect = R"(42 + 345 -> 387
345 + 42 -> 387
42 - 345 -> -303
345 - 42 -> 303
42 * 345 -> 14490
345 * 42 -> 14490
42 / 345 -> 0
345 / 42 -> 8
42 % 345 -> 42
345 % 42 -> 9
42 < 345 -> 1
345 < 42 -> 0
42 <= 345 -> 1
345 <= 42 -> 0
42 > 345 -> 0
345 > 42 -> 1
42 >= 345 -> 0
345 >= 42 -> 1
42 == 345 -> 0
345 == 42 -> 0
42 != 345 -> 1
345 != 42 -> 1
42 & 345 -> 8
345 & 42 -> 8
42 | 345 -> 379
345 | 42 -> 379
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, eq_by_bitmask)
{
    auto output = compile_and_run(R"(

        main() {
            auto cval;

            cval = 51;
            if ((cval & 017777) == cval) {
                printf("Small positive: %d*n", cval);
            } else {
                printf("Wrong: %d*n", cval);
            }
        }
    )");
    EXPECT_EQ(output, "SMALL POSITIVE: 51\n");
}

TEST_F(besm6, octal_literals)
{
    auto output = compile_and_run(R"(

        main() {
            auto v;
            v = 012345;
            printf("%d*n", v);
            v = -04567;
            printf("%d*n", v);
        }
    )");
    const std::string expect = R"(5349
-2423
)";
    EXPECT_EQ(output, expect);
}
