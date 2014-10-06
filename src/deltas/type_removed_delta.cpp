#include "type_removed_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    type_removed_delta::type_removed_delta(const type_identity &identity_, const unity_type &type_)
            : delta("remove type"), identity(identity_), type(type_) {
    }

    void
    type_removed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("identity");
        identity.print(writer);
        writer.add_key("type");
        type.print(writer, /*print_defaults:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
