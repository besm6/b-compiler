#include <fstream>

#include "fixture.h"

TEST_F(besm6, native_empty_program)
{
    auto const b_besm = file_contents(TEST_DIR "/../b-besm.b");
    auto output = compile_and_run_with_input(b_besm, R"(
        main() {
            /* empty */
        }
    )");
    const std::string expect = R"( MAIN:8,NAME,
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
)";
    EXPECT_EQ(output, expect);
}
