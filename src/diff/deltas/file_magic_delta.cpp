#include "file_magic_delta.hpp"

#include "../../json_writer.hpp"

namespace zizany {
    file_magic_delta::file_magic_delta(const int magic_id_, const int old_value_, const int new_value_)
            : delta(), magic_id(magic_id_), old_value(old_value_), new_value(new_value_) {
    }

    void
    file_magic_delta::print_action(json_writer &writer) const {
        writer.add_string("change file magic");
    }

    void
    file_magic_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("magic_id");
        writer.add_number(magic_id);
        writer.add_key("old_value");
        writer.add_number(old_value);
        writer.add_key("new_value");
        writer.add_number(new_value);
        writer.end_object();
    }
}
