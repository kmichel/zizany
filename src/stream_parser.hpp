#pragma once

#include "endianness.hpp"

#include <algorithm>
#include <istream>
#include <vector>

#include "range_checker.hpp"

namespace zizany {
    class stream_parser {
        std::istream &stream;
        const bool must_swap_bytes;
        std::int64_t last_seek;
        range_checker &checker;
    public:
        stream_parser(std::istream &stream_, const endianness endianness, range_checker &checker_);

        stream_parser(const stream_parser &) = delete;

        stream_parser &operator=(const stream_parser &) = delete;

        ~stream_parser();

        template<typename value_type>
        value_type
        parse() {
            value_type value;
            char *const buffer(reinterpret_cast<char *>(&value));
            stream.read(buffer, sizeof(value_type));
            if (sizeof(value_type) > 1 && must_swap_bytes)
                std::reverse(buffer, buffer + sizeof(value_type));
            return value;
        }

        template<typename value_type>
        void
        parse(typename std::vector<value_type> &values, const std::size_t count) {
            if (count > 0) {
                values.resize(count);
                if (sizeof(value_type) > 1 && must_swap_bytes) {
                    for (std::size_t index = 0; index < count; ++index) {
                        char *const buffer(reinterpret_cast<char *>(values.data() + index));
                        stream.read(buffer, sizeof(value_type));
                        std::reverse(buffer, buffer + sizeof(value_type));
                    }
                } else {
                    char *const buffer(reinterpret_cast<char *>(values.data()));
                    stream.read(buffer, static_cast<long>(count) * static_cast<long>(sizeof(value_type)));
                }
            }
        }

        std::string parse_string();

        void align(const int stride);

        std::int64_t tell() const;

        void seek(std::int64_t position);

        void seek_from_end(std::int64_t position);
    };

    template<>
    inline void
    stream_parser::parse<bool>(typename std::vector<bool> &values, const std::vector<bool>::size_type count) {
        // This is slower than the default implementation but required
        // since std::vector<bool> is so special...
        if (count > 0) {
            values.resize(count);
            for (std::vector<bool>::size_type index = 0; index < count; ++index)
                values.at(index) = parse<bool>();
        }
    }
}
