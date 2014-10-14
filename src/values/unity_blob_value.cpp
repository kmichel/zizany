#include "unity_blob_value.hpp"

#include "../json_writer.hpp"

#include <cstring>

namespace zizany {
    unity_blob_value::unity_blob_value()
            : unity_value(), data() {
    }

    void
    unity_blob_value::print(json_writer &writer) const {
        writer.start_array();
        char line_buffer[40];
        int line_index(0);
        for (std::size_t buffer_index = 0; buffer_index < data.size(); ++buffer_index) {
            std::snprintf(line_buffer + line_index, 3, "%02x", data[buffer_index]);
            line_index += 2;
            if (buffer_index % 2 == 1)
                line_buffer[line_index++] = ' ';
            if (line_index == sizeof(line_buffer)) {
                writer.add_string(line_buffer, sizeof(line_buffer) - 1);
                line_index = 0;
            }
        }
        writer.end_array();
    }

    bool
    unity_blob_value::equals(const unity_value &value) const {
        return value.equals(data);
    }

    bool
    unity_blob_value::equals(const std::vector<char> &value) const {
        if (data.size() != value.size())
            return false;
        for (std::size_t index = 0; index < data.size(); ++index)
            if (data[index] != value[index])
                return false;
        return true;
    }
}
