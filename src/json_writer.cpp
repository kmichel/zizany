#include "json_writer.hpp"

#include <limits>
#include <cmath>

// TODO: test putc_unlocked (EOF) and snprintf (length if unlimited) output
namespace zizany {
    static
    void
    fputs_unlocked(const char *str, FILE *output) {
        while (*str != 0)
            putc_unlocked(*str++, output);
    }

    json_writer::json_writer(FILE *output_, const int indent_width_)
            : output(output_),
              indent_width(indent_width_),
              indent_level(0),
              inline_level(std::numeric_limits<int>::max()),
              state(state::after_start) {
    }

    void
    json_writer::add_string(const std::string &value) {
        insert_separator_if_needed();
        print_quoted_string(value);
        state = state::after_value;
    }

    void
    json_writer::add_string(const char *chars, const std::size_t length) {
        insert_separator_if_needed();
        print_quoted_string(chars, length);
        state = state::after_value;
    }

    void
    json_writer::add_number(std::int32_t value) {
        insert_separator_if_needed();
        char buffer[64];
        snprintf(buffer, 64, "%i", value);
        fputs_unlocked(buffer, output);
        state = state::after_value;
    }

    void
    json_writer::add_number(std::uint32_t value) {
        insert_separator_if_needed();
        char buffer[64];
        snprintf(buffer, 64, "%u", value);
        fputs_unlocked(buffer, output);
        state = state::after_value;
    }

    void
    json_writer::add_number(std::int64_t value) {
        insert_separator_if_needed();
        char buffer[64];
        snprintf(buffer, 64, "%lli", value);
        fputs_unlocked(buffer, output);
        state = state::after_value;
    }

    void
    json_writer::add_number(std::uint64_t value) {
        insert_separator_if_needed();
        char buffer[64];
        snprintf(buffer, 64, "%llu", value);
        fputs_unlocked(buffer, output);
        state = state::after_value;
    }

    void
    json_writer::add_number(float value) {
        insert_separator_if_needed();
        if (std::isnan(value) || std::isinf(value))
            fputs_unlocked("null", output);
        else {
            char buffer[64];
            snprintf(buffer, 64, "%f", value);
            fputs_unlocked(buffer, output);
        }
        state = state::after_value;
    }

    void
    json_writer::add_number(double value) {
        insert_separator_if_needed();
        if (std::isnan(value) || std::isinf(value))
            fputs_unlocked("null", output);
        else {
            char buffer[64];
            snprintf(buffer, 64, "%lf", value);
            fputs_unlocked(buffer, output);
        }
        state = state::after_value;
    }

    void
    json_writer::add_bool(bool value) {
        insert_separator_if_needed();
        fputs_unlocked(value ? "true" : "false", output);
        state = state::after_value;
    }

    void
    json_writer::add_null() {
        insert_separator_if_needed();
        fputs_unlocked("null", output);
        state = state::after_value;
    }

    void
    json_writer::start_object(bool force_inline) {
        start_composite('{', force_inline);
    }

    void
    json_writer::add_key(const char *key) {
        insert_separator_if_needed();
        print_quoted_string(key);
        fputs_unlocked(": ", output);
        state = state::after_key;
    }

    void
    json_writer::add_key(const std::string &key) {
        insert_separator_if_needed();
        print_quoted_string(key);
        fputs_unlocked(": ", output);
        state = state::after_key;
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
        state = state::after_start;
    }

    void
    json_writer::start_composite(char marker, bool force_inline) {
        insert_separator_if_needed();
        putc_unlocked(marker, output);
        if (force_inline)
            inline_level = std::min(inline_level, indent_level);
        indent_level += 1;
        state = state::after_composite_start;
    }

    void
    json_writer::end_composite(char marker) {
        indent_level -= 1;
        if (state == state::after_value && indent_level < inline_level)
            insert_newline();
        putc_unlocked(marker, output);
        state = state::after_value;
        if (indent_level == inline_level)
            inline_level = std::numeric_limits<int>::max();
    }

    void
    json_writer::insert_separator_if_needed() {
        if (state == state::after_value)
            putc_unlocked(',', output);
        if (state != state::after_key) {
            if (state != state::after_start && indent_level < inline_level)
                insert_newline();
            else if (state == state::after_value)
                putc_unlocked(' ', output);
        }
    }

    void
    json_writer::insert_newline() {
        putc_unlocked('\n', output);
        for (int i = 0; i < indent_width * indent_level; ++i)
            putc_unlocked(' ', output);
    }

    inline
    void print_quoted_character(FILE *output, const char character) {
        const char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
        switch (character) {
            case '\b':
                putc_unlocked('\\', output);
                putc_unlocked('b', output);
                break;
            case '\f':
                putc_unlocked('\\', output);
                putc_unlocked('f', output);
                break;
            case '\n':
                putc_unlocked('\\', output);
                putc_unlocked('n', output);
                break;
            case '\r':
                putc_unlocked('\\', output);
                putc_unlocked('r', output);
                break;
            case '\t':
                putc_unlocked('\\', output);
                putc_unlocked('t', output);
                break;
            case '\\':
                putc_unlocked('\\', output);
                putc_unlocked('\\', output);
                break;
            case '\"':
                putc_unlocked('\\', output);
                putc_unlocked('"', output);
                break;
            default:
                if (character > 0 && character < 32) {
                    putc_unlocked('\\', output);
                    putc_unlocked('u', output);
                    putc_unlocked('0', output);
                    putc_unlocked('0', output);
                    putc_unlocked(hex[character >> 4], output);
                    putc_unlocked(hex[character & 0xf], output);
                } else {
                    putc_unlocked(character, output);
                }
        }
    }

    void
    json_writer::print_quoted_string(const std::string &string) {
        putc_unlocked('"', output);
        for (std::size_t index = 0; index < string.size(); ++index)
            print_quoted_character(output, string[index]);
        putc_unlocked('"', output);
    }

    void
    json_writer::print_quoted_string(const char *string) {
        putc_unlocked('"', output);
        while (*string != 0)
            print_quoted_character(output, *string++);
        putc_unlocked('"', output);
    }

    void
    json_writer::print_quoted_string(const char *string, const std::size_t length) {
        putc_unlocked('"', output);
        for (std::size_t index = 0; index < length; ++index)
            print_quoted_character(output, string[index]);
        putc_unlocked('"', output);
    }
}
