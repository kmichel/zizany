#pragma once

#include "registry.hpp"

#include <memory>
#include <string>

namespace zizany {
    class stream_parser;

    class unity_file_reference;

    class unity_type;

    class unity_value;

    std::unique_ptr<unity_value> parse_value(stream_parser &parser, const unity_type &type, const std::string &member_name, const registry<unity_file_reference> &file_references);
}
