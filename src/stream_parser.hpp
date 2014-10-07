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
                const std::size_t value_size(sizeof(value_type));
                values.resize(count);
                char *const buffer(reinterpret_cast<char *>(values.data()));
                stream.read(buffer, static_cast<long>(count) * static_cast<long>(value_size));
                if (value_size > 1 && must_swap_bytes)
                    for (std::size_t index = 0; index < count; ++index)
                        std::reverse(buffer + index * value_size, buffer + index * value_size + value_size);
            }
        }

        void parse(std::vector<bool> &values, const std::size_t count);

        std::string parse_string();

        void align(const int stride);

        std::int64_t tell() const;

        void seek(std::int64_t position);

        void seek_from_end(std::int64_t position);
    };
}
