#include <parser.hpp>
#include <print>

void print_all_options(const Grep::Options& options) {
    std::println("print_only_pattern: {}", options.print_only_pattern);
    std::println("case_insensitive: {}", options.case_insensitive);
    std::println("pattern: {}", options.pattern);
    std::println("read_stdin: {}", options.read_stdin);
    std::println("file_object: {}", options.file_object.string());
}

void print_grep_output(const Grep::Options& options) {

}

int main(int argc, char** argv) {
    Grep::Options options { Grep::parse_options(std::span<char*>(argv, argc)) };
    print_all_options(options);
    return 0;
}
