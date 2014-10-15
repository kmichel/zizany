#pragma once


#include "../asset_reference.hpp"
#include <vector>

namespace zizany {
    class json_writer;

    class unity_file;

    class unity_type;

    struct type_identity {
        bool is_script;
        int type_id;
        asset_reference script_asset;

        explicit type_identity(const int type_id_);

        explicit type_identity(const asset_reference &script_asset_);

        const unity_type *resolve(const unity_file &file) const;

        void print(json_writer &writer) const;
    };

    type_identity get_type_identity(const unity_file &file, const int type_id);

    bool operator==(const type_identity &lhs, const type_identity &rhs);
}
