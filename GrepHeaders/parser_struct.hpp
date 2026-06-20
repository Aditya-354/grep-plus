#pragma once

#include <filesystem>
#include <string>

namespace Grep {
    struct Options {
        std::filesystem::path file_object {};
        std::string pattern {};
        bool print_only_pattern { false };
        bool case_insensitive { false };
        bool read_stdin { false };
    };
}

