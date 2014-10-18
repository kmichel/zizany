#include "diff/delta.hpp"
#include "diff/file_comparer.hpp"
#include "diff/simple_delta_store.hpp"
#include "file_stream.hpp"
#include "file_lock.hpp"
#include "json_writer.hpp"
#include "parser_exception.hpp"
#include "pod_vector.hpp"
#include "program_options.hpp"
#include "unity_asset.hpp"
#include "unity_file.hpp"
#include "unity_file_printer.hpp"
#include "unity_file_reference.hpp"
#include "unity_file_parser.hpp"
#include "unity_preview.hpp"
#include "unity_type.hpp"
#include "unity_value.hpp"

#include <cerrno>
#include <cinttypes>
#include <cstring>
#include <stdexcept>
#include <sys/stat.h>

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
            if (mkdir_status != 0 && errno != EEXIST)
                throw std::runtime_error("error while creating directory '" + partial_path + "': " + strerror(errno));
            search_start_position = slash_position + 1;
        } while (slash_position != std::string::npos);
    }
}

static
void
parse_file(const std::string &filename, zizany::unity_file &unity_file) {
    zizany::unity_file_parser unity_file_parser(unity_file);
    zizany::file_stream file_stream(filename.c_str(), "rb");
    {
        const zizany::file_lock file_lock(file_stream.get_lock());
        unity_file_parser.parse(
                file_stream,
                [&filename](std::int64_t a_start, std::int64_t a_end, std::int64_t b_start, std::int64_t b_end) {
                    fprintf(stderr,
                            "read ranges (%" PRIi64 ",%" PRIi64 ") and (%" PRIi64 ",%" PRIi64 ") overlap in file '%s'",
                            a_start, a_end, b_start, b_end, filename.c_str());
                });
    }
    // XXX: file won't be closed in case of exception
    file_stream.close();
}

static
void
dump_file(const std::string &filename, zizany::unity_file_printer_options options) {
    zizany::unity_file unity_file;
    parse_file(filename, unity_file);
    zizany::file_stream file_stream(stdout);
    {
        const zizany::file_lock file_lock(file_stream.get_lock());
        zizany::json_writer writer(file_stream);
        zizany::print_file(writer, unity_file, options);
        file_stream.putc('\n');
        file_stream.flush();
    }
}

static
void diff_files(const std::string &base_filename, const std::string &other_filename) {
    zizany::unity_file base_unity_file;
    parse_file(base_filename, base_unity_file);
    zizany::unity_file other_unity_file;
    parse_file(other_filename, other_unity_file);

    zizany::simple_delta_store store;
    zizany::compare_files(base_unity_file, other_unity_file, store);

    zizany::file_stream file_stream(stdout);
    {
        const zizany::file_lock file_lock(file_stream.get_lock());
        zizany::json_writer writer(file_stream);
        store.print(writer);
        file_stream.putc('\n');
        file_stream.flush();
    }
}

static
void
extract_previews(const std::string &output_dir, const std::string &filename) {
    zizany::unity_file unity_file;
    parse_file(filename, unity_file);
    for (std::size_t index = 0; index < unity_file.previews.size(); ++index) {
        const zizany::unity_preview &preview(unity_file.previews.at(index));
        std::string preview_filename;
        preview_filename += output_dir;
        if (output_dir.rfind('/') != output_dir.size() - 1)
            preview_filename += '/';
        preview_filename += get_filename_without_extension(filename);
        preview_filename += '_';
        char buffer[32];
        snprintf(buffer, sizeof(buffer) - 1, "%i", unity_file.previews.get_id_at(index));
        preview_filename += buffer;
        preview_filename += '_';
        snprintf(buffer, sizeof(buffer) - 1, "%" PRIi32, preview.asset_id);
        preview_filename += buffer;
        preview_filename += ".png";
        zizany::file_stream preview_file_stream(preview_filename.c_str(), "wb");
        const zizany::pod_vector<unsigned char> &data(unity_file.previews.at(index).data);
        preview_file_stream.write(data.data(), data.size(), 1);
        // XXX: file won't be closed in case of exception
        preview_file_stream.close();
    }
}

static
void
show_help() {
    puts(
            "Usage: zizany <command> [<args>]\n"
                    "\n"
                    "Parse and extract information from Unity binary assets files.\n"
                    "\n"
                    "Commands:\n"
                    "    zizany help\n"
                    "        Show this help message and exit.\n"
                    "    zizany dump [-tarpdlm] <filename>\n"
                    "        Dump information about the specified file as a json object.\n"
                    "        Options:\n"
                    "            -t, --types\n"
                    "                Dump structure of the stored types.\n"
                    "            -a, --assets\n"
                    "                Dump value of the stored assets.\n"
                    "            -r, --file-references\n"
                    "                Dump value of the file references.\n"
                    "            -p, --previews\n"
                    "                Dump value of the stored previews.\n"
                    "            -d, --type-defaults\n"
                    "                Dump type attributes even if they are equal to their default value.\n"
                    "            -l, --file-layout\n"
                    "                Dump attributes related to the internal layout of the file.\n"
                    "            -m, --magic\n"
                    "                Dump attributes whose meaning is not yet known.\n"
                    "    zizany diff <base_filename> <other_filename>\n"
                    "        Compare two files and print a list of differences.\n"
                    "    zizany extract_previews [-o <path>] [<filename> ...]\n"
                    "        Extract previews as png files.\n"
                    "        Options:\n"
                    "            -o <path>, --output-dir=<path>\n"
                    "                Select a directory where preview files will be saved.\n"
                    "                The directory will be created if it does not already exist.\n"
                    "                Defaults to the current directory.\n");
}

int
main(const int argc, const char *const *const argv) {
    try {
        const zizany::program_options options(zizany::program_options::parse(argc, argv));
        switch (options.program_command) {
            case zizany::program_options::command::show_help:
                show_help();
                break;
            case zizany::program_options::command::dump:
                dump_file(options.filenames.at(0), options.printer_options);
                break;
            case zizany::program_options::command::diff:
                diff_files(options.filenames.at(0), options.filenames.at(1));
                break;
            case zizany::program_options::command::extract_previews:
                create_path(options.output_dir);
                for (std::size_t filename_index = 0; filename_index < options.filenames.size(); ++filename_index)
                    extract_previews(options.output_dir, options.filenames.at(filename_index));
                break;
        }
        return 0;
    } catch (const std::exception &error) {
        zizany::file_stream stderr_stream(stderr);
        const zizany::file_lock lock(stderr_stream.get_lock());
        stderr_stream.puts("error: ");
        stderr_stream.puts(error.what());
        stderr_stream.putc('\n');
        return -1;
    }
}
