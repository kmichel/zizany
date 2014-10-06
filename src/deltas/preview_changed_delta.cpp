#include "preview_changed_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    preview_changed_delta::preview_changed_delta(const unity_preview &old_preview_, const unity_preview &new_preview_)
            : delta("change preview"), old_preview(old_preview_), new_preview(new_preview_) {
    }

    void preview_changed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("old_preview");
        old_preview.print(writer, /*print_layout:*/false, /*print_magic:*/true);
        writer.add_key("new preview");
        new_preview.print(writer, /*print_layout:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
