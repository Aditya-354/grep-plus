#include <parser.hpp>
#include <fstream>
#include <iostream>
#include <print>

void print_all_options(const Grep::Options& options) {
    std::println("print_only_pattern: {}", options.print_only_pattern);
    std::println("case_insensitive: {}", options.case_insensitive);
    std::println("pattern: {}", options.pattern);
    std::println("read_stdin: {}", options.read_stdin);
    std::println("file_object: {}", options.file_object.string());
}

void GrepOutput(const Grep::Options& options) {
    std::ifstream file { options.file_object.string() };
    std::string line {};
    size_t i { 1 };

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.contains(options.pattern)) {
                std::println("{}:\t{}", i, line);
            }
            i++;
        }

        file.close();
    } else {
        std::cerr << "Error: Unable to open file " << options.file_object << "\n";
    }
}

int main(int argc, char** argv) {
    Grep::Options options { Grep::parse_options(std::span<char*>(argv, argc)) };
    print_all_options(options);
    GrepOutput(options);
    return 0;
}
