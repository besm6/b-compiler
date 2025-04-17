#include <fstream>

#include "fixture.h"

TEST_F(besm6, libb_flush_writeb)
{
    auto output = compile_and_run(R"(
        main() {
            writeb(060); /* 0 */
            writeb(10);
            writeb(053); /* + */
            writeb(10);
            writeb(061); /* 1 */
            flush();
        }
    )");
    const std::string expect = R"(0
+
1
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_okno)
{
    auto output = compile_and_run(R"(
        main() {
            OKHO();
        }
    )");
    const std::string expect = "0000000000001054          007   00000 00000 00000 00000 11001 53401 53406 00000 00000 00000 00000 00000 01004 01054 53406\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_write)
{
    auto output = compile_and_run(R"(
        main() {
            write('Hello,');
            write(' World');
            write('!*n');
        }
    )");
    const std::string expect = "HELLO, WORLD!\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_printd)
{
    auto output = compile_and_run(R"(
        main() {
            printd(12345);
            flush();
            printd(-56789);
            flush();
        }
    )");
    const std::string expect = R"(12345
-56789
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_printo)
{
    auto output = compile_and_run(R"(
        main() {
            printo(012345);
            flush();
            printo(-034567);
            flush();
        }
    )");
    const std::string expect = R"(12345
37777777743211
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_printf)
{
    auto output = compile_and_run(R"(
        main() {
            printf("Hello, World!*n");
            printf("%% %% %%%%*n");
            printf("format %%d: %d %d*n", 123, -123);
            printf("format %%o: %o %o*n", 234, -234);
            printf("format %%c: %c %c*n", 'foo', 'bar');
            printf("format %%s: *"%s*" *"%s*"*n", "Hello", "World");
            printf("unknown format: %q*n", "foo");
        }
    )");
    const std::string expect = R"(HELLO, WORLD!
% % %%
FORMAT %D: 123 -123
FORMAT %O: 352 37777777777426
FORMAT %C: FOO BAR
FORMAT %S: "HELLO" "WORLD"
UNKNOWN FORMAT: %Q
)";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_exit)
{
    auto output = compile_and_run(R"(
        main() {
            printf("before exit()*n");
            exit();
            printf("after exit()*n");
        }
    )");
    const std::string expect = "BEFORE EXIT()\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_char)
{
    auto output = compile_and_run(R"(
        main() {
            write(char("fubar", 2));
            write(char("fubar", 4));
            write(char("fubar", 1));
            write(char("fubar", 0));
            write(char("fubar", 3));
            write('*n');
        }
    )");
    const std::string expect = "BRUFA\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_lchar)
{
    auto output = compile_and_run(R"(
        main() {
            auto str;

            lchar(&str, 0, 'f');
            lchar(&str, 1, 'u');
            lchar(&str, 2, 'b');
            lchar(&str, 3, 'a');
            lchar(&str, 4, 'r');
            lchar(&str, 5, 0);
            write(str);
            write('*n');
        }
    )");
    const std::string expect = "FUBAR\n";
    EXPECT_EQ(output, expect);
}

TEST_F(besm6, libb_read)
{
    auto output = compile_and_run_with_input(R"(
        main() {
            auto ch;

            while (ch = read()) {
                write(ch);
            }
        }
    )", R"(main() {
    printf("Hello, World!*n");
}
)");
    const std::string expect = R"(MAIN() ≤
    PRINTF("HELLO, WORLD!*N");
≥
)";
    EXPECT_EQ(output, expect);
}
