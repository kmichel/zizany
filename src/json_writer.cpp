#include "json_writer.hpp"

#include "file_stream.hpp"
#include "../libs/gdtoa/gdtoa.h"

#include <limits>
#include <cinttypes>
#include <cmath>
#include <cstring>
#include <stdexcept>

namespace zizany {
    static
    void
    print_with_leading_zero(const char *const buffer, file_stream &stream) {
        if (buffer[0] == '-' && buffer[1] == '.') {
            stream.putc('-');
            stream.putc('0');
            stream.puts(buffer + 1);
        } else {
            if (buffer[0] == '.')
                stream.putc('0');
            stream.puts(buffer);
        }
    }

    json_writer::json_writer(file_stream &stream_, const int indent_width_)
            : stream(stream_),
              indent_width(indent_width_),
              indent_level(0),
              inline_level(std::numeric_limits<int>::max()),
              state(writer_state::after_start) {
    }

    void
    json_writer::add_string(const std::string &value) {
        insert_separator_if_needed();
        print_quoted_string(value);
        state = writer_state::after_value;
    }

    void
    json_writer::add_string(const char *const chars, const std::size_t length) {
        insert_separator_if_needed();
        print_quoted_string(chars, length);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(const std::int32_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, sizeof(buffer), "%" PRIi32, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        stream.puts(buffer);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(const std::uint32_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, sizeof(buffer), "%" PRIu32, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        stream.puts(buffer);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(const std::int64_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, 32, "%" PRIi64, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        stream.puts(buffer);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(const std::uint64_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, 32, "%" PRIu64, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        stream.puts(buffer);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(const float value) {
        insert_separator_if_needed();
        if (std::isnan(value) || std::isinf(value))
            stream.puts("null");
        else {
            char buffer[32];
            const char *g_ffmt_status(g_ffmt(buffer, &value, -1, sizeof(buffer)));
            if (g_ffmt_status == nullptr)
                throw std::runtime_error("unexpected large number");
            print_with_leading_zero(buffer, stream);
        }
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(const double value) {
        insert_separator_if_needed();
        if (std::isnan(value) || std::isinf(value))
            stream.puts("null");
        else {
            char buffer[32];
            const char *g_dfmt_status(g_dfmt(buffer, &value, -1, sizeof(buffer)));
            if (g_dfmt_status == nullptr)
                throw std::runtime_error("unexpected large number");
            print_with_leading_zero(buffer, stream);
        }
        state = writer_state::after_value;
    }

    void
    json_writer::add_bool(const bool value) {
        insert_separator_if_needed();
        stream.puts(value ? "true" : "false");
        state = writer_state::after_value;
    }

    void
    json_writer::add_null() {
        insert_separator_if_needed();
        stream.puts("null");
        state = writer_state::after_value;
    }

    void
    json_writer::start_object(const bool force_inline) {
        start_composite('{', force_inline);
    }

    void
    json_writer::add_key(const char *const key) {
        insert_separator_if_needed();
        print_quoted_string(key);
        stream.puts(": ");
        state = writer_state::after_key;
    }

    void
    json_writer::add_key(const std::string &key) {
        insert_separator_if_needed();
        print_quoted_string(key);
        stream.puts(": ");
        state = writer_state::after_key;
    }

    void
    json_writer::end_object() {
        end_composite('}');
    }

    void
    json_writer::start_array(const bool force_inline) {
        start_composite('[', force_inline);
    }

    void
    json_writer::end_array() {
        end_composite(']');
    }

    void
    json_writer::reset() {
        indent_level = 0;
        state = writer_state::after_start;
    }

    void
    json_writer::start_composite(const char marker, const bool force_inline) {
        insert_separator_if_needed();
        stream.putc(marker);
        if (force_inline)
            inline_level = std::min(inline_level, indent_level);
        indent_level += 1;
        state = writer_state::after_composite_start;
    }

    void
    json_writer::end_composite(const char marker) {
        indent_level -= 1;
        if (state == writer_state::after_value && indent_level < inline_level)
            insert_newline();
        stream.putc(marker);
        state = writer_state::after_value;
        if (indent_level == inline_level)
            inline_level = std::numeric_limits<int>::max();
    }

    void
    json_writer::insert_separator_if_needed() {
        if (state == writer_state::after_value)
            stream.putc(',');
        if (state != writer_state::after_key) {
            if (state != writer_state::after_start && indent_level < inline_level)
                insert_newline();
            else if (state == writer_state::after_value)
                stream.putc(' ');
        }
    }

    void
    json_writer::insert_newline() {
        stream.putc('\n');
        for (int i = 0; i < indent_width * indent_level; ++i)
            stream.putc(' ');
    }

    inline
    void print_quoted_character(file_stream &stream, const char character) {
        const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        switch (character) {
            case '\b':
                stream.putc('\\');
                stream.putc('b');
                break;
            case '\f':
                stream.putc('\\');
                stream.putc('f');
                break;
            case '\n':
                stream.putc('\\');
                stream.putc('n');
                break;
            case '\r':
                stream.putc('\\');
                stream.putc('r');
                break;
            case '\t':
                stream.putc('\\');
                stream.putc('t');
                break;
            case '\\':
                stream.putc('\\');
                stream.putc('\\');
                break;
            case '\"':
                stream.putc('\\');
                stream.putc('"');
                break;
            default:
                if (character > 0 && character < 32) {
                    stream.putc('\\');
                    stream.putc('u');
                    stream.putc('0');
                    stream.putc('0');
                    stream.putc(hex[character >> 4]);
                    stream.putc(hex[character & 0xf]);
                } else {
                    stream.putc(character);
                }
        }
    }

    void
    json_writer::print_quoted_string(const std::string &string) {
        stream.putc('"');
        for (const char c : string)
            print_quoted_character(stream, c);
        stream.putc('"');
    }

    void
    json_writer::print_quoted_string(const char *const string) {
        stream.putc('"');
        for (const char *iter(string); *iter != 0; ++iter)
            print_quoted_character(stream, *iter);
        stream.putc('"');
    }

    void
    json_writer::print_quoted_string(const char *const string, const std::size_t length) {
        stream.putc('"');
        for (std::size_t index = 0; index < length; ++index)
            print_quoted_character(stream, string[index]);
        stream.putc('"');
    }
}
