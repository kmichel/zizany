#include "asset_member_removed_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    asset_member_removed_delta::asset_member_removed_delta(const int asset_id_, const member_path &path_, const unity_value &value_)
            : delta("remove asset member"), asset_id(asset_id_), path(path_), value(value_) {
    }

    void
    asset_member_removed_delta::print_details(json_writer &writer) const {
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

