#pragma once

#include "unity_file_printer_options.hpp"

#include <vector>
#include <string>

namespace zizany {
    struct program_options {
        enum class command {
            show_help,
            dump,
            extract_previews
        };
        command command;
        unity_file_printer_options printer_options;
        std::vector<std::string> filenames;
        std::string output_dir;

        static
        program_options parse(int argc, char **argv);

        static
        void parse_dump_options(program_options &options, int argc, char **argv);

        static
        void parse_extract_previews_options(program_options &options, int argc, char **argv);
    };
}
