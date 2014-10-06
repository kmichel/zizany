#include "preview_removed_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    preview_removed_delta::preview_removed_delta(const unity_preview &preview_)
            : delta("remove preview"), preview(preview_) {
    }

    void preview_removed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("preview");
        preview.print(writer, /*print_layout:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
