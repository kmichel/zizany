#include "preview_added_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    preview_added_delta::preview_added_delta(const unity_preview &preview_)
            : delta("add preview"), preview(preview_) {
    }

    void preview_added_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("preview");
        preview.print(writer, /*print_layout:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
