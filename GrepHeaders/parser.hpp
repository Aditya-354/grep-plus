#pragma once

#include "parser_struct.hpp"
#include <span>

namespace Grep {
    Options parse_options(std::span<char*> arguments);
}
