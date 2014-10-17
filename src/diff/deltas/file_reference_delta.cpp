#include "file_reference_delta.hpp"

#include "../../json_writer.hpp"

namespace zizany {
    file_reference_delta::file_reference_delta(const guid &file_guid_, const unity_file_reference_properties &old_properties_, const unity_file_reference_properties &new_properties_)
            : delta(), file_guid(file_guid_), old_properties(old_properties_), new_properties(new_properties_) {
    }

    void
    file_reference_delta::print_action(json_writer &writer) const {
        writer.add_string("change file reference");
    }

    void
    file_reference_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("file_guid");
        file_guid.print(writer);
        writer.add_key("old_properties");
        old_properties.print(writer, /*print_magic:*/true);
        writer.add_key("new_properties");
        new_properties.print(writer, /*print_magic:*/true);
        writer.end_object();
    }
}
