#pragma once

namespace zizany {
    class delta_store;

    class unity_array_value;

    void compare_sequences(const unity_array_value &lhs, const unity_array_value &rhs, delta_store &store);
}
