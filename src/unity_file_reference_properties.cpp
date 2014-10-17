#include "unity_file_reference_properties.hpp"

#include "json_writer.hpp"

namespace zizany {
    void
    unity_file_reference_properties::print(json_writer &writer, bool print_magic) const {
        writer.start_object();
        writer.add_key("path");
        writer.add_string(path);
        if (print_magic) {
            writer.add_key("magic_byte_1");
            writer.add_number(magic_byte_1);
            writer.add_key("magic_int_2");
            writer.add_number(magic_int_2);
        }
        writer.end_object();
    }

    bool
    operator==(const unity_file_reference_properties &lhs, const unity_file_reference_properties &rhs) {
        return lhs.path == rhs.path && lhs.magic_int_2 == rhs.magic_int_2 && lhs.magic_byte_1 == rhs.magic_byte_1;
    }

    bool
    operator!=(const unity_file_reference_properties &lhs, const unity_file_reference_properties &rhs) {
        return lhs.path != rhs.path || lhs.magic_int_2 != rhs.magic_int_2 || lhs.magic_byte_1 != rhs.magic_byte_1;
    }
}
