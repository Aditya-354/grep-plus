#include <iostream>
#include <print>
#include <fstream>
#include <filesystem>
#include <span>
#include <cstdlib>

constexpr std::string_view case_insensitive_flag { "-i" };
constexpr std::string_view print_only_pattern_flag { "-o" };
constexpr std::string_view help_flag { "--help" };

struct Options {
    std::filesystem::path file_object {};
    std::string pattern {};
    bool print_only_pattern { false };
    bool case_insensitive { false };
    bool read_stdin { false };
};

void print_usage(int exit_code) {
    std::cerr << "Usage: grep [OPTIONS]... PATTERN [FILES]...\n";
    std::exit(exit_code);
}

void print_help() {
    std::cout << "Usage: grep [OPTIONS]... PATTERN [FILES]...\n";
    std::exit(EXIT_SUCCESS);
}

Options parse_options(std::span<char*> arguments) {
   Options options {};

   size_t i { 1 };
   while (i < arguments.size()) {
       std::string argument { arguments[i] };
       size_t next_index { i + 1 };

       if (argument == case_insensitive_flag) {
           if (next_index == arguments.size()) {
               print_usage(1);
               continue;
           } else options.case_insensitive = true;
       } else if (argument == print_only_pattern_flag) {
           if (next_index == arguments.size()) {
               print_usage(1);
               continue;
           } else options.print_only_pattern = true;
       } else if (options.pattern.size() == 0){
           options.pattern = argument;
           if (next_index == arguments.size()) options.read_stdin = true;
       } else if (std::filesystem::exists(std::filesystem::path(argument)))
           options.file_object = argument;
       else { std::cerr << "Error: File '" << argument << "' does not exists.\n"; continue; }

       i++;
   }

   return options;
}

void print_all_options(const Options& options) {
    std::println("print_only_pattern: {}", options.print_only_pattern);
    std::println("case_insensitive: {}", options.case_insensitive);
    std::println("pattern: {}", options.pattern);
    std::println("read_stdin: {}", options.read_stdin);
    std::println("file_object: {}", options.file_object.string());
}

void print_grep_output(const Options& options) {

}

int main(int argc, char** argv) {
    Options options { parse_options(std::span<char*>(argv, argc)) };
    print_all_options(options);
    return 0;
}
