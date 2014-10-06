#include "unity_asset_reference_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_asset_reference_value::unity_asset_reference_value()
            : unity_value(), value() {
    }

    void
    unity_asset_reference_value::print(json_writer &writer) const {
        if (value.file_reference_id == 0) {
            if (value.asset_id == 0)
                writer.add_null();
            else
                writer.add_number(value.asset_id);
        } else {
            writer.start_object(/*inline:*/true);
            writer.add_key("file_reference_id");
            writer.add_number(value.file_reference_id);
            writer.add_key("asset_id");
            writer.add_number(value.asset_id);
            writer.end_object();
        }
    }

    bool
    unity_asset_reference_value::equals(const unity_value &value_) const {
        return value_.equals(value);
    }

    bool
    unity_asset_reference_value::equals(const asset_reference &value_) const {
        // XXX: this is wrong, we must replace file_reference_ids with guids
        return value_.file_reference_id == value.file_reference_id && value_.asset_id == value.asset_id;
    }
}
