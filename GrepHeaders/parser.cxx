#include "parser.hpp"
#include "parser_struct.hpp"
#include "helpers.hpp"
#include "globals.hpp"

#include <string>
#include <filesystem>
#include <iostream>

namespace Grep {
    Options parse_options(std::span<char*> arguments) {
        if (arguments.size() < 2) print_usage(1);
        Options options {};

        size_t i { 1 };
        while (i < arguments.size()) {
            std::string argument { arguments[i] };
            size_t next_index { i + 1 };

            if (argument == case_insensitive_flag) {
                if (next_index == arguments.size()) {
                    print_usage(2);
                    continue;
                } else options.case_insensitive = true;
            } else if (argument == print_only_pattern_flag) {
                if (next_index == arguments.size()) {
                    print_usage(2);
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
}
