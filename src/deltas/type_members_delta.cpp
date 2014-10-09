#include "type_members_delta.hpp"

#include "../json_writer.hpp"
#include "../unity_type.hpp"

namespace zizany {

    type_members_delta::type_members_delta(const delta_set_operation operation_, const type_identity &identity_, const member_path &path_, const unity_type &added_type_)
            : delta(),
              operation(operation_),
              identity(identity_),
              path(path_),
              added_type(added_type_) {
    }

    void
    type_members_delta::print_action(json_writer &writer) const {
        writer.add_string(operation == delta_set_operation::add ? "add type member" : "remove type member");
    }

    void
    type_members_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("identity");
        identity.print(writer);
        writer.add_key("path");
        path.print(writer);
        writer.add_key("added_type");
        added_type.print(writer, /*print_defaults:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
