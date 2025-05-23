#include "fixture.h"

#include <fstream>
#include <cstring>

static void run_command(std::string &result, const std::string &cmd)
{
    // Run simulator via shell.
    std::cout << cmd << '\n';
    FILE *pipe = popen(cmd.c_str(), "r");
    ASSERT_TRUE(pipe != nullptr);

    // Capture output.
    result = stream_contents(pipe);
    std::cout << result;

    // Check exit code.
    int exit_status = pclose(pipe);
    int exit_code   = WEXITSTATUS(exit_status);
    ASSERT_NE(exit_status, -1);
    ASSERT_EQ(exit_code, 0);
}

//
// Compile B code.
// Return assembly output.
//
std::string besm6::compile(const std::string &source_code)
{
    const auto b_filename = test_name + ".b";

    create_file(b_filename, source_code);

    // Compile B source into assembly code.
    std::string result;
    run_command(result, TEST_DIR "/../bbesm < " + b_filename + " 2>&1");

    // Return output.
    return result;
}

//
// Compile B program for besm6 and run it.
// Return output.
//
std::string besm6::compile_and_run(const std::string &source_code)
{
    const auto b_filename   = test_name + ".b";
    const auto asm_filename = test_name + ".assem";
    const auto exe_filename = test_name + ".exe";

    create_file(b_filename, source_code);

    // Compile B source into assembly code.
    std::string result;
    run_command(result, TEST_DIR "/../bbesm < " + b_filename + " > " + asm_filename);

    // Compile assembly code with B library into executable.
    run_command(result, "besmc " + asm_filename + " " + TEST_DIR "/../libb/libb.obj");

    // Run executable.
    // Return output.
    run_command(result, "./" + exe_filename);
    return result;
}

//
// Compile B program and run it with given input.
// Return output.
//
std::string besm6::compile_and_run_with_input(const std::string &source_code, const std::string &input_data)
{
    const auto b_filename   = test_name + ".b";
    const auto asm_filename = test_name + ".assem";
    const auto exe_filename = test_name + ".exe";
    const auto in_filename  = test_name + ".input";

    create_file(b_filename, source_code);
    create_file(in_filename, input_data);

    // Compile B source into assembly code.
    std::string result;
    run_command(result, TEST_DIR "/../bbesm < " + b_filename + " > " + asm_filename);

    // Compile assembly code with B library into executable.
    run_command(result, "besmc " + asm_filename + " " + TEST_DIR "/../libb/libb.obj");

    // Run executable with input.
    // Return output.
    run_command(result, "./" + exe_filename + " < " + in_filename);
    return result;
}

//
// Read file contents and return it as a string.
//
std::string file_contents(const std::string &filename)
{
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << filename << ": " << std::strerror(errno) << std::endl;
        return "";
    }
    std::stringstream contents;
    contents << input.rdbuf();
    return contents.str();
}

//
// Read file contents as vector of strings.
//
std::vector<std::string> split_stream(std::istream &input)
{
    std::vector<std::string> output;
    std::string line;
    while (std::getline(input, line)) {
        output.push_back(line);
    }
    return output;
}

//
// Read file contents as vector of strings.
//
std::vector<std::string> file_contents_split(const std::string &filename)
{
    std::ifstream input(filename);
    return split_stream(input);
}

//
// Read file contents as vector of strings.
//
std::vector<std::string> multiline_split(const std::string &multiline)
{
    std::stringstream input(multiline);
    return split_stream(input);
}

//
// Read FILE* stream contents until EOF and return it as a string.
//
std::string stream_contents(FILE *input)
{
    std::stringstream contents;
    char line[256];
    while (fgets(line, sizeof(line), input)) {
        contents << line;
    }
    return contents.str();
}

//
// Create file with given contents.
//
void create_file(const std::string &filename, const std::string &contents)
{
    std::ofstream output(filename);
    output << contents;
}

//
// Create file with given contents.
//
void create_file(const std::string &dest_filename, const std::string &prolog,
                 const std::string &src_filename, const std::string &epilog)
{
    std::ofstream output(dest_filename);
    EXPECT_TRUE(output.is_open()) << dest_filename;

    std::ifstream input(src_filename);
    EXPECT_TRUE(input.is_open()) << src_filename;

    output << prolog;
    output << input.rdbuf();
    output << epilog;
}

//
// Check whether string starts with given prefix.
//
bool starts_with(const std::string &str, const char *prefix)
{
    auto prefix_size = strlen(prefix);
    return str.size() >= prefix_size && memcmp(str.c_str(), prefix, prefix_size) == 0;
}
