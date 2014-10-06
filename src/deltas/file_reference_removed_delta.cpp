#include "file_reference_removed_delta.hpp"
#include "../json_writer.hpp"

namespace zizany {
    file_reference_removed_delta::file_reference_removed_delta(const unity_file_reference &reference_)
            : delta("remove file reference"), reference(reference_) {
    }

    void
    file_reference_removed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("reference");
        {
            writer.start_object();
            writer.add_key("file_guid");
            reference.file_guid.print(writer);
            writer.add_key("path");
            writer.add_string(reference.path);
            writer.add_key("magic_byte_1");
            writer.add_number(reference.magic_byte_1);
            writer.add_key("magic_int_2");
            writer.add_number(reference.magic_int_2);
            writer.end_object();
        }
        writer.end_object();
    }
}
