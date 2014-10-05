#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

namespace zizany {
    class json_writer {
        enum class writer_state {
            after_start,
            after_composite_start,
            after_value,
            after_key
        };
        FILE *output;
        const int indent_width;

        int indent_level;
        int inline_level;
        writer_state state;

        void insert_separator_if_needed();

        void insert_newline();

        void start_composite(char marker, bool force_inline);

        void end_composite(char marker);

        void print_quoted_string(const std::string &string);

        void print_quoted_string(const char *string);

        void print_quoted_string(const char *string, const std::size_t length);

    public:
        json_writer(FILE *output_, const int indent_width_ = 3);

        json_writer(const json_writer &) = delete;

        json_writer &operator=(const json_writer &) = delete;

        void add_string(const std::string &value);

        void add_string(const char *chars, const std::size_t length);

        void add_number(std::int32_t value);

        void add_number(std::uint32_t value);

        void add_number(std::int64_t value);

        void add_number(std::uint64_t value);

        void add_number(float value);

        void add_number(double value);

        void add_bool(bool value);

        void add_null();

        void start_object(bool force_inline = false);

        void add_key(const char *key);

        void add_key(const std::string &key);

        void end_object();

        void start_array(bool force_inline = false);

        void end_array();

        void reset();
    };
}
