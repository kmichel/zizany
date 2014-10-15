#include "asset_reference.hpp"

#include "json_writer.hpp"

namespace zizany {
    void asset_reference::print(json_writer &writer) const {
        if (is_local) {
            if (asset_id == 0)
                writer.add_null();
            else
                writer.add_number(asset_id);
        } else {
            writer.start_object(/*inline:*/true);
            writer.add_key("file_guid");
            file_guid.print(writer);
            writer.add_key("asset_id");
            writer.add_number(asset_id);
            writer.end_object();
        }
    }

    bool operator==(const asset_reference &lhs, const asset_reference &rhs) {
        if (lhs.is_local != rhs.is_local)
            return false;
        if (!lhs.is_local && !rhs.is_local && (lhs.file_guid != rhs.file_guid))
            return false;
        return lhs.asset_id == rhs.asset_id;
    }
}
