#include "asset_members_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    asset_members_delta::asset_members_delta(const delta_set_operation operation_, const int asset_id_, const member_path &path_, const unity_value &value_)
            : delta(),
              operation(operation_),
              asset_id(asset_id_),
              path(path_),
              value(value_) {
    }

    void
    asset_members_delta::print_action(json_writer &writer) const {
        writer.add_string(operation == delta_set_operation::add ? "add asset member" : "remove asset member");
    }

    void
    asset_members_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("asset_id");
        writer.add_number(asset_id);
        writer.add_key("path");
        path.print(writer);
        writer.add_key("value");
        value.print(writer);
        writer.end_object();
    }
}

