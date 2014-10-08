#include "unity_multiline_string_value.hpp"

#include "../json_writer.hpp"

#include <algorithm>

namespace zizany {
    unity_multiline_string_value::unity_multiline_string_value()
            : unity_string_value() {
    }

    void
    unity_multiline_string_value::print(json_writer &writer) const {
        writer.start_array();
        std::vector<char>::const_iterator iter(chars.begin());
        const std::vector<char>::const_iterator end(chars.end());
        while (iter != end) {
            const std::vector<char>::const_iterator next_newline(std::find(iter, end, '\n'));
            const std::vector<char>::const_iterator after_line_end(next_newline == end ? next_newline : next_newline + 1);
            writer.add_string(&*iter, static_cast<std::size_t>(after_line_end - iter));
            iter = after_line_end;
        }
        writer.end_array();
    }
}
