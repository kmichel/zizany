#include "preview_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    preview_delta::preview_delta(const unity_preview &old_preview_, const unity_preview &new_preview_)
            : delta(), old_preview(old_preview_), new_preview(new_preview_) {
    }

    void
    preview_delta::print_action(json_writer &writer) const {
        writer.add_string("change preview");
    }

    void
    preview_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("old_preview");
        old_preview.print(writer, /*print_layout:*/false, /*print_magic:*/true);
        writer.add_key("new preview");
        new_preview.print(writer, /*print_layout:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
