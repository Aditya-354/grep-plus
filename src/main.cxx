#include <parser.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <print>

void print_all_options(const Grep::Options& options) 
{
    std::println("print_only_pattern: {}", options.print_only_pattern);
    std::println("case_insensitive: {}", options.case_insensitive);
    std::println("pattern: {}", options.pattern);
    std::println("read_stdin: {}", options.read_stdin);
    std::println("file_object: {}", options.file_object.string());
}

constexpr std::string_view RED { "\x1b[31m" };
constexpr std::string_view GREEN { "\x1b[32m" };
constexpr std::string_view ESC { "\x1b[0m" };

class GrepCLI 
{
    private:
        Grep::Options m_options {};
    public:
        GrepCLI(Grep::Options options)
            : m_options(options)
        {}

        std::vector<size_t> ipattern(std::string& line, std::string& pattern) const
        {
            std::vector<size_t> positions {};

            if (pattern.size() > line.size())
            {
                return {};
            }

            for (size_t i { 0 }; i < line.size() - pattern.size(); ++i)
            {
                size_t j { 0 };

                while (j < pattern.size())
                {
                    if (m_options.case_insensitive)
                    {
                        if (std::tolower(line[i + j]) != std::tolower(pattern[j]))
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (line[i + j] != pattern[j])
                        {
                            break;
                        }
                    }

                    ++j;
                }

                if (j == pattern.size())
                {
                    positions.push_back(i);
                }
            }

            return positions;
        }

        std::string opattern(std::string& line, std::string& pattern) const
        {
            std::string res {};
            size_t j { 0 };

            if (line.size() < pattern.size()) return "";

            for (size_t i { 0 }; i < line.size() - pattern.size(); ++i)
            {
                j = 0;

                if (m_options.case_insensitive)
                {
                    while (j < pattern.size())
                    {
                        if (std::tolower(line[i + j]) != std::tolower(pattern[j]))
                        {
                            res = "";
                            break;
                        }

                        res += line[i + j];
                        ++j;
                        if (res.size() == pattern.size())
                        {
                            return res;
                        }
                    }
                }
                else
                {
                    while (j < pattern.size())
                    {
                        if (line[i + j] != pattern[j])
                        {
                            res = "";
                            break;
                        }

                        res += line[i + j];
                        ++j;
                        if (res.size() == pattern.size())
                        {
                            return res;
                        }
                    }
                }
            }
            return res;
        }

        std::string make_colored_line(std::string& line, std::vector<size_t>& positions)
        {
            std::string result {};
            size_t position_idx {};

            for (size_t i {}; i < line.size(); ++ i)
            {

                if (position_idx < positions.size() && i == positions[position_idx])
                {
                    result += RED;
                    result += line.substr(positions[position_idx], m_options.pattern.size());
                    result += ESC;
                    while (i < line.size() && i < positions[position_idx] + m_options.pattern.size())
                    {
                        i ++;
                    }
                    position_idx ++;
                }
                else
                {
                    result += line[i];
                }
            }
            return result;
        }

        void init_grep()
        {
            std::ifstream file { m_options.file_object.string() };
            std::istream& input { m_options.read_stdin ? std::cin : file };
            std::string line {};
            size_t i { 1 };

            if (input && file.is_open())
            {
                while (std::getline(input, line))
                {
                    if (m_options.case_insensitive)
                    {
                        std::vector<size_t> pattern_positions { ipattern(line, m_options.pattern) };

                        if (!pattern_positions.empty())
                        {
                            if (m_options.print_only_pattern)
                            {
                                std::println("{}{}{}:{}{}{}", GREEN, i, ESC, RED, opattern(line, m_options.pattern), ESC);
                            }
                            else
                            {
                                std::string colored_line  { make_colored_line(line, pattern_positions) };
                                std::println("{}{}{}:{}", GREEN, i, ESC, colored_line);
                            }
                        }
                    }
                    else if (m_options.print_only_pattern)
                    {
                        if (!line.contains(m_options.pattern))
                        { 
                            ++ i; 
                            continue;
                        }
                        std::vector<std::string> patterns_in_line {};
                        std::println("{}{}{}:{}{}{}", GREEN, i, ESC, RED, opattern(line, m_options.pattern), ESC);
                    }
                    else
                    {
                        if (!line.contains(m_options.pattern))
                        {
                            ++ i;
                            continue;
                        }
                        else
                        {
                            std::vector<size_t> pattern_positions { ipattern(line, m_options.pattern) };
                            std::string colored_line  { make_colored_line(line, pattern_positions) };
                            std::println("{}{}{}:{}", GREEN, i, ESC, colored_line);
                        }
                    }
                    ++ i;
                }

                file.close();
            }
            else
            {
                std::cerr << "Error: Unable to open file " << m_options.file_object << "\n";
                std::exit(3);
            }
        }
};


int main(int argc, char** argv)
{
    Grep::Options options { Grep::parse_options(std::span<char*>(argv, argc)) };
    print_all_options(options);

    GrepCLI grep { options };
    grep.init_grep();
    return 0;
}
