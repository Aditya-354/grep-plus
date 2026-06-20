add_executable(grep)

target_sources(grep
    PRIVATE
    main.cxx
)

target_link_libraries(grep
    PRIVATE
    GrepHeaders
)
