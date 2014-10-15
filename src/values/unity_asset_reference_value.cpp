#include "unity_asset_reference_value.hpp"

#include "../json_writer.hpp"

namespace zizany {
    unity_asset_reference_value::unity_asset_reference_value()
            : unity_value(), value() {
    }

    void
    unity_asset_reference_value::print(json_writer &writer) const {
        if (value.is_local) {
            if (value.asset_id == 0)
                writer.add_null();
            else
                writer.add_number(value.asset_id);
        } else {
            writer.start_object(/*inline:*/true);
            writer.add_key("file_guid");
            value.file_guid.print(writer);
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
        if (value_.is_local != value.is_local)
            return false;
        if (!value_.is_local && !value.is_local) if (value_.file_guid != value.file_guid)
            return false;
        return value_.asset_id == value.asset_id;
    }
}
