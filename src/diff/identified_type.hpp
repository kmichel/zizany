#pragma once

#include "type_identity.hpp"

#include <vector>

namespace zizany {
    class unity_file;

    class unity_type;

    class identified_type {
    public:
        type_identity identity;
        const unity_type &type;
    };

    std::vector<identified_type> identify_types(const unity_file &file);
}
