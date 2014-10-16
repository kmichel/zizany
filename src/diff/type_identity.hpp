#pragma once

#include "../asset_reference.hpp"

namespace zizany {
    class json_writer;

    class unity_file;

    class unity_type;

    enum class type_kind {
        builtin_type,
        script_type
    };

    struct type_identity {
        type_kind kind;
        int type_id;
        asset_reference script_asset;

        explicit type_identity(const int type_id_);

        explicit type_identity(const asset_reference &script_asset_);

        void print(json_writer &writer) const;
    };

    type_identity get_type_identity(const unity_file &file, const int type_id);

    bool operator==(const type_identity &lhs, const type_identity &rhs);

    bool operator!=(const type_identity &lhs, const type_identity &rhs);
}
