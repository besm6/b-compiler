#include <fstream>

#include "fixture.h"

TEST_F(besm6, global_var)
{
    auto output = compile(R"(
        foo;
    )");
    EXPECT_EQ(output, R"( foo:,name,
    ,bss,1
    ,end,
)");
}

TEST_F(besm6, empty_program)
{
    auto output = compile(R"(
        main() {
            /* empty */
        }
    )");
    EXPECT_EQ(output, R"( main:,name,
 program:,entry,
 main:,bss,
 b/save0:,subp,
 b/ret:,subp,
    ,its,13
    ,call,b/save0
    ,uj, b/ret
    ,end,
)");
}

TEST_F(besm6, hello_write)
{
    auto output = compile(R"(
        main() {
            write('Hello*n');
        }
    )");
    EXPECT_EQ(output, R"( main:,name,
 program:,entry,
 main:,bss,
 b/save0:,subp,
 b/ret:,subp,
    ,its,13
    ,call,b/save0
 write:,subp,
  14,vtm,write
    ,ita,14
    ,xts,=2206255433067412
  14,vtm,-1
  15,wtc,
  13,vjm,
    ,uj, b/ret
    ,end,
)");
}
