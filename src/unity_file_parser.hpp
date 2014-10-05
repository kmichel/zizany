#pragma once

#include <cstdint>
#include <iosfwd>
#include <memory>

#include "range_checker.hpp"

namespace zizany {
    class stream_parser;

    class unity_asset;

    class unity_file;

    class unity_file_reference;

    class unity_preview;

    class unity_type;

    class unity_file_parser {
        unity_file &file;
        range_checker checker;
    public:
        explicit unity_file_parser(unity_file &file_);

        unity_file_parser(const unity_file_parser &) = delete;

        unity_file_parser &operator=(const unity_file_parser &) = delete;

        void parse(std::istream &stream, range_checker::overlapping_ranges_callback overlapping_ranges_callback);

        void parse_header(std::istream &stream);

        void parse_types(stream_parser &parser);

        std::unique_ptr<unity_type> parse_type(stream_parser &parser, unity_type *parent, std::int32_t &expected_definition_index);

        void parse_assets(stream_parser &parser);

        std::unique_ptr<unity_asset> parse_asset(stream_parser &parser);

        void parse_asset_value(stream_parser &parser, unity_asset &asset);

        void parse_file_references(stream_parser &parser);

        std::unique_ptr<unity_file_reference> parse_file_reference(stream_parser &parser);

        void parse_previews(stream_parser &parser);

        std::unique_ptr<unity_preview> parse_preview(stream_parser &parser);

        void parse_preview_data(stream_parser &parser, unity_preview &preview);
    };
}
