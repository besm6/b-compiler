#include <fstream>

#include "fixture.h"

TEST_F(besm6, example_fibonacci)
{
    auto output = compile_and_run(file_contents(TEST_DIR "/../../examples/fibonacci.b"));
    EXPECT_EQ(output, "55\n");
}
