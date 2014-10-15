#include "file_reference_delta.hpp"

#include "../../json_writer.hpp"

namespace zizany {
    file_reference_delta::file_reference_delta(const unity_file_reference &old_reference_, const unity_file_reference &new_reference_)
            : delta(), old_reference(old_reference_), new_reference(new_reference_) {
    }

    void
    file_reference_delta::print_action(json_writer &writer) const {
        writer.add_string("change file reference");
    }

    void
    file_reference_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("file_guid");
        old_reference.file_guid.print(writer);
        writer.add_key("old_reference");
        {
            writer.start_object();
            writer.add_key("path");
            writer.add_string(old_reference.path);
            writer.add_key("magic_byte_1");
            writer.add_number(old_reference.magic_byte_1);
            writer.add_key("magic_int_2");
            writer.add_number(old_reference.magic_int_2);
            writer.end_object();
        }
        writer.add_key("new_reference");
        {
            writer.start_object();
            writer.add_key("path");
            writer.add_string(new_reference.path);
            writer.add_key("magic_byte_1");
            writer.add_number(new_reference.magic_byte_1);
            writer.add_key("magic_int_2");
            writer.add_number(new_reference.magic_int_2);
            writer.end_object();
        }
        writer.end_object();
    }
}
