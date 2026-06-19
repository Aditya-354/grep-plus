// grep command in C++.

/* Expected working:
 * ./gp -i -o 'pattern' filename
 * ./gp -i 'pattern' filename
 * ./gp -o 'pattern filename
*/

#include <iostream>
#include <string>
#include <string_view>
#include <print>
#include <filesystem>
#include <fstream>
#include <optional>
#include <span>

namespace fs = std::filesystem;

constexpr std::string_view case_insensitive_flag { "-i" };
constexpr std::string_view print_only_pattern_flag { "-o" };
constexpr std::string_view print_Help { "--help" };

struct Options {
    std::optional<fs::path> file_object {};
    std::string pattern {};
    bool case_insensitive { false };
    bool print_only_pattern { false };
};

int print_help(int exit_code) {
    std::println("Usage: grep [OPTIONS]... pattern [FILE]...");
    std::println("Options:");
    std::println("-i: Enable case-insensitive search.");
    std::println("-o: Print only the matching patterns.");
    exit(exit_code);
}

Options parse_print_options(std::span<char*> arguments) {
    Options options {};
    size_t i { 1 };

    while (i < arguments.size()) {
        std::string argument { arguments[i++] };

        if (argument == case_insensitive_flag) {
            if (i == arguments.size()) {
                print_help(1);
                continue;
            } else options.case_insensitive = true;
        } else if (argument == print_only_pattern_flag) {
            if (i == arguments.size()) {
                print_help(1);
                continue;
            } else options.print_only_pattern = true;
        } else if (argument == print_Help) {
            print_help(0);
            continue;
        } else {
            if (argument.starts_with('-')) {
                std::cerr << "Invalid option. Run 'gp --help' for more information.\n";
                continue;
            } else options.pattern = argument;
        }

        if (i == arguments.size()) {
            fs::path user_path_to_file { argument };
            if (fs::exists(user_path_to_file)) {
                if (!fs::is_regular_file(user_path_to_file)) {
                    std::cerr << "Error: Not a file.\n";
                    continue;
                } else {
                    std::cout << "File exists!\n";
                    options.file_object = user_path_to_file;
                }
            } else {
                std::cerr << arguments.size() << "\n";
                std::cerr << "Error: " << argument << " does not exist!\n";
                continue;
            }
        }
    }

    return options;
}

class Grep {
    private:
        Options m_options {};
    public:
        Grep(Options& print_options)
            : m_options(print_options)
        {
            std::cout << "Grep class constructed successfully!\n";
            std::cout << "printing options struct: \n";
            std::cout << m_options.case_insensitive << "\n";
            std::cout << m_options.print_only_pattern << "\n";
            if (m_options.file_object.has_value()) {
                std::cout << m_options.file_object->string() << std::endl;
            }
        }

};

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) {
    if (argc < 2) {
        std::cerr << "usage: gp [OPTION]... PATTERN [FILE]...\n"
            "Try 'grep --help' for more information.\n";
        return 1;
    }

    Options print_options { parse_print_options(std::span<char*>(argv, argc)) };

    Grep grep { print_options };
    return 0;
}
