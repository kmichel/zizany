#pragma once

#include "guid.hpp"

namespace zizany {
    struct asset_reference {
    public:
        bool is_local;
        guid file_guid;
        int asset_id;
    };
}
