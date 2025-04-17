#include <fstream>

#include "fixture.h"

TEST_F(besm6, global_var)
{
    auto output = compile(R"(
        foo_$;
    )");
    EXPECT_EQ(output, R"( FOO*/:,NAME,
    ,BSS,1
    ,END,
)");
}

TEST_F(besm6, empty_program)
{
    auto output = compile(R"(
        main() {
            /* empty */
        }
    )");
    EXPECT_EQ(output, R"( MAIN:,NAME,
 PROGRAM:,ENTRY,
 MAIN:,BSS,
 B/SAVE0:,SUBP,
 B/RET:,SUBP,
    ,ITS,13
    ,CALL,B/SAVE0
   5,BASE,*+10000B
    ,UJ, B/RET
    ,END,
)");
}

TEST_F(besm6, hello_write)
{
    auto output = compile(R"(
        main() {
            write('Hello*n');
        }
    )");
    EXPECT_EQ(output, R"( MAIN:,NAME,
 PROGRAM:,ENTRY,
 MAIN:,BSS,
 B/SAVE0:,SUBP,
 B/RET:,SUBP,
    ,ITS,13
    ,CALL,B/SAVE0
   5,BASE,*+10000B
 WRITE:,SUBP,
  14,VTM,WRITE
    ,ITA,14
    ,XTS,=2204251423047412
  14,VTM,-1
  15,WTC,
  13,VJM,
    ,UJ, B/RET
    ,END,
)");
}
