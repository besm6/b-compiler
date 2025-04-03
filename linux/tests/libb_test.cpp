#include <fstream>

#include "fixture.h"

TEST_F(bcause, libb_write)
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

TEST_F(bcause, libb_printf)
{
    auto output = compile_and_run(R"(
        main() {
            extrn printf;

            printf("Hello, World!*n");
            printf("%% %% %%%%*n");
            printf("format %%d: %d %d*n", 123, -123);
            printf("format %%o: %o %o*n", 234, -234);
            printf("format %%c: %c %c*n", 'foo', 'bar');
            printf("format %%s: *"%s*" *"%s*"*n", "Hello", "World");
        }
    )");
    const std::string expect = R"(Hello, World!
% % %%
format %d: 123 -123
format %o: 352 -352
format %c: foo bar
format %s: "Hello" "World"
)";
    EXPECT_EQ(output, expect);
}
