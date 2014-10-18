#include "file_lock.hpp"

namespace zizany {
    file_lock::file_lock(FILE *file_)
            : file(file_) {
        if (file != nullptr)
            flockfile(file);
    }

    file_lock::file_lock(file_lock &&that)
            : file(that.file) {
        that.file = nullptr;
    }

    file_lock::~file_lock() {
        if (file != nullptr)
            funlockfile(file);
    }
}
