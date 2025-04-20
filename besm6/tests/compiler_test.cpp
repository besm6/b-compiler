#include <fstream>

#include "fixture.h"

TEST_F(besm6, DISABLED_native_empty_program)
{
    auto const b_besm = file_contents(TEST_DIR "/../b-besm.b");
    auto output = compile_and_run_with_input(b_besm, R"(
        main() {
            /* empty */
        }
    )");
    const std::string expect = R"( MAIN:,NAME,
 PROGRAM:,ENTRY,
 MAIN:,BSS,
 B/SAVE0:,SUBP,
 B/RET:,SUBP,
    ,ITS,13
    ,CALL,B/SAVE0
   5,BASE,*+10000B
    ,UJ, B/RET
    ,END,
)";
    EXPECT_EQ(output, expect);
}
