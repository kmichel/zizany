#include "file_lock.hpp"

namespace zizany {
    file_lock::file_lock(FILE *file_)
            : file(file_) {
        flockfile(file);
    }

    file_lock::~file_lock() {
        funlockfile(file);
    }
}
