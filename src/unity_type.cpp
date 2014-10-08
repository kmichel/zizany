#include "unity_type.hpp"

#include "json_writer.hpp"

namespace zizany {
    unity_type::unity_type()
            : members(),
              name(),
              size(),
              magic_int_1(),
              magic_bitset_2() {
    }

    bool
    unity_type::requires_padding() const {
        return (magic_bitset_2 & 0x4000) != 0;
    }

    bool
    unity_type::is_simple() const {
        return members.size() == 0 && size != 0;
    }

    void
    unity_type::print(json_writer &writer, bool print_defaults, bool print_magic) const {
        writer.start_object();
        writer.add_key("name");
        writer.add_string(name);
        if (is_array || print_defaults) {
            writer.add_key("is_array");
            writer.add_bool(is_array);
        }
        if (size != -1 || print_defaults) {
            writer.add_key("size");
            writer.add_number(size);
        }
        if (print_magic) {
            writer.add_key("magic_int_1");
            writer.add_number(magic_int_1);
            writer.add_key("magic_bitset_2");
            writer.add_number(magic_bitset_2);
        }
        if (members.size() > 0 || print_defaults) {
            writer.add_key("members");
            writer.start_array();
            for (const unity_type_member &member : members) {
                writer.start_object();
                writer.add_key("name");
                writer.add_string(member.name);
                writer.add_key("type");
                member.type.print(writer, print_defaults, print_magic);
                writer.end_object();
            }
            writer.end_array();
        }
        writer.end_object();
    }
}
