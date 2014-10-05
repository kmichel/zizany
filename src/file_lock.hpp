#pragma once

#include <cstdio>

namespace zizany {
    class file_lock {
        FILE *file;
    public:
        file_lock(FILE *file_);

        file_lock(const file_lock &) = delete;

        file_lock &operator=(const file_lock &) = delete;

        ~file_lock();
    };
}
