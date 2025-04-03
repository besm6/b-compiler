#include <fstream>

#include "fixture.h"

TEST_F(bcause, write)
{
    auto output = compile_and_run(R"(
        main() {
            extrn write;

            write('Hello,');
            write(' World');
            write('!*n');
        }
    )");
    const std::string expect = R"(Hello, World!
)";
    EXPECT_EQ(output, expect);
}
