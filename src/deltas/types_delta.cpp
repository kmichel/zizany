#include "types_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    types_delta::types_delta(const delta_set_operation operation_, const type_identity &identity_, const unity_type &type_)
            : delta(),
              operation(operation_),
              identity(identity_),
              type(type_) {
    }

    void
    types_delta::print_action(json_writer &writer) const {
        writer.add_string(operation == delta_set_operation::add ? "add type" : "remove type");
    }

    void
    types_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("identity");
        identity.print(writer);
        writer.add_key("type");
        type.print(writer, /*print_defaults:*/false, /*print_magic:*/true);
        writer.end_object();
    }
}
