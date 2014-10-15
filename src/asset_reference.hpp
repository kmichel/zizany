#pragma once

#include "guid.hpp"

namespace zizany {
    class json_writer;

    struct asset_reference {
    public:
        bool is_local;
        guid file_guid;
        int asset_id;

        void print(json_writer &writer) const;
    };

    bool operator==(const asset_reference &lhs, const asset_reference &rhs);
}
