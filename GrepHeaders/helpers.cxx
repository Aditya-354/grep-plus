#include "helpers.hpp"
#include <iostream>

namespace Grep {
    void print_usage(int exit_code) {
        std::cerr << "Usage: grep [OPTIONS]... PATTERN [FILES]...\n";
        std::exit(exit_code);
    }

    void print_help() {
        std::cout << "Usage: grep [OPTIONS]... PATTERN [FILES]...\n";
        std::exit(EXIT_SUCCESS);
    }
}
