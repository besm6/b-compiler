#include <fstream>

#include "fixture.h"

TEST_F(besm6, global_var)
{
    auto output = compile(R"(
        foo;
    )");
    EXPECT_EQ(output, ".foo:0\n");
}
