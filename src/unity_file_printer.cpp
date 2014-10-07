#include "unity_file_printer.hpp"

#include "json_writer.hpp"
#include "unity_asset.hpp"
#include "unity_file.hpp"
#include "unity_file_reference.hpp"
#include "unity_preview.hpp"
#include "unity_type.hpp"
#include "values/unity_value.hpp"

#include <cstdio>

namespace zizany {
    static
    void
    print_hexdump(json_writer &writer, const std::vector<char> &data) {
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

    void
    print_file(json_writer &writer, const unity_file &file, unity_file_printer_options options) {
        writer.start_object();
        writer.add_key("unity_version");
        writer.add_string(file.unity_version);
        if (options.print_layout) {
            writer.add_key("is_big_endian");
            writer.add_bool(file.file_layout.file_endianness == zizany::endianness::big_endian);
            writer.add_key("assets_start");
            writer.add_number(file.file_layout.assets_start);
            writer.add_key("previews_start");
            writer.add_number(file.file_layout.previews_start);
            writer.add_key("metadata_size");
            writer.add_number(file.file_layout.metadata_size);
        }
        if (options.print_magic) {
            writer.add_key("magic_int_1");
            writer.add_number(file.magic_int_1);
            writer.add_key("magic_int_2");
            writer.add_number(file.magic_int_2);
            writer.add_key("magic_int_3");
            writer.add_number(file.magic_int_3);
        }
        if (options.print_types) {
            writer.add_key("types");
            writer.start_array();
            for (std::size_t index = 0; index < file.types.size(); ++index) {
                writer.start_object();
                writer.add_key("id");
                writer.add_number(file.types.get_id_at(index));
                writer.add_key("definition");
                print_type(writer, file.types.at(index), options.print_defaults, options.print_magic);
                writer.end_object();
            }
            writer.end_array();
        }
        if (options.print_assets) {
            writer.add_key("assets");
            writer.start_array();
            for (std::size_t index = 0; index < file.assets.size(); ++index) {
                const zizany::unity_asset &asset(file.assets.at(index));
                writer.start_object();
                writer.add_key("id");
                writer.add_number(file.assets.get_id_at(index));
                writer.add_key("type_id");
                writer.add_number(asset.type_id);
                writer.add_key("type_id_2");
                writer.add_number(asset.type_id_2);
                if (options.print_layout) {
                    writer.add_key("offset");
                    writer.add_number(asset.file_layout.offset);
                    writer.add_key("size");
                    writer.add_number(asset.file_layout.size);
                }
                if (asset.value) {
                    writer.add_key("value");
                    asset.value->print(writer);
                } else {
                    writer.add_key("unparsed_value");
                    print_hexdump(writer, asset.unparsed_value);
                }
                writer.end_object();
            }
            writer.end_array();
        }
        if (options.print_file_references) {
            writer.add_key("file_references");
            writer.start_array();
            for (std::size_t index = 0; index < file.file_references.size(); ++index) {
                const int file_reference_id(file.file_references.get_id_at(index));
                const zizany::unity_file_reference &file_reference(file.file_references.at(index));
                writer.start_object();
                writer.add_key("id");
                writer.add_number(file_reference_id);
                writer.add_key("guid");
                file_reference.file_guid.print(writer);
                writer.add_key("path");
                writer.add_string(file_reference.path);
                if (options.print_magic) {
                    writer.add_key("magic_byte_1");
                    writer.add_number(file_reference.magic_byte_1);
                    writer.add_key("magic_int_2");
                    writer.add_number(file_reference.magic_int_2);
                }
                writer.end_object();
            }
            writer.end_array();
        }
        if (options.print_previews) {
            writer.add_key("previews");
            writer.start_array();
            for (const unity_preview &preview : file.previews) {
                writer.start_object();
                writer.add_key("asset_id");
                writer.add_number(preview.asset_id);
                if (options.print_layout) {
                    writer.add_key("offset");
                    writer.add_number(preview.file_layout.offset);
                    writer.add_key("size");
                    writer.add_number(preview.file_layout.size);
                }
                if (options.print_magic) {
                    writer.add_key("magic_int_1");
                    writer.add_number(preview.magic_int_1);
                    writer.add_key("magic_int_2");
                    writer.add_number(preview.magic_int_2);
                    writer.add_key("magic_int_3");
                    writer.add_number(preview.magic_int_3);
                }
                writer.add_key("data");
                writer.start_array(true);
                for (const unsigned char byte : preview.data)
                    writer.add_number(byte);
                writer.end_array();
                writer.end_object();
            }
            writer.end_array();
        }
        writer.end_object();
    }

    void
    print_type(json_writer &writer, const unity_type &type, bool print_defaults, bool print_magic) {
        writer.start_object();
        writer.add_key("name");
        writer.add_string(type.member_name);
        writer.add_key("type");
        writer.add_string(type.type_name);
        if (type.is_array || print_defaults) {
            writer.add_key("is_array");
            writer.add_bool(type.is_array);
        }
        if (type.type_size != -1 || print_defaults) {
            writer.add_key("size");
            writer.add_number(type.type_size);
        }
        if (print_magic) {
            writer.add_key("magic_int_1");
            writer.add_number(type.magic_int_1);
            writer.add_key("magic_bitset_2");
            writer.add_number(type.magic_bitset_2);
        }
        if (type.members.size() > 0 || print_defaults) {
            writer.add_key("members");
            writer.start_array();
            for (const unity_type &member_type : type.members)
                print_type(writer, member_type, print_defaults, print_magic);
            writer.end_array();
        }
        writer.end_object();
    }
}
