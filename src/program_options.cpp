#include "program_options.hpp"

#include <stdexcept>

namespace zizany {
    program_options::program_options()
            : program_command(),
              printer_options(),
              filenames(),
              output_dir() {
    }

    program_options
    program_options::parse(const int argc, const char *const *const argv) {
        program_options options;

        if (argc < 2) {
            options.program_command = program_options::command::show_help;
        } else {
            const std::string command(argv[1]);
            if (command == "help") {
                options.program_command = program_options::command::show_help;
            } else if (command == "dump") {
                options.program_command = program_options::command::dump;
                parse_dump_options(options, argc - 2, argv + 2);
            } else if (command == "extract_previews") {
                options.program_command = program_options::command::extract_previews;
                parse_extract_previews_options(options, argc - 2, argv + 2);
            } else if (command == "diff") {
                options.program_command = program_options::command::diff;
                parse_diff_options(options, argc - 2, argv + 2);
            } else {
                throw std::runtime_error("unknown command: '" + command + "'\nType 'zizany help' for usage.");
            }
        }
        return options;
    }

    void
    program_options::parse_dump_options(program_options &options, const int argc, const char *const *const argv) {
        bool allow_options(true);
        for (int argument_index = 0; argument_index < argc; ++argument_index) {
            const std::string argument(argv[argument_index]);
            if (allow_options && argument.find('-') == 0 && argument.size() > 1) {
                if (argument.at(1) == '-') {
                    const std::string option_name = argument.substr(2, std::string::npos);
                    if (option_name.size() == 0)
                        allow_options = false;
                    else if (option_name == "types")
                        options.printer_options.print_types = true;
                    else if (option_name == "assets")
                        options.printer_options.print_assets = true;
                    else if (option_name == "file-references")
                        options.printer_options.print_file_references = true;
                    else if (option_name == "previews")
                        options.printer_options.print_previews = true;
                    else if (option_name == "type-defaults")
                        options.printer_options.print_defaults = true;
                    else if (option_name == "file-layout")
                        options.printer_options.print_layout = true;
                    else if (option_name == "magic")
                        options.printer_options.print_magic = true;
                    else
                        throw std::runtime_error("invalid argument: '" + argument + "'");
                } else {
                    for (std::size_t char_index = 1; char_index < argument.size(); ++char_index) {
                        switch (argument.at(char_index)) {
                            case 't':
                                options.printer_options.print_types = true;
                                break;
                            case 'a':
                                options.printer_options.print_assets = true;
                                break;
                            case 'r':
                                options.printer_options.print_file_references = true;
                                break;
                            case 'p':
                                options.printer_options.print_previews = true;
                                break;
                            case 'd':
                                options.printer_options.print_defaults = true;
                                break;
                            case 'l':
                                options.printer_options.print_layout = true;
                                break;
                            case 'm':
                                options.printer_options.print_magic = true;
                                break;
                            default: {
                                std::string message;
                                message += "invalid argument: '";
                                if (argument.size() != 2) {
                                    message += argument.at(char_index);
                                    message += "' in '";
                                }
                                message += argument;
                                message += "'";
                                throw std::runtime_error(message);
                            }
                        }
                    }
                }
            } else {
                options.filenames.push_back(argument);
            }
        }
        if (options.filenames.size() != 1)
            throw std::runtime_error("dump command takes exactly one file parameter");
    }

    void
    program_options::parse_diff_options(program_options &options, const int argc, const char *const *const argv) {
        bool allow_options(true);
        for (int argument_index = 0; argument_index < argc; ++argument_index) {
            const std::string argument(argv[argument_index]);
            if (allow_options && argument.find('-') == 0 && argument.size() > 1) {
                if (argument.at(1) == '-') {
                    const std::string option_name = argument.substr(2, std::string::npos);
                    if (option_name.size() == 0)
                        allow_options = false;
                    else
                        throw std::runtime_error("invalid argument: '" + argument + "'");
                } else {
                    std::string message;
                    message += "invalid argument: '";
                    if (argument.size() != 2) {
                        message += argument.at(1);
                        message += "' in '";
                    }
                    message += argument;
                    message += "'";
                    throw std::runtime_error(message);
                }
            } else {
                options.filenames.push_back(argument);
            }
        }
        if (options.filenames.size() != 2)
            throw std::runtime_error("diff command takes exactly two file parameters");
    }

    void
    program_options::parse_extract_previews_options(program_options &options, const int argc, const char *const *const argv) {
        bool allow_options(true);
        bool expecting_output_dir(false);
        for (int argument_index = 0; argument_index < argc; ++argument_index) {
            const std::string argument(argv[argument_index]);
            if (expecting_output_dir) {
                options.output_dir = argument;
                expecting_output_dir = false;
            } else if (allow_options && argument.find('-') == 0 && argument.size() > 1) {
                if (argument.at(1) == '-') {
                    const std::string option_name(argument.substr(2, std::string::npos));
                    if (option_name.size() == 0)
                        allow_options = false;
                    else if (option_name.find("output-dir=") == 0)
                        options.output_dir = option_name.substr(11);
                    else
                        throw std::runtime_error("invalid argument: '" + argument + "'");
                } else {
                    // short opts
                    for (std::size_t char_index = 1; char_index < argument.size(); ++char_index) {
                        switch (argument.at(char_index)) {
                            case 'o':
                                // next argument is the filename and we should stop expecting short options
                                expecting_output_dir = true;
                                if (char_index != argument.size() - 1)
                                    throw std::runtime_error(
                                            "unexpected short option '" + argument.substr(char_index + 1, 1) +
                                                    "' after 'o' in '" + argument + "'");
                                break;
                            default: {
                                std::string message;
                                message += "invalid argument: '";
                                if (argument.size() != 2) {
                                    message += argument.at(char_index);
                                    message += "' in '";
                                }
                                message += argument;
                                message += "'";
                                throw std::runtime_error(message);
                            }
                        }
                    }
                }
            } else {
                options.filenames.push_back(argument);
            }
        }
        if (expecting_output_dir)
            throw std::runtime_error("missing <output_dir> argument after -o short option");
    }
}
