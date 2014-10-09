#include "previews_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    previews_delta::previews_delta(const delta_set_operation operation_, const unity_preview &preview_)
            : delta(operation_ == delta_set_operation::add ? "add preview" : "remove preview"),
              operation(operation_),
              preview(preview_) {
    }

    void previews_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("preview");
        preview.print(writer, /*print_layout:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
