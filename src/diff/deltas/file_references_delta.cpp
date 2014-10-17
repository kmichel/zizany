#include "file_references_delta.hpp"

#include "../../json_writer.hpp"

namespace zizany {
    file_references_delta::file_references_delta(delta_set_operation operation_, const unity_file_reference &reference_)
            : delta(),
              operation(operation_),
              reference(reference_) {
    }

    void
    file_references_delta::print_action(json_writer &writer) const {
        writer.add_string(operation == delta_set_operation::add ? "add file reference" : "remove file reference");
    }

    void
    file_references_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("reference");
        {
            writer.start_object();
            writer.add_key("file_guid");
            reference.file_guid.print(writer);
            writer.add_key("properties");
            reference.properties.print(writer, /*print_magic:*/true);
            writer.end_object();
        }
        writer.end_object();
    }
}
