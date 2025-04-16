#include <fstream>

#include "fixture.h"

// Not working yet: need KOI7 encoding in read().
TEST_F(besm6, DISABLED_native_empty_program)
{
    auto const b_besm = file_contents(TEST_DIR "/../b-besm.b");
    auto output = compile_and_run_with_input(b_besm, R"(
        main() {
            /* empty */
        }
    )");
    const std::string expect = R"( MAIN:8,NAME,
 PROGRAM:,ENTRY,
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
