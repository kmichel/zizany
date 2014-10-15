#include "asset_type_2_delta.hpp"

#include "../json_writer.hpp"

namespace zizany {
    asset_type_2_delta::asset_type_2_delta(const int asset_id_, const int old_type_2_, const int new_type_2_)
            : asset_id(asset_id_), old_type_2(old_type_2_), new_type_2(new_type_2_) {
    }

    void
    asset_type_2_delta::print_action(json_writer &writer) const {
        writer.add_string("change asset type 2");
    }

    void
    asset_type_2_delta::print_details(json_writer &writer) const {
        writer.start_object();
        writer.add_key("asset_id");
        writer.add_number(asset_id);
        writer.add_key("old_type_2");
        writer.add_number(old_type_2);
        writer.add_key("new_type_2");
        writer.add_number(new_type_2);
        writer.end_object();
    }
}
