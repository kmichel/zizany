#include "file_layout_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    file_layout_delta::file_layout_delta()
            : delta() {
    }

    void
    file_layout_delta::print_action(json_writer &writer) const {
        writer.add_string("change file layout");
    }

    void
    file_layout_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.end_object();
    }
}
