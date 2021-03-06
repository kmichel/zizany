cmake_minimum_required(VERSION 2.8.4)
project(zizany)
set(CMAKE_CXX_FLAGS_MINSIZEREAL "${CMAKE_CXX_FLAGS_MINSIZEREAL} -flto")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -pedantic -fvisibility=hidden -fvisibility-inlines-hidden")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-padded")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
set(SOURCE_FILES
    asset_reference.cpp
    diff/delta.cpp
    diff/delta_store.cpp
    diff/deltas/asset_delta.cpp
    diff/deltas/asset_type_delta.cpp
    diff/deltas/asset_type_2_delta.cpp
    diff/deltas/asset_members_delta.cpp
    diff/deltas/assets_delta.cpp
    diff/deltas/file_endianness_delta.cpp
    diff/deltas/file_layout_delta.cpp
    diff/deltas/file_magic_delta.cpp
    diff/deltas/file_reference_delta.cpp
    diff/deltas/file_references_delta.cpp
    diff/deltas/preview_delta.cpp
    diff/deltas/previews_delta.cpp
    diff/deltas/sequence_delta.cpp
    diff/deltas/type_delta.cpp
    diff/deltas/type_members_delta.cpp
    diff/deltas/types_delta.cpp
    diff/deltas/unity_version_delta.cpp
    diff/file_comparer.cpp
    diff/identified_type.cpp
    diff/member_path.cpp
    diff/preview_comparer.cpp
    diff/reverse_delta_store.cpp
    diff/sequence_comparer.cpp
    diff/simple_delta_store.cpp
    diff/type_comparer.cpp
    diff/type_identity.cpp
    file_stream.cpp
    file_lock.cpp
    guid.cpp
    json_writer.cpp
    main.cpp
    parse_exception.cpp
    program_options.cpp
    range_checker.cpp
    stream_parser.cpp
    type_factory.cpp
    unity_asset.cpp
    unity_file.cpp
    unity_file_parser.cpp
    unity_file_printer.cpp
    unity_file_reference.cpp
    unity_file_reference_properties.cpp
    unity_preview.cpp
    unity_type.cpp
    unity_value.cpp
    unity_value_member.cpp
    unity_value_members.cpp
    value_parser.cpp
    value_parsers/array_value_parser.cpp
    value_parsers/asset_reference_value_parser.cpp
    value_parsers/blob_value_parser.cpp
    value_parsers/bool_value_parser.cpp
    value_parsers/composite_value_parser.cpp
    value_parsers/double_value_parser.cpp
    value_parsers/fixed_size_blob_value_parser.cpp
    value_parsers/float_value_parser.cpp
    value_parsers/guid_value_parser.cpp
    value_parsers/inline_array_value_parser.cpp
    value_parsers/multiline_string_value_parser.cpp
    value_parsers/padded_value_parser.cpp
    value_parsers/string_value_parser.cpp
    value_parsers/tail_blob_value_parser.cpp
    values/unity_array_value.cpp
    values/unity_asset_reference_value.cpp
    values/unity_blob_value.cpp
    values/unity_bool_value.cpp
    values/unity_composite_value.cpp
    values/unity_double_value.cpp
    values/unity_float_value.cpp
    values/unity_guid_value.cpp
    values/unity_inline_array_value.cpp
    values/unity_multiline_string_value.cpp
    values/unity_string_value.cpp
)

add_executable(zizany ${SOURCE_FILES} $<TARGET_OBJECTS:gdtoa>)

install(TARGETS zizany RUNTIME DESTINATION bin)

# Workaround for headers missing from project in CLion
# See: https://youtrack.jetbrains.com/issue/CPP-270
file(GLOB_RECURSE ALL_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
add_custom_target(clion_header_fix
    SOURCES ${ALL_HEADER_FILES}
)
