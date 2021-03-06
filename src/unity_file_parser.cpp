#include "unity_file_parser.hpp"

#include "file_stream.hpp"
#include "parser_exception.hpp"
#include "pod_vector.hpp"
#include "stream_parser.hpp"
#include "type_factory.hpp"
#include "unity_asset.hpp"
#include "unity_file.hpp"
#include "unity_file_reference.hpp"
#include "unity_preview.hpp"
#include "unity_value.hpp"
#include "value_parser.hpp"
#include "values/unity_blob_value.hpp"

namespace zizany {
    unity_file_parser::unity_file_parser(unity_file &file_)
            : file(file_), checker(), fallback_types() {
    }

    void
    unity_file_parser::parse(file_stream &stream, range_checker::overlapping_ranges_callback overlapping_ranges_callback) {
        parse_header(stream);
        {
            stream_parser parser(stream, file.file_layout.file_endianness, checker);
            // normal files have a value of -2, released files have a value of 4 or 5
            // released files do not contain type definition and use a different
            // structure when serializing objects of known type.
            file.magic_int_2 = parser.parse<std::int32_t>();
            parse_types(parser);
            if (file.magic_int_2 != -2) {
                // For release files, we include some basic definitions in order to be able
                // to resolve type identities during diff. It would be nice to have more definitions,
                // however we can't use those found in normal files, some members, mostly related
                // to editor behavior are missing or different.
                fallback_types.add(1, std::unique_ptr<unity_type>(new unity_type(create_release_gameobject_type())));
                fallback_types.add(114, std::unique_ptr<unity_type>(new unity_type(create_release_monobehaviour_type())));
                fallback_types.add(115, std::unique_ptr<unity_type>(new unity_type(create_release_monoscript_type())));
            }
            file.magic_int_3 = parser.parse<std::int32_t>();
            if (file.magic_int_3 != 0)
                throw parser_exception("magic_int_3 value should be zero");
            parse_assets(parser);
            parse_file_references(parser);
            parse_previews(parser);
            // moved at the end because :
            // - asset_value and preview_data require many seeks to parse
            // - parsing asset values requires file_references
            for (unity_asset &asset : file.assets)
                parse_asset_value(parser, asset);
            for (unity_preview &preview : file.previews)
                parse_preview_data(parser, preview);
        }
        checker.check([&stream](std::int64_t start, std::int64_t end) {
            stream.seek(start, SEEK_SET);
            const std::size_t size(static_cast<std::size_t>(end - start));
            std::unique_ptr<char[]> buffer(new char[size]);
            stream.read(buffer.get(), size, 1);
            for (std::size_t index = 0; index < size; ++index)
                if (buffer[index] != 0)
                    throw parser_exception("padding should be filled with zeroes");
        }, overlapping_ranges_callback);
    }

    void
    unity_file_parser::parse_header(file_stream &stream) {
        stream_parser big_endian_parser(stream, endianness::big_endian, checker);
        file.file_layout.metadata_size = big_endian_parser.parse<std::uint32_t>();
        file.file_layout.previews_start = big_endian_parser.parse<std::uint32_t>();
        file.magic_int_1 = big_endian_parser.parse<std::int32_t>();
        if (file.magic_int_1 != 9)
            throw parser_exception("magic_int_1 value should be 9");
        file.file_layout.assets_start = big_endian_parser.parse<std::uint32_t>();
        file.file_layout.file_endianness = big_endian_parser.parse<bool>() ? endianness::big_endian : endianness::little_endian;
        big_endian_parser.align(4);
        file.unity_version = big_endian_parser.parse_string();
    }

    void
    unity_file_parser::parse_types(stream_parser &parser) {
        const std::uint32_t types_count(parser.parse<std::uint32_t>());
        for (std::uint32_t index = 0; index < types_count; ++index) {
            std::int32_t type_id(parser.parse<std::int32_t>());
            std::int32_t expected_definition_index(0);
            unity_type_member type_member(parse_type_member(parser, expected_definition_index));
            if (type_member.name != "Base")
                throw parser_exception("unexpected name for base type");
            file.types.add(type_id, std::unique_ptr<unity_type>(new unity_type(type_member.type)));
        }
    }

    unity_type_member
    unity_file_parser::parse_type_member(stream_parser &parser, std::int32_t &expected_definition_index) {
        unity_type_member type_member;
        type_member.type.name = parser.parse_string();
        type_member.name = parser.parse_string();
        type_member.type.size = parser.parse<std::int32_t>();
        std::int32_t definition_index(parser.parse<std::int32_t>());
        if (definition_index != expected_definition_index)
            throw parser_exception("unexpected definition index");
        ++expected_definition_index;
        type_member.type.is_array = parser.parse<std::int32_t>() != 0;
        type_member.type.magic_int_1 = parser.parse<std::int32_t>();
        type_member.type.magic_bitset_2 = parser.parse<std::uint32_t>();
        const std::uint32_t members_count(parser.parse<std::uint32_t>());
        if (members_count > 0) {
            type_member.type.members.reserve(members_count);
            for (std::uint32_t index = 0; index < members_count; ++index)
                type_member.type.members.push_back(parse_type_member(parser, expected_definition_index));
        }
        return type_member;
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
        asset->file_layout.offset = parser.parse<std::uint32_t>();
        asset->file_layout.size = parser.parse<std::uint32_t>();
        asset->type_id = parser.parse<std::int32_t>();
        // usually type_id == type_id_2, except for MonoBehaviour, type_id < 0 && type_id_2 = 114
        asset->type_id_2 = parser.parse<std::int32_t>();
        return asset;
    }

    void
    unity_file_parser::parse_asset_value(stream_parser &parser, unity_asset &asset) {
        parser.seek(file.file_layout.assets_start + asset.file_layout.offset);
        const std::int64_t expected_end = file.file_layout.assets_start + asset.file_layout.offset + asset.file_layout.size;
        // XXX: we should cache the value parser, but this would be easier if unity_type were immutables
        if (file.types.has_id(asset.type_id))
            asset.value = file.types.get_by_id(asset.type_id).get_value_parser()->parse_value(parser, file.file_references, expected_end);
        else if (fallback_types.has_id(asset.type_id_2))
            asset.value = fallback_types.get_by_id(asset.type_id_2).get_value_parser()->parse_value(parser, file.file_references, expected_end);
        else {
            std::unique_ptr<unity_blob_value> blob(new unity_blob_value);
            parser.parse(blob->data, asset.file_layout.size);
            asset.value = std::move(blob);
        }
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
        file_reference->properties.magic_byte_1 = parser.parse<unsigned char>();
        file_reference->file_guid.a = parser.parse<std::uint32_t>();
        file_reference->file_guid.b = parser.parse<std::uint32_t>();
        file_reference->file_guid.c = parser.parse<std::uint32_t>();
        file_reference->file_guid.d = parser.parse<std::uint32_t>();
        file_reference->properties.magic_int_2 = parser.parse<std::int32_t>();
        file_reference->properties.path = parser.parse_string();
        return file_reference;
    }

    void
    unity_file_parser::parse_previews(stream_parser &parser) {
        parser.seek_from_end(0);
        if (file.file_layout.previews_start != parser.tell()) {
            const char expected_marker[] = "PreviewAssetData";
            const std::size_t marker_length(sizeof(expected_marker) - 1);
            const std::size_t count_length(sizeof(std::int32_t));
            parser.seek_from_end(-static_cast<std::int64_t>(marker_length + count_length));
            pod_vector<char> marker;
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
        preview->file_layout.size = parser.parse<std::uint32_t>();
        preview->file_layout.offset = parser.parse<std::uint32_t>();
        return preview;
    }

    void
    unity_file_parser::parse_preview_data(stream_parser &parser, unity_preview &preview) {
        parser.seek(preview.file_layout.offset);
        parser.parse(preview.data, preview.file_layout.size);
    }
}
