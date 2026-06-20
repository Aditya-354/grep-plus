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

void case_insensitive_output(std::ifstream& file, std::string& pattern) {

}

class GrepCLI {
    private:
        Grep::Options m_options {};
    public:
        GrepCLI(Grep::Options options)
            : m_options(options)
        {}

        void GrepOutput() {
            std::ifstream file { m_options.file_object.string() };
            std::string line {};
            size_t i { 1 };

            if (file.is_open()) {
                while (std::getline(file, line)) {
                    if (line.contains(m_options.pattern)) {
                        if (m_options.case_insensitive) {
                            case_insensitive_output(file, m_options.pattern);
                        } else 
                            std::println("{}:\t{}", i, line);
                    }
                    i++;
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
    grep.GrepOutput();
    return 0;
}
