#include "json_writer.hpp"

#include "../libs/gdtoa/gdtoa.h"

#include <limits>
#include <cinttypes>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <sys/errno.h>

namespace zizany {
    static
    void
    putc_unlocked_checked(int c, FILE *output) {
        const int putc_status(putc_unlocked(c, output));
        if (putc_status == EOF)
            throw std::runtime_error(strerror(errno));
    }

    static
    void
    fputs_unlocked_(const char *str, FILE *output) {
        while (*str != 0)
            putc_unlocked_checked(*str++, output);
    }

    static
    void
    print_with_leading_zero(const char *buffer, FILE *output) {
        if (buffer[0] == '-' && buffer[1] == '.') {
            putc_unlocked_checked('-', output);
            putc_unlocked_checked('0', output);
            fputs_unlocked_(buffer + 1, output);
        } else {
            if (buffer[0] == '.')
                putc_unlocked_checked('0', output);
            fputs_unlocked_(buffer, output);
        }
    }

    json_writer::json_writer(FILE *output_, const int indent_width_)
            : output(output_),
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
    json_writer::add_string(const char *chars, const std::size_t length) {
        insert_separator_if_needed();
        print_quoted_string(chars, length);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(std::int32_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, sizeof(buffer), "%" PRIi32, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        fputs_unlocked_(buffer, output);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(std::uint32_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, sizeof(buffer), "%" PRIu32, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        fputs_unlocked_(buffer, output);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(std::int64_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, 32, "%" PRIi64, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        fputs_unlocked_(buffer, output);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(std::uint64_t value) {
        insert_separator_if_needed();
        char buffer[32];
        const int snprintf_status(snprintf(buffer, 32, "%" PRIu64, value));
        if (snprintf_status >= static_cast<int>(sizeof(buffer)))
            throw std::runtime_error("unexpected large number");
        fputs_unlocked_(buffer, output);
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(float value) {
        insert_separator_if_needed();
        if (std::isnan(value) || std::isinf(value))
            fputs_unlocked_("null", output);
        else {
            char buffer[32];
            const char *g_ffmt_status(g_ffmt(buffer, &value, -1, sizeof(buffer)));
            if (g_ffmt_status == nullptr)
                throw std::runtime_error("unexpected large number");
            print_with_leading_zero(buffer, output);
        }
        state = writer_state::after_value;
    }

    void
    json_writer::add_number(double value) {
        insert_separator_if_needed();
        if (std::isnan(value) || std::isinf(value))
            fputs_unlocked_("null", output);
        else {
            char buffer[32];
            const char *g_dfmt_status(g_dfmt(buffer, &value, -1, sizeof(buffer)));
            if (g_dfmt_status == nullptr)
                throw std::runtime_error("unexpected large number");
            print_with_leading_zero(buffer, output);
        }
        state = writer_state::after_value;
    }

    void
    json_writer::add_bool(bool value) {
        insert_separator_if_needed();
        fputs_unlocked_(value ? "true" : "false", output);
        state = writer_state::after_value;
    }

    void
    json_writer::add_null() {
        insert_separator_if_needed();
        fputs_unlocked_("null", output);
        state = writer_state::after_value;
    }

    void
    json_writer::start_object(bool force_inline) {
        start_composite('{', force_inline);
    }

    void
    json_writer::add_key(const char *key) {
        insert_separator_if_needed();
        print_quoted_string(key);
        fputs_unlocked_(": ", output);
        state = writer_state::after_key;
    }

    void
    json_writer::add_key(const std::string &key) {
        insert_separator_if_needed();
        print_quoted_string(key);
        fputs_unlocked_(": ", output);
        state = writer_state::after_key;
    }

    void
    json_writer::end_object() {
        end_composite('}');
    }

    void
    json_writer::start_array(bool force_inline) {
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
    json_writer::start_composite(char marker, bool force_inline) {
        insert_separator_if_needed();
        putc_unlocked_checked(marker, output);
        if (force_inline)
            inline_level = std::min(inline_level, indent_level);
        indent_level += 1;
        state = writer_state::after_composite_start;
    }

    void
    json_writer::end_composite(char marker) {
        indent_level -= 1;
        if (state == writer_state::after_value && indent_level < inline_level)
            insert_newline();
        putc_unlocked_checked(marker, output);
        state = writer_state::after_value;
        if (indent_level == inline_level)
            inline_level = std::numeric_limits<int>::max();
    }

    void
    json_writer::insert_separator_if_needed() {
        if (state == writer_state::after_value)
            putc_unlocked_checked(',', output);
        if (state != writer_state::after_key) {
            if (state != writer_state::after_start && indent_level < inline_level)
                insert_newline();
            else if (state == writer_state::after_value)
                putc_unlocked_checked(' ', output);
        }
    }

    void
    json_writer::insert_newline() {
        putc_unlocked_checked('\n', output);
        for (int i = 0; i < indent_width * indent_level; ++i)
            putc_unlocked_checked(' ', output);
    }

    inline
    void print_quoted_character(FILE *output, const char character) {
        const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        switch (character) {
            case '\b':
                putc_unlocked_checked('\\', output);
                putc_unlocked_checked('b', output);
                break;
            case '\f':
                putc_unlocked_checked('\\', output);
                putc_unlocked_checked('f', output);
                break;
            case '\n':
                putc_unlocked_checked('\\', output);
                putc_unlocked_checked('n', output);
                break;
            case '\r':
                putc_unlocked_checked('\\', output);
                putc_unlocked_checked('r', output);
                break;
            case '\t':
                putc_unlocked_checked('\\', output);
                putc_unlocked_checked('t', output);
                break;
            case '\\':
                putc_unlocked_checked('\\', output);
                putc_unlocked_checked('\\', output);
                break;
            case '\"':
                putc_unlocked_checked('\\', output);
                putc_unlocked_checked('"', output);
                break;
            default:
                if (character > 0 && character < 32) {
                    putc_unlocked_checked('\\', output);
                    putc_unlocked_checked('u', output);
                    putc_unlocked_checked('0', output);
                    putc_unlocked_checked('0', output);
                    putc_unlocked_checked(hex[character >> 4], output);
                    putc_unlocked_checked(hex[character & 0xf], output);
                } else {
                    putc_unlocked_checked(character, output);
                }
        }
    }

    void
    json_writer::print_quoted_string(const std::string &string) {
        putc_unlocked_checked('"', output);
        for (std::size_t index = 0; index < string.size(); ++index)
            print_quoted_character(output, string[index]);
        putc_unlocked_checked('"', output);
    }

    void
    json_writer::print_quoted_string(const char *string) {
        putc_unlocked_checked('"', output);
        while (*string != 0)
            print_quoted_character(output, *string++);
        putc_unlocked_checked('"', output);
    }

    void
    json_writer::print_quoted_string(const char *string, const std::size_t length) {
        putc_unlocked_checked('"', output);
        for (std::size_t index = 0; index < length; ++index)
            print_quoted_character(output, string[index]);
        putc_unlocked_checked('"', output);
    }
}
