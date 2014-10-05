#include "file_lock.hpp"
#include "json_writer.hpp"
#include "parser_exception.hpp"
#include "program_options.hpp"
#include "unity_asset.hpp"
#include "unity_file.hpp"
#include "unity_file_printer.hpp"
#include "unity_file_reference.hpp"
#include "unity_file_parser.hpp"
#include "unity_preview.hpp"
#include "unity_type.hpp"
#include "values/unity_value.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <cstring>

static
std::string
get_filename_without_extension(const std::string &filepath) {
    const std::size_t slash_position(filepath.rfind("/"));
    const std::size_t dot_position(filepath.rfind("/", slash_position + 1));
    if (slash_position == std::string::npos) {
        if (dot_position == std::string::npos)
            return filepath;
        else
            return filepath.substr(0, dot_position);
    } else if (dot_position == std::string::npos || dot_position == 0) {
        return filepath.substr(slash_position + 1);
    } else {
        return filepath.substr(slash_position + 1, dot_position - (slash_position + 1));
    }
}

static
void
create_path(const std::string path) {
    if (path.size()) {
        std::size_t search_start_position(0);
        std::size_t slash_position;
        do {
            slash_position = path.find('/', search_start_position);
            const std::string partial_path(path.substr(0, slash_position));
            const int mkdir_status(mkdir(partial_path.c_str(), 0777));
            if (mkdir_status != 0 && errno != EEXIST) {
                std::stringstream buffer;
                buffer << "error while creating directory '" << partial_path << "': " << strerror(errno);
                throw std::runtime_error(buffer.str());
            }
            search_start_position = slash_position + 1;
        } while (slash_position != std::string::npos);
    }
}

static
void
parse_file(const std::string &filename, zizany::unity_file &unity_file) {
    zizany::unity_file_parser unity_file_parser(unity_file);
    std::ifstream file_stream(filename, std::ios_base::in | std::ios_base::binary);
    if (file_stream) {
        unity_file_parser.parse(
                file_stream,
                [&filename](std::int64_t a_start, std::int64_t a_end, std::int64_t b_start, std::int64_t b_end) {
                    std::cerr
                            << "read ranges (" << a_start << ',' << a_end << ") and ("
                            << b_start << ',' << b_end << ") overlap in file " << filename;
                });
        file_stream.close();
    } else {
        std::stringstream buffer;
        buffer << "failed opening file: '" << filename << '\'';
        throw std::runtime_error(buffer.str());
    }
}

static
void
dump_file(const std::string &filename, zizany::unity_file_printer_options options) {
    zizany::unity_file unity_file;
    parse_file(filename, unity_file);
    const zizany::file_lock file_lock(stdout);
    zizany::json_writer writer(stdout);
    zizany::print_file(writer, unity_file, options);
    fputc('\n', stdout);
    fflush(stdout);
}

static
void
extract_previews(const std::string &output_dir, const std::string &filename) {
    zizany::unity_file unity_file;
    parse_file(filename, unity_file);
    for (std::size_t index = 0; index < unity_file.previews.size(); ++index) {
        const zizany::unity_preview &preview(unity_file.previews.at(index));
        std::stringstream previews_filename;
        previews_filename
                << output_dir;
        if (output_dir.rfind('/') != output_dir.size() - 1)
            previews_filename << '/';
        previews_filename << get_filename_without_extension(filename)
                << "_" << unity_file.previews.get_id_at(index)
                << "_" << preview.object_id << ".png";
        std::fstream preview_stream(previews_filename.str(), std::ios_base::out | std::ios_base::binary);
        const std::vector<unsigned char> &data(unity_file.previews.at(index).data);
        preview_stream.write(reinterpret_cast<const char *>(data.data()), static_cast<long>(data.size()));
    }
}

static
void
show_help() {
    std::cout
            << "Usage: zizany <command> [<args>]\n"
            << "\n"
            << "Parse and extract information from Unity binary assets files.\n"
            << "\n"
            << "Commands:\n"
            << "    zizany help\n"
            << "        Show this help message and exit.\n"
            << "    zizany dump [-tarp] [--types] [--assets] [--file-references] [--previews] <filename>\n"
            << "        Dump information about the specified file as a json object.\n"
            << "        Options:\n"
            << "            -t, --types\n"
            << "                Dump structure of the stored types.\n"
            << "            -a, --assets\n"
            << "                Dump value of the stored assets.\n"
            << "            -r, --file-references\n"
            << "                Dump value of the file references.\n"
            << "            -p, --previews\n"
            << "                Dump value of the stored previews.\n"
            << "    zizany extract_previews [-o <path>] [--output-dir=<path>] [<filename> ...]\n"
            << "        Extract previews as png files.\n"
            << "        Options:\n"
            << "            -o <path>, --output-dir=<path>\n"
            << "                Select a directory where preview files will be saved.\n"
            << "                The directory will be created if it does not already exist.\n"
            << "                Defaults to the current directory.\n";
}

int
main(const int argc, char **argv) {
    try {
        zizany::program_options options(zizany::program_options::parse(argc, argv));
        switch (options.program_command) {
            case zizany::program_options::command::show_help:
                show_help();
                break;
            case zizany::program_options::command::dump:
                dump_file(options.filenames.at(0), options.printer_options);
                break;
            case zizany::program_options::command::extract_previews:
                create_path(options.output_dir);
                for (std::size_t filename_index = 0; filename_index < options.filenames.size(); ++filename_index)
                    extract_previews(options.output_dir, options.filenames.at(filename_index));
                break;
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return -1;
    }
}
