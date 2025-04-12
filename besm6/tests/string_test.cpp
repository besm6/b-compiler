#include <fstream>

#include "fixture.h"

TEST_F(besm6, string_literals)
{
    auto output = compile_and_run(R"(
        sa;
        sb;
        /* sc "bar"; not supported */

        main() {
            extrn sa, sb;

            sa = "*t*0x";
            sb = "123*e56";
            printf("*(*)***"*n");
            printf("%d %d %d*n", char(sa, 0), char(sa, 1), char(sa, 2));
            printf("%d %d %d %d %d*n", char(sb, 0), char(sb, 1), char(sb, 2), char(sb, 3), char(sb, 4));
        }
    )");
    const std::string expect = R"({}*"
9 0 120
49 50 51 4 53
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, char_literals)
{
    auto output = compile_and_run(R"(
        ca '*t*0x';
        cb '123*e45';
        cc 'bar';

        main() {
            extrn ca, cb;

            printf("%d*n", '*0');
            printf("%d*n", '*e');
            printf("%d*n", '*t');
            printf("%d*n", '*n');
            printf("%d*n", '*r');
            printf("%c*n", '*(*)***'*"');
        }
    )");
    const std::string expect = R"(0
4
9
10
13
{}*'"
)";
    EXPECT_EQ(output, expect);
}
