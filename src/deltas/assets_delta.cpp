#include "assets_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    assets_delta::assets_delta(const delta_set_operation operation_, const int asset_id_, const type_identity &type_, const unity_value &value_)
            : delta(operation_ == delta_set_operation::add ? "add asset" : "remove asset"),
              operation(operation_),
              asset_id(asset_id_),
              type(type_),
              value(value_) {
    }

    void
    assets_delta::print_details(json_writer &writer) const {
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

