#pragma once

#include <string_view>

namespace Grep {
    constexpr std::string_view case_insensitive_flag { "-i" };
    constexpr std::string_view print_only_pattern_flag { "-o" };
    constexpr std::string_view help_flag { "--help" };
    constexpr std::string_view GREP_RESULT_COLOR { "\x1b[31" };
}
