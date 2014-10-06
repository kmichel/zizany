#include "unity_value_parser.hpp"

#include "stream_parser.hpp"
#include "parser_exception.hpp"
#include "unity_type.hpp"
#include "values/unity_value.hpp"
#include "values/unity_array_value.hpp"
#include "values/unity_asset_reference_value.hpp"
#include "values/unity_composite_value.hpp"
#include "values/unity_guid_value.hpp"
#include "values/unity_dense_array_value.hpp"
#include "values/unity_multiline_string_value.hpp"
#include "values/unity_simple_value.hpp"
#include "values/unity_string_value.hpp"

namespace zizany {

    static
    std::unique_ptr<unity_value>
    parse_simple_value(stream_parser &parser, const unity_type &type, unity_value *parent) {
        if (type.type_name == "bool") {
            return make_simple_value(type, parent, parser.parse<bool>());
        } else if (type.type_name == "char") {
            return make_simple_value(type, parent, parser.parse<char>());
        } else if (type.type_name == "unsigned char") {
            return make_simple_value(type, parent, parser.parse<unsigned char>());
        } else if (type.type_name == "byte"
                || type.type_name == "SInt8") {
            return make_simple_value(type, parent, parser.parse<std::int8_t>());
        } else if (type.type_name == "unsigned byte"
                || type.type_name == "UInt8") {
            return make_simple_value(type, parent, parser.parse<std::uint8_t>());
        } else if (type.type_name == "short"
                || type.type_name == "SInt16") {
            return make_simple_value(type, parent, parser.parse<std::int16_t>());
        } else if (type.type_name == "unsigned short"
                || type.type_name == "UInt16") {
            return make_simple_value(type, parent, parser.parse<std::uint16_t>());
        } else if (type.type_name == "int"
                || type.type_name == "SInt32") {
            return make_simple_value(type, parent, parser.parse<std::int32_t>());
        } else if (type.type_name == "unsigned int"
                || type.type_name == "UInt32") {
            return make_simple_value(type, parent, parser.parse<std::uint32_t>());
        } else if (type.type_name == "SInt64") {
            return make_simple_value(type, parent, parser.parse<std::int64_t>());
        } else if (type.type_name == "UInt64") {
            return make_simple_value(type, parent, parser.parse<std::uint64_t>());
        } else if (type.type_name == "float") {
            return make_simple_value(type, parent, parser.parse<float>());
        } else if (type.type_name == "double") {
            return make_simple_value(type, parent, parser.parse<double>());
        } else {
            throw parser_exception("unexpected simple type");
        }
    }

    template<typename element_type>
    std::unique_ptr<unity_dense_array_value<element_type>>
    parse_dense_array_(const std::size_t length, stream_parser &parser, const unity_type &type, unity_value *parent) {
        std::unique_ptr<unity_dense_array_value<element_type>> elements_value(make_dense_array_value<element_type>(type, parent));
        parser.parse(elements_value->elements, length);
        return elements_value;
    }

    static
    std::unique_ptr<unity_value>
    parse_dense_array(const unity_type &element_type, const std::size_t length, stream_parser &parser, const unity_type &type, unity_value *parent) {
        if (element_type.type_name == "bool") {
            return parse_dense_array_<bool>(length, parser, type, parent);
        } else if (element_type.type_name == "char") {
            return parse_dense_array_<char>(length, parser, type, parent);
        } else if (element_type.type_name == "unsigned char") {
            return parse_dense_array_<unsigned char>(length, parser, type, parent);
        } else if (element_type.type_name == "byte"
                || element_type.type_name == "SInt8") {
            return parse_dense_array_<std::int8_t>(length, parser, type, parent);
        } else if (element_type.type_name == "unsigned byte"
                || element_type.type_name == "UInt8") {
            return parse_dense_array_<std::uint8_t>(length, parser, type, parent);
        } else if (element_type.type_name == "short"
                || element_type.type_name == "SInt16") {
            return parse_dense_array_<std::int16_t>(length, parser, type, parent);
        } else if (element_type.type_name == "unsigned short"
                || element_type.type_name == "UInt16") {
            return parse_dense_array_<std::uint16_t>(length, parser, type, parent);
        } else if (element_type.type_name == "int"
                || element_type.type_name == "SInt32") {
            return parse_dense_array_<std::int32_t>(length, parser, type, parent);
        } else if (element_type.type_name == "unsigned int"
                || element_type.type_name == "UInt32") {
            return parse_dense_array_<std::uint32_t>(length, parser, type, parent);
        } else if (element_type.type_name == "SInt64") {
            return parse_dense_array_<std::int64_t>(length, parser, type, parent);
        } else if (element_type.type_name == "UInt64") {
            return parse_dense_array_<std::uint64_t>(length, parser, type, parent);
        } else if (element_type.type_name == "float") {
            return parse_dense_array_<float>(length, parser, type, parent);
        } else if (element_type.type_name == "double") {
            return parse_dense_array_<double>(length, parser, type, parent);
        } else {
            throw parser_exception("unexpected simple type");
        }
    }

    static
    std::unique_ptr<unity_array_value>
    parse_array(const unity_type &element_type, const std::size_t length, stream_parser &parser, const unity_type &type, unity_value *parent) {
        std::unique_ptr<unity_array_value> elements_value(new unity_array_value(type, parent));
        if (length > 0) {
            elements_value->elements.reserve(length);
            for (std::size_t index = 0; index < length; ++index)
                elements_value->elements.add(parse_value(parser, element_type, elements_value.get()));
        }
        return elements_value;
    }

    static
    std::unique_ptr<unity_composite_value>
    parse_composite(stream_parser &parser, const unity_type &type, unity_value *parent) {
        std::unique_ptr<unity_composite_value> composite_value(new unity_composite_value(type, parent));
        const std::size_t members_count(type.members.size());
        if (members_count > 0) {
            composite_value->members.reserve(members_count);
            for (const unity_type &member_type : type.members)
                composite_value->members.add(parse_value(parser,member_type, composite_value.get()));
        }
        return composite_value;
    }


    std::unique_ptr<unity_value>
    parse_value(stream_parser &parser, const unity_type &type, unity_value *parent) {
        std::unique_ptr<unity_value> ret;
        if (type.is_array) {
            const unity_type &length_type(type.members.at(0));
            if (length_type.type_name != "int" && length_type.type_name != "SInt32")
                throw parser_exception("type of length for array should be int or SInt32");
            const std::uint32_t length(parser.parse<std::uint32_t>());

            const unity_type &element_type(type.members.at(1));
            if (element_type.is_simple())
                ret = parse_dense_array(element_type, length, parser, type, parent);
            else
                ret = parse_array(element_type, length, parser, type, parent);
        } else if (type.is_simple()) {
            ret = parse_simple_value(parser, type, parent);
        } else if (type.type_name == "string") {
            // XXX: we should check that the type named "string" is actually what we expect
            std::unique_ptr<unity_string_value> string_value;
            if (type.member_name == "m_Script")
                string_value.reset(new unity_multiline_string_value(type, parent));
            else
                string_value.reset(new unity_string_value(type, parent));
            const std::uint32_t length(parser.parse<std::uint32_t>());
            if (length > 0) {
                parser.parse(string_value->chars, length);
                parser.align(4);
            }
            ret = std::move(string_value);
        } else if (type.type_name == "GUID") {
            // XXX: we should check that the type named "GUID" is actually what we expect
            std::unique_ptr<unity_guid_value> guid_value(new unity_guid_value(type, parent));
            guid_value->value.a = parser.parse<std::uint32_t>();
            guid_value->value.b = parser.parse<std::uint32_t>();
            guid_value->value.c = parser.parse<std::uint32_t>();
            guid_value->value.d = parser.parse<std::uint32_t>();
            ret = std::move(guid_value);
        } else if (type.type_name.find("PPtr<") == 0 && type.type_name.at(type.type_name.size() -1) == '>') {
            // XXX: we should check that the type named "PPtr<*>" is actually what we expect
            std::unique_ptr<unity_asset_reference_value> asset_reference_value(new unity_asset_reference_value(type, parent));
            asset_reference_value->file_reference_id = parser.parse<std::int32_t>();
            asset_reference_value->asset_id = parser.parse<std::int32_t>();
            ret = std::move(asset_reference_value);
        } else {
            ret = parse_composite(parser, type, parent);
        }
        if (type.requires_padding())
            parser.align(4);
        return ret;
    }
}
