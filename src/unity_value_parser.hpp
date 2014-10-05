#pragma once

#include <memory>

namespace zizany {
    class stream_parser;

    class unity_type;

    class unity_value;

    std::unique_ptr<unity_value> parse_value(stream_parser &parser, unity_type &type, unity_value *parent);
}
