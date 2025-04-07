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
  7,mtj,15
  7,stx,-3
   ,sti,13
   ,sti,7
 13,uj,
   ,end,
)");
}
