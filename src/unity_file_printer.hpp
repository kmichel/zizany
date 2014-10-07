#pragma once

#include "unity_file_printer_options.hpp"

namespace zizany {
    class json_writer;

    class unity_file;

    class unity_type;

    void print_file(json_writer &writer, const unity_file &file, unity_file_printer_options options);
}
