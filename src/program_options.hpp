#pragma once

#include "unity_file_printer_options.hpp"

#include <vector>
#include <string>

namespace zizany {
    class program_options {
    public:
        enum class command {
            show_help,
            dump,
            diff,
            extract_previews
        };
        command program_command;
        unity_file_printer_options printer_options;
        std::vector<std::string> filenames;
        std::string output_dir;

        program_options();

        static
        program_options parse(int argc, char **argv);

        static
        void parse_dump_options(program_options &options, int argc, char **argv);

        static
        void parse_diff_options(program_options &options, int argc, char **argv);

        static
        void parse_extract_previews_options(program_options &options, int argc, char **argv);
    };
}
