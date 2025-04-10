#include <fstream>

#include "fixture.h"

TEST_F(besm6, assign_global)
{
    auto output = compile_and_run(R"(
        main() {
            extrn x;
            x = 'foo*n';
            write(x);
            x = 'bar*n';
            write(x);
        }
        x;
    )");
    EXPECT_EQ(output, "foo\nbar\n");
}

TEST_F(besm6, assign_local)
{
    auto output = compile_and_run(R"(
        main() {
            auto x, y, z;
            x = 'foo*n';
            y = 'bar*n';
            z = 'done*n';
            write(x);
            write(y);
            write(z);
        }
    )");
    EXPECT_EQ(output, "foo\nbar\ndone\n");
}
