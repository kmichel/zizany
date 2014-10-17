#pragma once

#include "unity_type.hpp"

#include <string>

namespace zizany {
    unity_type create_simple_type(const std::string &name, bool requires_padding = false);

    unity_type create_array_type(const std::string &name, const std::string &size_name, const unity_type &size_type, const std::string &element_name, const unity_type &element_type);

    unity_type create_string_type();

    unity_type create_asset_reference_type(const std::string &target_type);

    unity_type create_tail_blob_type();

    unity_type create_release_gameobject_type();

    unity_type create_release_monobehaviour_type();

    unity_type create_release_monoscript_type();
}
