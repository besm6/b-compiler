#include <fstream>

#include "fixture.h"

TEST_F(besm6, unary_negate)
{
    auto output = compile_and_run(R"(

        main() {
            auto v;
            v = -102;
            writeb(-v); /* 102 is ASCII 'f' */
            v = 145;
            writeb(-v); /* 256-145 -> 111 is ASCII 'o' */
            flush();
        }
    )");
    const std::string expect = "fo\n";
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
    const std::string expect = "Yes\nNo\n";
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
    const std::string expect = "No\nYes\n";
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
    const std::string expect = R"(False
True
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
    const std::string expect = R"(True
False
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
    const std::string expect = R"(True
False
False
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
    const std::string expect = R"(True
True
False
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
    const std::string expect = R"(False
False
True
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
    const std::string expect = R"(False
True
True
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
    const std::string expect = R"(global -x = -42, expect -42
global !x = 1, expect 1
global *x = 987, expect 987
local -y = -987, expect -987
local !y = 1, expect 1
local *y = 42, expect 42
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
    const std::string expect = R"(x = 0, y = 100
if (x) Correct
if (y) Correct
if (!x) Correct
if (!y) Correct
while (!x) x = 0
)";
    EXPECT_EQ(output, expect);
}

//
// Operators ++ and -- are not supported by PDP-7 version of B compiler
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
    const std::string expect = R"(increment 42
43
add 42 + 123
165
decrement 42
41
subtract 42 - 123
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
    const std::string expect = R"(assign local 42
42
assign global 42
42
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, local_array)
{
    auto output = compile_and_run(R"(
        main() {
            auto l 3;

            l = &l + 1;
            l[0] = 123;
            l[1] = 'local';
            l[2] = "string";
            printf("local = %d, '%c', *"%s*"*n", l[0], l[1], l[2]);
        }
    )");
    const std::string expect = R"(local = 123, 'local', "string"
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, global_array)
{
    auto output = compile_and_run(R"(
        g[3] 123, -345, 'foo';

        main() {
            extrn g;

            printf("global = %d, %d, '%c'*n", g[0], g[1], g[2]);
            printf("address = %d, %d, %d*n", &g[0] - &g, &g[1] - &g, &g[2] - &g);
        }
    )");
    const std::string expect = R"(global = 123, -345, 'foo'
address = 1, 2, 3
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
    EXPECT_EQ(output, "Small positive: 51\n");
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
