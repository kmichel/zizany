#include "unity_asset_reference_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_asset_reference_value::unity_asset_reference_value(const unity_type &type_)
            : unity_value(type_),
              file_reference_id(),
              asset_id() {
    }

    void
    unity_asset_reference_value::print(json_writer &writer) const {
        if (file_reference_id == 0) {
            if (asset_id == 0)
                writer.add_null();
            else
                writer.add_number(asset_id);
        } else {
            writer.start_object(/*inline:*/true);
            writer.add_key("file_reference_id");
            writer.add_number(file_reference_id);
            writer.add_key("asset_id");
            writer.add_number(asset_id);
            writer.end_object();
        }
    }
}
