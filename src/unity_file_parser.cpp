#include "unity_file_parser.hpp"

#include "parser_exception.hpp"
#include "stream_parser.hpp"
#include "unity_asset.hpp"
#include "unity_file.hpp"
#include "unity_file_reference.hpp"
#include "unity_preview.hpp"
#include "unity_type.hpp"
#include "unity_value_parser.hpp"
#include "values/unity_value.hpp"

namespace zizany {
    unity_file_parser::unity_file_parser(unity_file &file_)
            : file(file_), checker() {
    }

    void
    unity_file_parser::parse(std::istream &stream, range_checker::overlapping_ranges_callback overlapping_ranges_callback) {
        parse_header(stream);
        {
            stream_parser parser(stream, file.artifact_data.file_endianness, checker);
            // normal files have a value of -2, released files have a value of 4 or 5
            // released files do not contain type definition and use a different
            // structure when serializing objects of known type.
            file.magic_int_2 = parser.parse<std::int32_t>();
            parse_types(parser);
            file.magic_int_3 = parser.parse<std::int32_t>();
            if (file.magic_int_3 != 0)
                throw parser_exception("magic_int_3 value should be zero");
            parse_assets(parser);
            parse_file_references(parser);
            parse_previews(parser);
            // moved at the end because asset_value and preview_data require many seeks to parse
            for (unity_asset &asset : file.assets)
                parse_asset_value(parser, asset);
            for (unity_preview &preview : file.previews)
                parse_preview_data(parser, preview);
        }
        checker.check([&stream](std::int64_t start, std::int64_t end) {
            stream.seekg(start, std::ios_base::beg);
            const std::size_t size(static_cast<std::size_t>(end - start));
            std::unique_ptr<char[]> buffer(new char[size]);
            stream.read(buffer.get(), end - start);
            for (std::size_t index = 0; index < size; ++index)
                if (buffer[index] != 0)
                    throw parser_exception("padding should be filled with zeroes");
        }, overlapping_ranges_callback);
    }

    void
    unity_file_parser::parse_header(std::istream &stream) {
        stream_parser big_endian_parser(stream, endianness::big_endian, checker);
        file.artifact_data.metadata_size = big_endian_parser.parse<std::uint32_t>();
        file.artifact_data.previews_start = big_endian_parser.parse<std::uint32_t>();
        file.magic_int_1 = big_endian_parser.parse<std::int32_t>();
        if (file.magic_int_1 != 9)
            throw parser_exception("magic_int_1 value should be 9");
        file.artifact_data.assets_start = big_endian_parser.parse<std::uint32_t>();
        file.artifact_data.file_endianness = big_endian_parser.parse<bool>() ? endianness::big_endian : endianness::little_endian;
        big_endian_parser.align(4);
        file.unity_version = big_endian_parser.parse_string();
    }

    void
    unity_file_parser::parse_types(stream_parser &parser) {
        const std::uint32_t types_count(parser.parse<std::uint32_t>());
        for (std::uint32_t index = 0; index < types_count; ++index) {
            std::int32_t type_id(parser.parse<std::int32_t>());
            std::int32_t expected_definition_index(0);
            file.types.add(type_id, parse_type(parser, expected_definition_index));
        }
    }

    std::unique_ptr<unity_type>
    unity_file_parser::parse_type(stream_parser &parser, std::int32_t &expected_definition_index) {
        std::unique_ptr<unity_type> type(new unity_type());
        type->type_name = parser.parse_string();
        type->member_name = parser.parse_string();
        type->type_size = parser.parse<std::int32_t>();
        std::int32_t definition_index(parser.parse<std::int32_t>());
        if (definition_index != expected_definition_index)
            throw parser_exception("unexpected definition index");
        ++expected_definition_index;
        type->is_array = parser.parse<std::int32_t>() != 0;
        type->magic_int_1 = parser.parse<std::int32_t>();
        type->magic_bitset_2 = parser.parse<std::uint32_t>();
        const std::uint32_t members_count(parser.parse<std::uint32_t>());
        if (members_count > 0) {
            type->members.reserve(members_count);
            for (std::uint32_t index = 0; index < members_count; ++index)
                type->members.add(parse_type(parser, expected_definition_index));
        }
        return type;
    }

    void
    unity_file_parser::parse_assets(stream_parser &parser) {
        const std::uint32_t assets_count(parser.parse<std::uint32_t>());
        for (std::uint32_t index = 0; index < assets_count; ++index) {
            const std::int32_t asset_id(parser.parse<std::int32_t>());
            file.assets.add(asset_id, parse_asset(parser));
        }
    }

    std::unique_ptr<unity_asset>
    unity_file_parser::parse_asset(stream_parser &parser) {
        std::unique_ptr<unity_asset> asset(new unity_asset);
        asset->artifact_data.offset = parser.parse<std::uint32_t>();
        asset->artifact_data.size = parser.parse<std::uint32_t>();
        asset->type_id = parser.parse<std::int32_t>();
        // usually type_id == type_id_2, except for monobehavior, type_id < 0 && type_id_2 = 114
        asset->type_id_2 = parser.parse<std::int32_t>();
        return asset;
    }

    void
    unity_file_parser::parse_asset_value(stream_parser &parser, unity_asset &asset) {
        parser.seek(file.artifact_data.assets_start + asset.artifact_data.offset);
        if (file.types.has_id(asset.type_id))
            asset.value = parse_value(parser, file.types.get_by_id(asset.type_id));
        else
            parser.parse(asset.unparsed_value, asset.artifact_data.size);
    }

    void
    unity_file_parser::parse_file_references(stream_parser &parser) {
        const std::int32_t file_references_count(parser.parse<std::int32_t>());
        for (std::int32_t index = 0; index < file_references_count; ++index) {
            file.file_references.add(index + 1, parse_file_reference(parser));
        }
    }

    std::unique_ptr<unity_file_reference>
    unity_file_parser::parse_file_reference(stream_parser &parser) {
        std::unique_ptr<unity_file_reference> file_reference(new unity_file_reference);
        file_reference->magic_byte_1 = parser.parse<unsigned char>();
        if (file_reference->magic_byte_1 != 0)
            throw parser_exception("magic_byte_1 value should be zero");
        file_reference->file_guid.a = parser.parse<std::uint32_t>();
        file_reference->file_guid.b = parser.parse<std::uint32_t>();
        file_reference->file_guid.c = parser.parse<std::uint32_t>();
        file_reference->file_guid.d = parser.parse<std::uint32_t>();
        file_reference->magic_int_2 = parser.parse<std::int32_t>();
        file_reference->path = parser.parse_string();
        return file_reference;
    }

    void
    unity_file_parser::parse_previews(stream_parser &parser) {
        parser.seek_from_end(0);
        if (file.artifact_data.previews_start != parser.tell()) {
            const char expected_marker[] = "PreviewAssetData";
            const std::size_t marker_length(sizeof(expected_marker) - 1);
            const std::size_t count_length(sizeof(std::int32_t));
            parser.seek_from_end(-static_cast<std::int64_t>(marker_length + count_length));
            std::vector<char> marker;
            parser.parse(marker, marker_length);
            for (std::size_t index = 0; index < marker_length; ++index)
                if (marker[index] != expected_marker[index])
                    throw parser_exception("previews header should contain the string 'PreviewAssetData'");
            const std::int32_t previews_count(parser.parse<std::int32_t>());
            if (previews_count > 0) {
                parser.seek_from_end(-static_cast<std::int64_t>(marker_length + count_length) - 24 * previews_count);
                for (std::int32_t index = 0; index < previews_count; ++index)
                    file.previews.add(previews_count - index, parse_preview(parser));
            }
        }
    }

    std::unique_ptr<unity_preview>
    unity_file_parser::parse_preview(stream_parser &parser) {
        std::unique_ptr<unity_preview> preview(new unity_preview);
        preview->asset_id = parser.parse<std::int32_t>();
        preview->magic_int_1 = parser.parse<std::int32_t>();
        preview->magic_int_2 = parser.parse<std::int32_t>();
        preview->magic_int_3 = parser.parse<std::int32_t>();
        preview->artifact_data.size = parser.parse<std::uint32_t>();
        preview->artifact_data.offset = parser.parse<std::uint32_t>();
        return preview;
    }

    void
    unity_file_parser::parse_preview_data(stream_parser &parser, unity_preview &preview) {
        parser.seek(preview.artifact_data.offset);
        parser.parse(preview.data, preview.artifact_data.size);
    }
}
