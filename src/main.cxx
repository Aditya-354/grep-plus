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

constexpr std::string_view GREP_RESULT_COLOR { "\x1b[31" };

class GrepCLI {
    private:
        Grep::Options m_options {};
    public:
        GrepCLI(Grep::Options options)
            : m_options(options)
        {}

        bool ipattern(std::string& line, std::string& pattern) const {
            bool is_pattern_case_insensitive { false };
            if (pattern.size() > line.size())
                return is_pattern_case_insensitive;

            for (size_t i { 0 }; i < line.size() - pattern.size(); ++i) {
                size_t j { 0 };

                while (j < pattern.size()) {
                    if (std::tolower(line[i + j]) != std::tolower(pattern[j]))
                        break;

                    ++j;
                }

                if (j == pattern.size()) is_pattern_case_insensitive = true;
            }

            return is_pattern_case_insensitive;
        }

        std::string opattern(std::string& line, std::string& pattern) const {
            std::string res {};
            size_t j { 0 };

            for (size_t i { 0 }; i < line.size() - pattern.size(); ++i) {
                j = 0;

                while (j < pattern.size()) {
                    if (std::tolower(line[i + j]) != std::tolower(pattern[j])) {
                        res = "";
                        break;
                    }

                    res += line[i + j];
                    ++j;
                }
            }

            return res;
        }

        void start_grep() {
            std::ifstream file { m_options.file_object.string() };
            std::string line {};
            size_t i { 1 };

            if (file.is_open()) {
                while (std::getline(file, line)) {
                    if (m_options.case_insensitive) {
                        if (ipattern(line, m_options.pattern)) {
                            if (m_options.print_only_pattern) {
                                std::println("{}:\t{}", i, opattern(line, m_options.pattern));
                            }
                            std::println("{}:\t{}", i, line);
                        }
                    }
                    ++i;
                }

                file.close();
            } else {
                std::cerr << "Error: Unable to open file " << m_options.file_object << "\n";
            }
        }
};


int main(int argc, char** argv) {
    Grep::Options options { Grep::parse_options(std::span<char*>(argv, argc)) };
    print_all_options(options);

    GrepCLI grep { options };
    grep.start_grep();
    return 0;
}
