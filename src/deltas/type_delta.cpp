#include "type_delta.hpp"

#include "../json_writer.hpp"
#include "../unity_type.hpp"

namespace zizany {
    type_delta::type_delta(
            type_identity identity_,
            const member_path &path_,
            const unity_type &old_definition_,
            const unity_type &new_definition_)
            : delta("change type"),
              identity(identity_),
              path(path_),
              old_definition(old_definition_),
              new_definition(new_definition_) {
    }

    void
    type_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("identity");
        identity.print(writer);
        writer.add_key("path");
        path.print(writer);
        writer.add_key("old_definition");
        old_definition.print(writer, /*print_defaults:*/false, /*print_magic:*/true);
        writer.add_key("new_definition");
        new_definition.print(writer, /*print_defaults:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
