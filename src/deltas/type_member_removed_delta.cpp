#include "type_member_removed_delta.hpp"

#include "../json_writer.hpp"
#include "../unity_type.hpp"

namespace zizany {

    type_member_removed_delta::type_member_removed_delta(const type_identity &identity_, const member_path &path_, const unity_type &removed_type_)
            : delta("remove type"), identity(identity_), path(path_), removed_type(removed_type_) {
    }

    void
    type_member_removed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("identity");
        identity.print(writer);
        writer.add_key("path");
        path.print(writer);
        writer.add_key("removed_type");
        removed_type.print(writer, /*print_defaults:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
