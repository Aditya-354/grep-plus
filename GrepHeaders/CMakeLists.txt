add_library(GrepHeaders)

target_sources(GrepHeaders
    PRIVATE
    parser.cxx
    helpers.cxx

    PUBLIC
    FILE_SET HEADERS
    FILES
    parser.hpp
    parser_struct.hpp
    globals.hpp
    helpers.hpp
)
