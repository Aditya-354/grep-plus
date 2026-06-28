#include <parser.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <print>

constexpr std::string_view RED { "\x1b[31m" };
constexpr std::string_view GREEN { "\x1b[32m" };
constexpr std::string_view ESC { "\x1b[0m" };

class GrepCLI {
    private:
        Grep::Options m_options {};
    public:
        GrepCLI(Grep::Options options)
            : m_options(options)
        {}

    private:
        auto ipattern(const std::string& line, const std::string& pattern) const
            -> std::vector<size_t> {
            std::vector<size_t> positions {};

            if (pattern.size() > line.size()) {
                return {};
            }

            for (size_t i { 0 }; i <= line.size() - pattern.size(); ++i) {
                size_t j { 0 };

                while (j < pattern.size()) {
                    if (m_options.case_insensitive) {
                        if (std::tolower(line[i + j]) != std::tolower(pattern[j])) break;
                    }
                    else {
                        if (line[i + j] != pattern[j]) {
                            break;
                        }
                    }

                    ++j;
                }

                if (j == pattern.size()) {
                    positions.push_back(i);
                }
            }

            return positions;
        }

        auto color_the_pattern(const std::string& line, const std::vector<size_t>& positions)
            -> std::string {
            std::string result {};
            size_t position_idx {};

            for (size_t i {}; i < line.size(); ++ i) {
                if (position_idx < positions.size() && i == positions[position_idx]) {
                    result += RED;
                    result += line.substr(positions[position_idx], m_options.pattern.size());
                    result += ESC;
                    while (i < line.size() && i < positions[position_idx] + m_options.pattern.size() - 1) {
                        i ++;
                    }
                    position_idx ++;
                }
                else {
                    result += line[i];
                }
            }
            return result;
        }

        auto get_only_matched_pattern(const std::string& line, const std::vector<size_t>& positions) 
            -> std::vector<std::string> {
            std::vector<std::string> matched_only_patterns {};

            for (const auto& pos : positions) {
                matched_only_patterns.push_back(
                        std::string(RED) +
                        line.substr(pos, m_options.pattern.size()) +
                        std::string(ESC)
                        );
            }
            return matched_only_patterns;
        }

        auto process_grep(std::istream& input) 
            -> void {
            std::string line {};
            size_t i { 1 };
            while (std::getline(input, line)) {
                if (m_options.case_insensitive) {
                    std::vector<size_t> pattern_positions { ipattern(line, m_options.pattern) };

                    if (!pattern_positions.empty()) {
                        if (m_options.print_only_pattern) {
                            std::vector<std::string> matched_patterns { get_only_matched_pattern(line, pattern_positions) };

                            for (const auto& mp : matched_patterns) {
                                std::println("{}{}{}:{}", GREEN, i, ESC, mp);
                            }
                        }
                        else {
                            std::string colored_line  { color_the_pattern(line, pattern_positions) };
                            std::println("{}{}{}:{}", GREEN, i, ESC, colored_line);
                        }
                    }
                }
                else if (m_options.print_only_pattern) {
                    if (!line.contains(m_options.pattern)) { 
                        ++ i; 
                        continue;
                    }

                    std::vector<size_t> pattern_positions { ipattern(line, m_options.pattern) };
                    std::vector<std::string> matched_patterns { get_only_matched_pattern(line, pattern_positions) };

                    for (const auto& mp : matched_patterns) {
                        std::println("{}{}{}:{}", GREEN, i, ESC, mp);
                    }
                }
                else {
                    if (!line.contains(m_options.pattern)) {
                        ++ i;
                        continue;
                    }
                    else {
                        std::vector<size_t> pattern_positions { ipattern(line, m_options.pattern) };
                        std::string colored_pattern  { color_the_pattern(line, pattern_positions) };
                        std::println("{}{}{}:{}", GREEN, i, ESC, colored_pattern);
                    }
                }
                ++ i;
            }
        }

    public:
        auto init_grep() 
            -> void {
            std::ifstream file { m_options.file_object.string() };
            std::istream& input { m_options.read_stdin ? std::cin : file };
            size_t i { 1 };
            bool file_is_open { false };

            if (&input != &std::cin) {
                if (file.is_open()) {
                    file_is_open = true;
                }
                else {
                    std::cerr << "Error: Unable to open file " << m_options.file_object << "\n";
                }
            }

            process_grep(input);

            if (file_is_open) {
                file.close();
            }
        }
};


auto main(int argc, char** argv)
    -> int {
    Grep::Options options { Grep::parse_options(std::span<char*>(argv, argc)) };
    GrepCLI grep { options };
    grep.init_grep();
    return 0;
}

