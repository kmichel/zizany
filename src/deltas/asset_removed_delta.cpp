#include "asset_removed_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    asset_removed_delta::asset_removed_delta(const int asset_id_, const type_identity &type_, const unity_value &value_)
            : delta("remove asset"), asset_id(asset_id_), type(type_), value(value_) {
    }

    void
    asset_removed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("asset_id");
        writer.add_number(asset_id);
        writer.add_key("type");
        type.print(writer);
        writer.add_key("value");
        value.print(writer);
        writer.end_object();
    }
}

