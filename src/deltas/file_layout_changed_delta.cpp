#include "file_layout_changed_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    file_layout_changed_delta::file_layout_changed_delta()
            : delta("change file layout") {
    }

    void file_layout_changed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.end_object();
    }
}
