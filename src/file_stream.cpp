#include "file_stream.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>

namespace zizany {
    file_stream::file_stream(FILE *const stream_)
            : stream(stream_) {
    }

    file_stream::file_stream(const char *const filename, const char *const mode)
            : stream(std::fopen(filename, mode)) {
        if (stream == nullptr)
            throw std::runtime_error(std::string("error while opening '") + filename + "': " + std::strerror(errno));
    }

    file_lock
    file_stream::get_lock() {
        return file_lock(stream);
    }

    int
    file_stream::getc() {
        errno = 0;
        const int c(getc_unlocked(stream));
        if (c == EOF) {
            if (feof_unlocked(stream))
                throw std::runtime_error("unexpected end of stream while reading file");
            else if (errno != 0)
                throw std::runtime_error(std::string("file read error: ") + std::strerror(errno));
            else
                throw std::runtime_error("unknown file read error");
        }
        return c;
    }

    void
    file_stream::putc(const char c) {
        errno = 0;
        const int putc_status(putc_unlocked(c, stream));
        if (putc_status == EOF) {
            if (errno != 0)
                throw std::runtime_error(std::string("file write error: ") + std::strerror(errno));
            else
                throw std::runtime_error("unknown file write error");
        }
    }

    void file_stream::puts(const char *const str) {
        errno = 0;
        for (const char *iter(str); *iter != 0; ++iter) {
            const int putc_status(putc_unlocked(*iter, stream));
            if (putc_status == EOF) {
                if (errno != 0)
                    throw std::runtime_error(std::string("file write error: ") + std::strerror(errno));
                else
                    throw std::runtime_error("unknown file write error");
            }
        }
    }

    void
    file_stream::read(void *const buffer, const std::size_t item_size, const std::size_t items_count) {
        errno = 0;
        const std::size_t fread_status(std::fread(buffer, item_size, items_count, stream));
        if (fread_status != items_count) {
            if (feof_unlocked(stream))
                throw std::runtime_error("unexpected end of stream while reading file");
            else if (errno != 0)
                throw std::runtime_error(std::string("file read error: ") + std::strerror(errno));
            else
                throw std::runtime_error("unknown file read error");
        }
    }

    void
    file_stream::write(const void *const buffer, const std::size_t item_size, const std::size_t items_count) {
        errno = 0;
        const std::size_t fwrite_status(std::fwrite(buffer, item_size, items_count, stream));
        if (fwrite_status != items_count) {
            if (errno != 0)
                throw std::runtime_error(std::string("file write error :") + std::strerror(errno));
            else
                throw std::runtime_error("unknown file write error");
        }
    }

    void
    file_stream::seek(const long position, const int whence) {
        errno = 0;
        const int fseek_status(std::fseek(stream, position, whence));
        if (fseek_status == -1) {
            if (errno != 0)
                throw std::runtime_error(std::string("file seek error: ") + std::strerror(errno));
            else
                throw std::runtime_error(std::string("unknown file seek error"));
        }
    }

    long
    file_stream::tell() {
        errno = 0;
        const long ftell_status(std::ftell(stream));
        if (ftell_status == -1) {
            if (errno != 0)
                throw std::runtime_error(std::string("file tell error: ") + std::strerror(errno));
            else
                throw std::runtime_error("unknown file tell error");
        }
        return ftell_status;
    }

    void
    file_stream::flush() {
        const int fflush_status(std::fflush(stream));
        if (fflush_status == EOF)
            throw std::runtime_error(std::string("file flush error: ") + std::strerror(errno));
    }

    void
    file_stream::close() {
        const int fclose_status(std::fclose(stream));
        if (fclose_status == EOF)
            throw std::runtime_error(std::string("file close error: ") + std::strerror(errno));
    }
}
