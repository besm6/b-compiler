#include <fstream>

#include "fixture.h"

TEST_F(besm6, global_var)
{
    auto output = compile(R"(
        foo;
    )");
    EXPECT_EQ(output, R"( foo:,name,
    ,oct,
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
    ,its,7
    ,its,13
    ,its,
  15,mtj,7
   7,stx,-3
    ,sti,13
    ,sti,7
  13,uj,
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
    ,its,7
    ,its,13
    ,its,
  15,mtj,7
 write:,subp,
  14,vtm,write
    ,ita,14
    ,xts,=2206255433067412
  15,wtc,
 :13,31,
    ,utc,1
   7,stx,-3
    ,sti,13
    ,sti,7
  13,uj,
    ,end,
)");
}
