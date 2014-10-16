#include "unity_value_parser.hpp"

#include "stream_parser.hpp"
#include "parser_exception.hpp"
#include "unity_file_reference.hpp"
#include "unity_type.hpp"
#include "unity_value.hpp"
#include "values/unity_array_value.hpp"
#include "values/unity_asset_reference_value.hpp"
#include "values/unity_bool_value.hpp"
#include "values/unity_double_value.hpp"
#include "values/unity_composite_value.hpp"
#include "values/unity_float_value.hpp"
#include "values/unity_guid_value.hpp"
#include "values/unity_inline_array_value.hpp"
#include "values/unity_integer_value.hpp"
#include "values/unity_multiline_string_value.hpp"
#include "values/unity_string_value.hpp"

namespace zizany {
    template<typename value_type>
    static
    std::unique_ptr<unity_integer_value<value_type>>
    make_unity_integer_value(value_type value) {
        return std::unique_ptr<unity_integer_value<value_type>>
                (new unity_integer_value<value_type>(value));
    }

    static
    std::unique_ptr<unity_value>
    parse_simple_value(stream_parser &parser, const unity_type &type) {
        // XXX char and byte values should not be marked as integer
        if (type.name == "bool") {
            return std::unique_ptr<unity_bool_value>(new unity_bool_value(parser.parse<bool>()));
        } else if (type.name == "char") {
            return make_unity_integer_value(parser.parse<char>());
        } else if (type.name == "unsigned char") {
            return make_unity_integer_value(parser.parse<unsigned char>());
        } else if (type.name == "byte"
                || type.name == "SInt8") {
            return make_unity_integer_value(parser.parse<std::int8_t>());
        } else if (type.name == "unsigned byte"
                || type.name == "UInt8") {
            return make_unity_integer_value(parser.parse<std::uint8_t>());
        } else if (type.name == "short"
                || type.name == "SInt16") {
            return make_unity_integer_value(parser.parse<std::int16_t>());
        } else if (type.name == "unsigned short"
                || type.name == "UInt16") {
            return make_unity_integer_value(parser.parse<std::uint16_t>());
        } else if (type.name == "int"
                || type.name == "SInt32") {
            return make_unity_integer_value(parser.parse<std::int32_t>());
        } else if (type.name == "unsigned int"
                || type.name == "UInt32") {
            return make_unity_integer_value(parser.parse<std::uint32_t>());
        } else if (type.name == "SInt64") {
            return make_unity_integer_value(parser.parse<std::int64_t>());
        } else if (type.name == "UInt64") {
            return make_unity_integer_value(parser.parse<std::uint64_t>());
        } else if (type.name == "float") {
            return std::unique_ptr<unity_float_value>(new unity_float_value(parser.parse<float>()));
        } else if (type.name == "double") {
            return std::unique_ptr<unity_double_value>(new unity_double_value(parser.parse<double>()));
        } else {
            throw parser_exception("unexpected simple type");
        }
    }

    static
    std::unique_ptr<unity_array_value>
    parse_array(const unity_type &element_type, const std::size_t length, stream_parser &parser, const registry<unity_file_reference> &file_references) {
        std::unique_ptr<unity_array_value> elements_value;
        if (element_type.is_simple())
            elements_value.reset(new unity_inline_array_value());
        else
            elements_value.reset(new unity_array_value());
        if (length > 0) {
            elements_value->elements.reserve(length);
            for (std::size_t index = 0; index < length; ++index)
                elements_value->elements.add(parse_value(parser, element_type, "", file_references));
        }
        return elements_value;
    }

    std::unique_ptr<unity_composite_value>
    parse_composite(stream_parser &parser, const unity_type &type, const registry<unity_file_reference> &file_references) {
        std::unique_ptr<unity_composite_value> composite_value(new unity_composite_value());
        const std::size_t members_count(type.members.size());
        if (members_count > 0) {
            composite_value->members.reserve(members_count);
            for (const unity_type_member &member : type.members)
                composite_value->members.add(member.name, parse_value(parser, member.type, member.name, file_references));
        }
        return composite_value;
    }

    std::unique_ptr<unity_value>
    parse_value(stream_parser &parser, const unity_type &type, const std::string &member_name, const registry<unity_file_reference> &file_references) {
        std::unique_ptr<unity_value> ret;
        if (type.is_array) {
            if (type.members.size() != 2)
                throw parser_exception("array type should have two members");
            const unity_type &length_type(type.members.at(0).type);
            if (length_type.name != "int" && length_type.name != "SInt32")
                throw parser_exception("type of length for array should be int or SInt32");
            const std::uint32_t length(parser.parse<std::uint32_t>());

            const unity_type &element_type(type.members.at(1).type);
            ret = parse_array(element_type, length, parser, file_references);
        } else if (type.is_simple()) {
            ret = parse_simple_value(parser, type);
        } else if (type.name == "string") {
            // XXX: we should check that the type named "string" is actually what we expect
            std::unique_ptr<unity_string_value> string_value;
            if (member_name == "m_Script")
                string_value.reset(new unity_multiline_string_value());
            else
                string_value.reset(new unity_string_value());
            const std::uint32_t length(parser.parse<std::uint32_t>());
            if (length > 0) {
                parser.parse(string_value->data, length);
                parser.align(4);
            }
            ret = std::move(string_value);
        } else if (type.name == "GUID") {
            // XXX: we should check that the type named "GUID" is actually what we expect
            std::unique_ptr<unity_guid_value> guid_value(new unity_guid_value());
            guid_value->value.a = parser.parse<std::uint32_t>();
            guid_value->value.b = parser.parse<std::uint32_t>();
            guid_value->value.c = parser.parse<std::uint32_t>();
            guid_value->value.d = parser.parse<std::uint32_t>();
            ret = std::move(guid_value);
        } else if (type.name.find("PPtr<") == 0 && type.name.at(type.name.size() - 1) == '>') {
            // XXX: we should check that the type named "PPtr<*>" is actually what we expect
            std::unique_ptr<unity_asset_reference_value> asset_reference_value(new unity_asset_reference_value());
            const std::int32_t file_reference_id = parser.parse<std::int32_t>();
            if (file_reference_id == 0)
                asset_reference_value->value.is_local = true;
            else {
                asset_reference_value->value.is_local = false;
                asset_reference_value->value.file_guid = file_references.get_by_id(file_reference_id).file_guid;
            }
            asset_reference_value->value.asset_id = parser.parse<std::int32_t>();
            ret = std::move(asset_reference_value);
        } else {
            ret = parse_composite(parser, type, file_references);
        }
        if (type.requires_padding())
            parser.align(4);
        return ret;
    }
}
