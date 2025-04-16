#include <fstream>

#include "fixture.h"

TEST_F(besm6, global_var)
{
    auto output = compile(R"(
        foo;
    )");
    EXPECT_EQ(output, R"( FOO:,NAME,
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
    EXPECT_EQ(output, R"( MAIN:8,NAME,
 PROGRAM:,ENTRY,
   8,VTM,20000B
   9,VTM,40000B
  10,VTM,60000B
 MAIN:,BSS,
 B/SAVE0:,SUBP,
 B/RET:,SUBP,
    ,ITS,13
    ,CALL,B/SAVE0
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
    EXPECT_EQ(output, R"( MAIN:8,NAME,
 PROGRAM:,ENTRY,
   8,VTM,20000B
   9,VTM,40000B
  10,VTM,60000B
 MAIN:,BSS,
 B/SAVE0:,SUBP,
 B/RET:,SUBP,
    ,ITS,13
    ,CALL,B/SAVE0
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
