#pragma once

#include "../guid.hpp"

#include <vector>

namespace zizany {
    class json_writer;

    class unity_file;

    class unity_type;

    struct type_identity {
        bool is_script;
        int type_id;
        // TODO: use asset_reference
        struct {
            guid file_guid;
            int asset_id;
        } script;

        type_identity(const int type_id_);

        type_identity(const guid &file_guid_, const int asset_id_);

        const unity_type *resolve(const unity_file &file) const;

        void print(json_writer &writer) const;
    };

    type_identity get_type_identity(const unity_file &file, const int type_id);

    bool operator==(const type_identity &lhs, const type_identity &rhs);
}
