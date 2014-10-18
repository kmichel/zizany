#pragma once

#include <cstdint>
#include <string>

namespace zizany {
    class file_stream;

    class json_writer {
        enum class writer_state {
            after_start,
            after_composite_start,
            after_value,
            after_key
        };
        file_stream &stream;
        const int indent_width;

        int indent_level;
        int inline_level;
        writer_state state;

        void insert_separator_if_needed();

        void insert_newline();

        void start_composite(const char marker, const bool force_inline);

        void end_composite(const char marker);

        void print_quoted_string(const std::string &string);

        void print_quoted_string(const char *const string);

        void print_quoted_string(const char *const string, const std::size_t length);

    public:
        json_writer(file_stream &stream_, const int indent_width_ = 3);

        json_writer(const json_writer &) = delete;

        json_writer &operator=(const json_writer &) = delete;

        void add_string(const std::string &value);

        void add_string(const char *const chars, const std::size_t length);

        void add_number(const std::int32_t value);

        void add_number(const std::uint32_t value);

        void add_number(const std::int64_t value);

        void add_number(const std::uint64_t value);

        void add_number(const float value);

        void add_number(const double value);

        void add_bool(const bool value);

        void add_null();

        void start_object(const bool force_inline = false);

        void add_key(const char *const key);

        void add_key(const std::string &key);

        void end_object();

        void start_array(const bool force_inline = false);

        void end_array();

        void reset();
    };
}
