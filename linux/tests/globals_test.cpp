#include <fstream>

#include "fixture.h"

TEST_F(bcause, global_scalars)
{
    auto output = compile_and_run(R"(
        a;
        b 123;
        c -345, 'foo', "bar";

        main() {
            extrn a, b, c;

            printf("a = %d*n", a);
            printf("b = %d*n", b);
            printf("c = %d, '%c', *"%s*"*n", c[0], c[1], c[2]);
        }
    )");
    const std::string expect = R"(a = 0
b = 123
c = -345, 'foo', "bar"
)";
    EXPECT_EQ(output, expect);
}

TEST_F(bcause, global_vectors)
{
    auto output = compile_and_run(R"(
        a[];
        b[] 123;
        c[4] -345, 'foo', "bar";

        main() {
            extrn a, b, c;

            printf("a = %d*n", a);
            printf("b = %d*n", b);
            printf("c = %d, '%c', *"%s*", %d*n", c[0], c[1], c[2], c[3]);
        }
    )");
    const std::string expect = R"(a = 123
b = 123
c = -345, 'foo', "bar", 0
)";
    EXPECT_EQ(output, expect);
}

TEST_F(bcause, local_scalars)
{
    auto output = compile_and_run(R"(
        main() {
            auto a 123;
            auto b 'x';
            auto c;

            printf("offset a = %d*n", &a - &a);
            printf("offset b = %d*n", &b - &a);
            printf("offset c = %d*n", &c - &a);
        }
    )");
    const std::string expect = R"(offset a = 0
offset b = 992
offset c = 1952
)";
    EXPECT_EQ(output, expect);
}

TEST_F(bcause, local_vectors)
{
    auto output = compile_and_run(R"(
        main() {
            auto a[];
            auto b[123];
            auto c[];

            printf("offset a = %d*n", &a - &a);
            printf("offset b = %d*n", &b - &a);
            printf("offset c = %d*n", &c - &a);
        }
    )");
    const std::string expect = R"(offset a = 0
offset b = 0
offset c = 992
)";
    EXPECT_EQ(output, expect);
}
