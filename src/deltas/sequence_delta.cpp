#include "sequence_delta.hpp"

#include "../json_writer.hpp"
#include "../values/unity_value.hpp"

namespace zizany {
    sequence_delta::sequence_delta(
            const delta_sequence_operation operation_, const int asset_id_, const member_path &path_, const int position_, const unity_value &value_)
            : operation(operation_), asset_id(asset_id_), path(path_), position(position_), value(value_) {
    }

    void
    sequence_delta::print_action(json_writer &writer) const {
        writer.add_string(operation == delta_sequence_operation::insert ? "insert sequence element" : "remove sequence element");
    }

    void
    sequence_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("asset_id");
        writer.add_number(asset_id);
        writer.add_key("path");
        path.print(writer);
        writer.add_key("position");
        writer.add_number(position);
        writer.add_key("value");
        value.print(writer);
        writer.end_object();
    }
}
