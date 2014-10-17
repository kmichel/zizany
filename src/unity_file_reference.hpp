#pragma once

#include "guid.hpp"
#include "unity_file_reference_properties.hpp"

namespace zizany {
    class unity_file_reference {
    public:
        guid file_guid;
        unity_file_reference_properties properties;

        unity_file_reference();
    };
}
