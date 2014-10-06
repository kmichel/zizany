#include "type_member_added_delta.hpp"

#include "../json_writer.hpp"
#include "../unity_type.hpp"

namespace zizany {

    type_member_added_delta::type_member_added_delta(const type_identity &identity_, const member_path &path_, const unity_type &added_type_)
            : delta("add type member"), identity(identity_), path(path_), added_type(added_type_) {
    }

    void
    type_member_added_delta::print_details(json_writer &writer) const {
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
