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
                file.types.at(index).print(writer, options.print_defaults, options.print_magic);
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
                writer.add_key("value");
                if (asset.value)
                    asset.value->print(writer);
                else
                    writer.add_null();
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
            for (const unity_preview &preview : file.previews)
                preview.print(writer, options.print_layout, options.print_magic);
            writer.end_array();
        }
        writer.end_object();
    }
}
