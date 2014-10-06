cmake_minimum_required(VERSION 2.8.4)
project(zizany)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -pedantic")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
set(SOURCE_FILES
    file_lock.cpp
    guid.cpp
    json_writer.cpp
    main.cpp
    parse_exception.cpp
    program_options.cpp
    range_checker.cpp
    stream_parser.cpp
    unity_asset.cpp
    unity_file.cpp
    unity_file_parser.cpp
    unity_file_printer.cpp
    unity_file_reference.cpp
    unity_preview.cpp
    unity_type.cpp
    unity_value_parser.cpp
    values/unity_array_value.cpp
    values/unity_bool_value.cpp
    values/unity_bool_array_value.cpp
    values/unity_asset_reference_value.cpp
    values/unity_composite_value.cpp
    values/unity_guid_value.cpp
    values/unity_multiline_string_value.cpp
    values/unity_string_value.cpp
    values/unity_value.cpp
)
add_executable(zizany ${SOURCE_FILES} $<TARGET_OBJECTS:gdtoa>)

install(TARGETS zizany RUNTIME DESTINATION bin)

# Workaround for headers missing from project in CLion
# See: https://youtrack.jetbrains.com/issue/CPP-270
file(GLOB_RECURSE ALL_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_custom_target(clion_header_fix
    SOURCES ${ALL_HEADER_FILES}
)
