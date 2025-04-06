#include <fstream>

#include "fixture.h"

TEST_F(bcause, unary_operators)
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

TEST_F(bcause, negation_in_conditional_context)
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

TEST_F(bcause, postfix_operators)
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

        assign_local(x) {
            auto result;
            printf("assign local %d*n", x);
            result = x;
            return (result);
        }

        assign_global(x) {
            extrn g;

            printf("assign global %d*n", x);
            g = x;
        }

        main() {
            extrn g;

            printf("%d*n", incr(42));
            printf("%d*n", add(42, 123));
            printf("%d*n", decr(42));
            printf("%d*n", sub(42, 123));
            printf("%d*n", assign_local(42));
            assign_global(42);
            printf("%d*n", g);
        }

        g;
    )");
    const std::string expect = R"(increment 42
43
add 42 + 123
165
decrement 42
41
subtract 42 - 123
-81
assign local 42
42
assign global 42
42
)";
    EXPECT_EQ(output, expect);
}

TEST_F(bcause, local_array)
{
    auto output = compile_and_run(R"(
        main() {
            auto l[3];

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

TEST_F(bcause, global_array)
{
    auto output = compile_and_run(R"(
        g[3] -345, 'foo', "bar";

        main() {
            extrn g;

            printf("global = %d, '%c', *"%s*"*n", g[0], g[1], g[2]);
            printf("address = %d, %d, %d*n", (&g[0]) - g, (&g[1]) - g, (&g[2]) - g);
        }
    )");
    const std::string expect = R"(global = -345, 'foo', "bar"
address = 0, 8, 16
)";
    EXPECT_EQ(output, expect);
}

TEST_F(bcause, local_mix)
{
    auto output = compile_and_run(R"(
        main() {
            auto e, d;
            auto c[1];
            auto b, a;
            auto p;

            a = 11;
            b = 22;
            c[0] = 33;
            d = 44;
            e = 55;

            printf("%d %d %d %d", a, b, c - &c, c[0]);
            printf(" %d %d*n", d, e);
            p = &a;
            printf("%d %d %d %d", p[0], p[1], p[2] - &c, p[3]);
            printf(" %d %d*n", p[4], p[5]);
        }
    )");
    const std::string expect = R"(11 22 8 33 44 55
11 22 8 33 44 55
)";
    EXPECT_EQ(output, expect);
}

TEST_F(bcause, binary_operators)
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

TEST_F(bcause, priority_add_mul)
{
    auto output = compile_and_run(R"(
        main() {
            printf("3 + 4 ** 2 -> %d*n", 3 + 4 * 2);
        }
    )");
    EXPECT_EQ(output,  "3 + 4 * 2 -> 11\n");
}

TEST_F(bcause, priority_mul_add_mul)
{
    auto output = compile_and_run(R"(
        main() {
            printf("5 ** 2 + 3 ** 4 -> %d*n", 5 * 2 + 3 * 4);
        }
    )");
    EXPECT_EQ(output, "5 * 2 + 3 * 4 -> 22\n");
}

TEST_F(bcause, priority_sub_div)
{
    auto output = compile_and_run(R"(
        main() {
            printf("10 - 6 / 2 -> %d*n", 10 - 6 / 2);
        }
    )");
    EXPECT_EQ(output, "10 - 6 / 2 -> 7\n");
}

TEST_F(bcause, priority_mod_add)
{
    auto output = compile_and_run(R"(
        main() {
            printf("7 %% 3 + 2 -> %d*n", 7 % 3 + 2);
        }
    )");
    EXPECT_EQ(output, "7 % 3 + 2 -> 3\n");
}

TEST_F(bcause, priority_add_lt)
{
    auto output = compile_and_run(R"(
        main() {
            printf("5 + 3 < 9 -> %d*n", 5 + 3 < 9);
        }
    )");
    EXPECT_EQ(output, "5 + 3 < 9 -> 1\n");
}

TEST_F(bcause, priority_lt_eq)
{
    auto output = compile_and_run(R"(
        main() {
            printf("4 < 6 == 1 -> %d*n", 4 < 6 == 1);
        }
    )");
    EXPECT_EQ(output, "4 < 6 == 1 -> 1\n");
}

TEST_F(bcause, priority_eq_and)
{
    auto output = compile_and_run(R"(
        main() {
            printf("3 == 3 & 1 -> %d*n", 3 == 3 & 1);
        }
    )");
    EXPECT_EQ(output, "3 == 3 & 1 -> 1\n");
}

TEST_F(bcause, priority_and_or)
{
    auto output = compile_and_run(R"(
        main() {
            printf("2 & 3 | 4 -> %d*n", 2 & 3 | 4);
        }
    )");
    EXPECT_EQ(output, "2 & 3 | 4 -> 6\n");
}

TEST_F(bcause, priority_mul_add_lt)
{
    auto output = compile_and_run(R"(
        main() {
            printf("2 ** 3 + 4 < 11 -> %d*n", 2 * 3 + 4 < 11);
        }
    )");
    EXPECT_EQ(output, "2 * 3 + 4 < 11 -> 1\n");
}

TEST_F(bcause, priority_mul_ge_eq)
{
    auto output = compile_and_run(R"(
        main() {
            printf("5 ** 2 >= 10 == 1 -> %d*n", 5 * 2 >= 10 == 1);
        }
    )");
    EXPECT_EQ(output, "5 * 2 >= 10 == 1 -> 1\n");
}

TEST_F(bcause, priority_mul_and_add)
{
    auto output = compile_and_run(R"(
            printf("4 ** 2 & 3 + 1 -> %d*n", 4 * 2 & 3 + 1);
        main() {
        }
    )");
    EXPECT_EQ(output, "4 * 2 & 3 + 1 -> 0\n");
}

TEST_F(bcause, priority_div_add_gt_or)
{
    auto output = compile_and_run(R"(
        main() {
            printf("6 / 2 + 1 > 3 | 2 -> %d*n", 6 / 2 + 1 > 3 | 2);
        }
    )");
    EXPECT_EQ(output, "6 / 2 + 1 > 3 | 2 -> 3\n");
}

TEST_F(bcause, priority_div_mod)
{
    auto output = compile_and_run(R"(
        main() {
            printf("10 / 2 %% 3 -> %d*n", 10 / 2 % 3);
        }
    )");
    EXPECT_EQ(output, "10 / 2 % 3 -> 2\n");
}

TEST_F(bcause, priority_mul_or)
{
    auto output = compile_and_run(R"(
        main() {
            printf("0 ** 5 | 3 -> %d*n", 0 * 5 | 3);
        }
    )");
    EXPECT_EQ(output, "0 * 5 | 3 -> 3\n");
}

            /*
            printf("12345 + 6789 ** 10 -> %d*n", 12345 + 6789 * 10);
            printf("100000 - 25000 / 5 -> %d*n", 100000 - 25000 / 5);
            printf("98765 %% 1234 + 5678 -> %d*n", 98765 % 1234 + 5678);
            printf("5000 ** 20 + 3000 < 105000 -> %d*n", 5000 * 20 + 3000 < 105000);
            printf("10000 / 25 ** 4 == 1600 -> %d*n", 10000 / 25 * 4 == 1600);
            printf("123456 & 7890 -> %d*n", 123456 & 7890);
            printf("25000 ** 4 | 98765 -> %d*n", 25000 * 4 | 98765);
            printf("987654 / 321 + 456 > 3000 | 12345 -> %d*n", 987654 / 321 + 456 > 3000 | 12345);
            printf("(12345 + 67890) ** 2 -> %d*n", (12345 + 67890) * 2);
            printf("50000 - 20000 <= 30000 & 127 -> %d*n", 50000 - 20000 <= 30000 & 127);
            printf("9876543 / 123 %% 100 -> %d*n", 9876543 / 123 % 100);
            printf("45678 | 12345 < 60000 -> %d*n", 45678 | 12345 < 60000);
            printf("1000000 / 500 + 1500 != 3499 -> %d*n", 1000000 / 500 + 1500 != 3499);
            printf("123456 ** 2 / 3 + 789 > 82300 -> %d*n", 123456 * 2 / 3 + 789 > 82300);
            printf("987654 & 123456 | 1000000 -> %d*n", 987654 & 123456 | 1000000);
            */
    /*
    EXPECT_EQ(output[15], "12345 + 6789 * 10 -> 80235");
    EXPECT_EQ(output[16], "100000 - 25000 / 5 -> 95000");
    EXPECT_EQ(output[17], "98765 % 1234 + 5678 -> 6365");
    EXPECT_EQ(output[18], "5000 * 20 + 3000 < 105000 -> 1");
    EXPECT_EQ(output[19], "10000 / 25 * 4 == 1600 -> 1");
    EXPECT_EQ(output[20], "123456 & 7890 -> 6144");
    EXPECT_EQ(output[21], "25000 * 4 | 98765 -> 99837");
    EXPECT_EQ(output[22], "987654 / 321 + 456 > 3000 | 12345 -> 1");
    EXPECT_EQ(output[23], "(12345 + 67890) * 2 -> 160470");
    EXPECT_EQ(output[24], "50000 - 20000 <= 30000 & 127 -> 1");
    EXPECT_EQ(output[25], "9876543 / 123 % 100 -> 29");
    EXPECT_EQ(output[26], "45678 | 12345 < 60000 -> 1");
    EXPECT_EQ(output[27], "1000000 / 500 + 1500 != 3499 -> 1");
    EXPECT_EQ(output[28], "123456 * 2 / 3 + 789 > 82300 -> 1");
    EXPECT_EQ(output[29], "987654 & 123456 | 1000000 -> 1015806");
    */

// TODO: Examples Including << and >>
//  2 + 3 << 1 → + (precedence 2) before << (precedence 3) →  (2 + 3) << 1 ->  5 << 1 -> 10
// 16 >> 2 * 4 → * (precedence 1) before >> (precedence 3) → 16 >> (2 * 4) -> 16 >> 8 -> 0
// 8 << 2 < 33 → << (precedence 3) before < (precedence 4) → (8 << 2) < 33 -> 32 < 33 -> 1
//  4 & 3 << 1 → << (precedence 3) before & (precedence 6) →  (3 << 1) & 4 ->  6 & 4  -> 4

// TODO:
// evaluate("3 + 2 << 1", 7);
// evaluate("4 * 3 << 2", 48);
// evaluate("1 << 2 < 5", 1);
// evaluate("16 - 8 >> 1", 12);
// evaluate("3 << 2 & 7", 4);
// evaluate("2 | 4 >> 1", 2);
// evaluate("8 >> 2 == 2", 1);
// evaluate("5 * 2 << 1 + 3", 23);
// evaluate("15 % 4 << 2", 12);
// evaluate("1 << 3 > 5 & 2", 1);
// evaluate("12345 + 10 << 4", 12505);
// evaluate("16 / 2 >> 1", 4);
// evaluate("7 & 3 << 2 | 8", 12);
// evaluate("1 << 4 != 15", 1);
// evaluate("98765 >> 3 >= 12345", 1);
