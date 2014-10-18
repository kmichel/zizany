#pragma once

#include "file_lock.hpp"

#include <cstdio>

namespace zizany {
    class file_stream {
        FILE *const stream;
    public:
        explicit file_stream(FILE *const stream_);

        file_stream(const char *const filename, const char *const mode);

        file_stream(const file_stream &) = delete;

        file_stream &operator=(const file_stream &) = delete;

        file_lock get_lock();

        int getc();

        void putc(const char c);

        void puts(const char *const str);

        void read(void *const buffer, const std::size_t item_size, const std::size_t items_count);

        void write(const void *const buffer, const std::size_t item_size, const std::size_t items_count);

        void seek(const long position, const int whence);

        long tell();

        void flush();

        void close();
    };
}
