#pragma once

namespace zizany {
    class delta_store;

    class unity_file;

    void compare_files(const unity_file &base, const unity_file &other, delta_store &delta);
}
