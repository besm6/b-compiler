#include <fstream>

#include "fixture.h"

TEST_F(besm6, example_hello_printf)
{
    auto output = compile_and_run(file_contents(TEST_DIR "/../../examples/helloworld.b"));
    EXPECT_EQ(output, "Hello, World!\n");
}
