#pragma once

namespace zizany {
    class delta_store;

    class unity_type;

    void compare_types(
            const unity_type &base,
            const unity_type &other,
            delta_store &store);
}
