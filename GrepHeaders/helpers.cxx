#include "helpers.hpp"
#include <iostream>

namespace Grep {
    void print_usage(int exit_code) {
        std::cerr << "Usage: grep [OPTIONS]... PATTERN [FILES]...\n";
        std::exit(exit_code);
    }

    void print_help() {
        std::cout << "Basic grep CLI utility with really basic and simple features.\n";
        std::cout << "Usage: grep [OPTIONS]... PATTERN [FILES]...\n";
        std::cout << "OPTIONS:\n";
        std::cout << "\t-i:\vEnable case-insensitive pattern matching.\n";
        std::cout << "\t-o:\vPrint only the matching pattern as the output with line numbers.";
        std::exit(EXIT_SUCCESS);
    }
}
