#include "asset_changed_delta.hpp"

#include "../values/unity_value.hpp"
#include "../json_writer.hpp"

namespace zizany {
    asset_changed_delta::asset_changed_delta(
            const int asset_id_,
            const member_path &path_,
            const unity_value &old_value_,
            const unity_value &new_value_)
            : delta("change asset"),
              asset_id(asset_id_),
              path(path_),
              old_value(old_value_),
              new_value(new_value_) {
    }

    void asset_changed_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("asset_id");
        writer.add_number(asset_id);
        writer.add_key("path");
        path.print(writer);
        writer.add_key("old_value");
        old_value.print(writer);
        writer.add_key("new_value");
        new_value.print(writer);
        writer.end_object();
    }
}
