#include "asset_type_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    asset_type_delta::asset_type_delta(const int asset_id_, const type_identity &old_type_, const type_identity &new_type_)
            : asset_id(asset_id_), old_type(old_type_), new_type(new_type_) {
    }

    void
    asset_type_delta::print_action(json_writer &writer) const {
        writer.add_string("change asset type");
    }

    void
    asset_type_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("asset_id");
        writer.add_number(asset_id);
        writer.add_key("old_type");
        old_type.print(writer);
        writer.add_key("new_type");
        new_type.print(writer);
        writer.end_object();
    }
}
